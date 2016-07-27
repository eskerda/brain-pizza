#include "SteeringBehaviors.h"

#include <iostream>
#include <cassert>

#include "MovingEntity.h"
#include "math/transformations.h"
#include "math/utils.h"
#include "math/geometry.h"
#include "BaseGameEntity.h"
#include "Obstacle.h"

//--------------------------- Constants ----------------------------------

//the radius of the constraining circle for the wander behavior
const double WanderRad    = 1.1; //Ganes que te de posar-se a fer cercles
//distance the wander circle is projected in front of the agent
const double WanderDist   = 6.0; //Velocitat a la que va
//the maximum amount of displacement along the circle each frame
const double WanderJitterPerSec = 60.0; //Trompicones que dona


//------------------------- ctor -----------------------------------------
//
//------------------------------------------------------------------------
SteeringBehavior::SteeringBehavior(MovingEntity* agent):
	m_pMovingEntity(agent),
	m_iFlags(0),
	m_dDBoxLength(10),
	m_dWeightWander(100),
	m_dWeightForward(100),
	m_dWeightBoundsAvoidance(400),
	m_dWeightObstacleAvoidance(200),
	m_Deceleration(normal),
	pursuitTarget(NULL),
	seekTarget(NULL),
	fleeTarget(NULL),
	hideTarget(NULL),
	m_dWeightSeek(100),
	m_dWeightFlee(100),
	m_dWeightArrive(100),
	m_dWeightPursuit(100),
	m_dWeightOffsetPursuit(50),
	m_dWeightHide(100),
	m_SummingMethod(weighted_average)
{
	//create a vector to a target position on the wander circle
	float theta = RandFloat() * TwoPi;
	m_vWanderTarget = Vector2D(WanderRad * cos(theta),
		WanderRad * sin(theta));  
}

//---------------------------------dtor ----------------------------------
SteeringBehavior::~SteeringBehavior(){
}


/////////////////////////////////////////////////////////////////////////////// CALCULATE METHODS 


//----------------------- Calculate --------------------------------------
//
//  calculates the accumulated steering force according to the method set
//  in m_SummingMethod
//------------------------------------------------------------------------
Vector2D SteeringBehavior::Calculate(float dt)
{ 
	switch (m_SummingMethod) {
		case weighted_average:
			return CalculateWeightedSum(dt); 

		case prioritized:
			return CalculatePrioritized(dt);

		default:
			return Vector2D(0,0);
	}
}

//--------------------- AccumulateForce ----------------------------------
//
//  This function calculates how much of its max steering force the 
//  MovingEntity has left to apply and then applies that amount of the
//  force to add.
//------------------------------------------------------------------------
bool SteeringBehavior::AccumulateForce(Vector2D &RunningTot,
	Vector2D ForceToAdd)
{

	//calculate how much steering force the MovingEntity has used so far
	double MagnitudeSoFar = RunningTot.Length();

	//calculate how much steering force remains to be used by this MovingEntity
	double MagnitudeRemaining = m_pMovingEntity->MaxForce() - MagnitudeSoFar;

	//return false if there is no more force left to use
	if (MagnitudeRemaining <= 0.0) return false;

	//calculate the magnitude of the force we want to add
	double MagnitudeToAdd = ForceToAdd.Length();

	//if the magnitude of the sum of ForceToAdd and the running total
	//does not exceed the maximum force available to this MovingEntity, just
	//add together. Otherwise add as much of the ForceToAdd vector is
	//possible without going over the max.
	if (MagnitudeToAdd < MagnitudeRemaining)
	{
		RunningTot += ForceToAdd;
	}

	else
	{
		//add it to the steering force
		RunningTot += (Vec2DNormalize(ForceToAdd) * MagnitudeRemaining); 
	}

	return true;
}



//---------------------- CalculatePrioritized ----------------------------
//
//  this method calls each active steering behavior in order of priority
//  and acumulates their forces until the max steering force magnitude
//  is reached, at which time the function returns the steering force 
//  accumulated to that  point
//------------------------------------------------------------------------
Vector2D SteeringBehavior::CalculatePrioritized(float dt)
{
	//reset the steering force
	m_vSteeringForce.Zero();

	Vector2D force;

	if (On(bounds_avoidance))
	{
		force = BoundsAvoidance() * m_dWeightBoundsAvoidance;

		if (!AccumulateForce(m_vSteeringForce, force)) return m_vSteeringForce;
	}

	if (On(obstacle_avoidance))
	{
		force = ObstacleAvoidance() * m_dWeightObstacleAvoidance;

		if (!AccumulateForce(m_vSteeringForce, force)) return m_vSteeringForce;
	}

	if (On(flee))
	{
		assert(fleeTarget && "Flee target not assigned");
		force = Flee(fleeTarget->Pos()) * m_dWeightFlee;

		if (!AccumulateForce(m_vSteeringForce, force)) return m_vSteeringForce;
	}

	if (On(seek))
	{
		assert(seekTarget && "Seek target not assigned");
		force = Seek(seekTarget->Pos()) * m_dWeightSeek;

		if (!AccumulateForce(m_vSteeringForce, force)) return m_vSteeringForce;
	}


	if (On(arrive))
	{
		assert(seekTarget && "Arrive target not assigned");
		force = Arrive(seekTarget->Pos(), m_Deceleration) * m_dWeightArrive;

		if (!AccumulateForce(m_vSteeringForce, force)) return m_vSteeringForce;
	}

	if (On(wander))
	{
		force = Wander(dt) * m_dWeightWander;

		if (!AccumulateForce(m_vSteeringForce, force)) return m_vSteeringForce;
	}

	if (On(pursuit))
	{
		assert(pursuitTarget && "pursuit target not assigned");

		force = Pursuit(pursuitTarget) * m_dWeightPursuit;

		if (!AccumulateForce(m_vSteeringForce, force)) return m_vSteeringForce;
	}

	if (On(offset_pursuit))
	{
		assert (pursuitTarget && "pursuit target not assigned");

		force = OffsetPursuit(pursuitTarget, m_vOffset);

		if (!AccumulateForce(m_vSteeringForce, force)) return m_vSteeringForce;
	}

	if (On(hide))
	{
		assert(hideTarget && "Hide target not assigned");

		force = Hide() * m_dWeightHide;

		if (!AccumulateForce(m_vSteeringForce, force)) return m_vSteeringForce;
	}

	if (On(forward)) {
		force = Forward() * m_dWeightForward;

		if (!AccumulateForce(m_vSteeringForce, force)) return m_vSteeringForce;
	}

	return m_vSteeringForce;
}


//---------------------- CalculateWeightedSum ----------------------------
//
//  this simply sums up all the active behaviors X their weights and 
//  truncates the result to the max available steering force before 
//  returning
//------------------------------------------------------------------------
Vector2D SteeringBehavior::CalculateWeightedSum(float dt)
{        
	//reset the steering force
	m_vSteeringForce.Zero();

	if (On(bounds_avoidance))
	{
		Vector2D force = BoundsAvoidance() * m_dWeightBoundsAvoidance;

		/*if (!force.isZero()) {
			std::cout << "avoid wall: " << force << std::endl;
		}*/

		m_vSteeringForce += force;
	}

	if (On(obstacle_avoidance))
	{
		Vector2D force = ObstacleAvoidance() * m_dWeightObstacleAvoidance;

		/*if (!force.isZero()) {
			std::cout << "avoid obst: " << force << std::endl;
		}*/

		m_vSteeringForce += force;
	}


	if (On(flee))
	{
		assert(fleeTarget && "Flee target not assigned");
		m_vSteeringForce += Flee(fleeTarget->Pos()) * m_dWeightFlee;
	}



	if (On(seek))
	{
		assert(seekTarget && "Seek target not assigned");
		m_vSteeringForce += Seek(seekTarget->Pos()) * m_dWeightSeek;
	}


	if (On(arrive))
	{
		assert(seekTarget && "Arrive target not assigned");
		m_vSteeringForce += Arrive(seekTarget->Pos(), m_Deceleration) * m_dWeightArrive;
	}


	if (On(wander))
	{

		Vector2D force = Wander(dt) * m_dWeightWander;

		//std::cout << "wander: " << force << std::endl;

		m_vSteeringForce += force;
	}



	if (On(pursuit))
	{
		assert(pursuitTarget && "pursuit target not assigned");

		m_vSteeringForce += Pursuit(pursuitTarget) * m_dWeightPursuit;
	}

	if (On(offset_pursuit))
	{
		assert (pursuitTarget && "pursuit target not assigned");

		m_vSteeringForce += OffsetPursuit(pursuitTarget, m_vOffset) * m_dWeightOffsetPursuit;
	}

	if (On(hide))
	{
		assert(hideTarget && "Hide target not assigned");

		m_vSteeringForce += Hide() * m_dWeightHide;
	}


	if (On(forward)) {
		Vector2D force = Forward() * m_dWeightForward;

		m_vSteeringForce += force;
	}

	//m_vSteeringForce.Truncate(m_pMovingEntity->MaxForce());
	return m_vSteeringForce;
}



/////////////////////////////////////////////////////////////////////////////// START OF BEHAVIORS

//------------------------------- Forward -----------------------------------
Vector2D SteeringBehavior::Forward()
{
	Vector2D force = m_pMovingEntity->Heading();
	return force;
}


//------------------------------- Seek -----------------------------------
//
//  Given a target, this behavior returns a steering force which will
//  direct the agent towards the target
//------------------------------------------------------------------------
Vector2D SteeringBehavior::Seek(Vector2D TargetPos)
{
	Vector2D DesiredVelocity = Vec2DNormalize(TargetPos - m_pMovingEntity->Pos())
		* m_pMovingEntity->MaxSpeed();

	return (DesiredVelocity - m_pMovingEntity->Velocity());
}

//----------------------------- Flee -------------------------------------
//
//  Does the opposite of Seek
//------------------------------------------------------------------------
Vector2D SteeringBehavior::Flee(Vector2D TargetPos)
{
	//only flee if the target is within 'panic distance'. Work in distance
	//squared space.
	/* const double PanicDistanceSq = 100.0f * 100.0;
	if (Vec2DDistanceSq(m_pMovingEntity->Pos(), target) > PanicDistanceSq)
	{
	return Vector2D(0,0);
	}
	*/

	Vector2D DesiredVelocity = Vec2DNormalize(m_pMovingEntity->Pos() - TargetPos) 
		* m_pMovingEntity->MaxSpeed();

	return (DesiredVelocity - m_pMovingEntity->Velocity());
}

//--------------------------- Arrive -------------------------------------
//
//  This behavior is similar to seek but it attempts to arrive at the
//  target with a zero velocity
//------------------------------------------------------------------------
Vector2D SteeringBehavior::Arrive(Vector2D TargetPos, Deceleration deceleration)
{
	Vector2D ToTarget = TargetPos - m_pMovingEntity->Pos();

	//calculate the distance to the target
	double dist = ToTarget.Length();

	if (dist > 5.f)
	{
		//because Deceleration is enumerated as an int, this value is required
		//to provide fine tweaking of the deceleration..
		const double DecelerationTweaker = 0.3;

		//calculate the speed required to reach the target given the desired
		//deceleration
		double speed =  dist / ((double)deceleration * DecelerationTweaker);     

		//make sure the velocity does not exceed the max
        speed = std::min(speed, (double)m_pMovingEntity->MaxSpeed());

		//from here proceed just like Seek except we don't need to normalize 
		//the ToTarget vector because we have already gone to the trouble
		//of calculating its length: dist. 
		Vector2D DesiredVelocity =  ToTarget * speed / dist;

		return (DesiredVelocity - m_pMovingEntity->Velocity());
	}

	return Vector2D(0,0);
}

//------------------------------ Pursuit ---------------------------------
//
//  this behavior creates a force that steers the agent towards the 
//  evader
//------------------------------------------------------------------------
Vector2D SteeringBehavior::Pursuit(const MovingEntity* evader)
{
	//if the evader is ahead and facing the agent then we can just seek
	//for the evader's current position.
	Vector2D ToEvader = evader->Pos() - m_pMovingEntity->Pos();

	double RelativeHeading = m_pMovingEntity->Heading().Dot(evader->Heading());

	if ( (ToEvader.Dot(m_pMovingEntity->Heading()) > 0) &&  
		(RelativeHeading < -0.95))  //acos(0.95)=18 degs
	{
		return Seek(evader->Pos());
	}

	//Not considered ahead so we predict where the evader will be.

	//the lookahead time is propotional to the distance between the evader
	//and the pursuer; and is inversely proportional to the sum of the
	//agent's velocities
	double LookAheadTime = ToEvader.Length() / 
		(m_pMovingEntity->MaxSpeed() + evader->Speed());

	//now seek to the predicted future position of the evader
	return Seek(evader->Pos() + evader->Velocity() * LookAheadTime);
}

//--------------------------- Wander -------------------------------------
//
//  This behavior makes the agent wander about randomly
//------------------------------------------------------------------------
Vector2D SteeringBehavior::Wander(float dt)
{ 
	//this behavior is dependent on the update rate, so this line must
	//be included when using time independent framerate.
	double JitterThisTimeSlice = WanderJitterPerSec * dt;

	//first, add a small random vector to the target's position
	m_vWanderTarget += Vector2D(RandomClamped() * JitterThisTimeSlice,
		RandomClamped() * JitterThisTimeSlice);

	//reproject this new vector back on to a unit circle
	m_vWanderTarget.Normalize();

	//increase the length of the vector to the same as the radius
	//of the wander circle
	m_vWanderTarget *= WanderRad;

	//move the target into a position WanderDist in front of the agent
	Vector2D target = m_vWanderTarget + Vector2D(WanderDist, 0);

	//project the target into world space
	Vector2D Target = PointToWorldSpace(target,
		m_pMovingEntity->Heading(),
		m_pMovingEntity->Pos());

	//and steer towards it
	Vector2D Origin = m_pMovingEntity->Pos();
	Vector2D Ret = Target - Origin;

	return Ret;
}


//---------------------- ObstacleAvoidance -------------------------------
//
//  Given a vector of CObstacles, this method returns a steering force
//  that will prevent the agent colliding with the closest obstacle
//------------------------------------------------------------------------
Vector2D SteeringBehavior::ObstacleAvoidance()
{
	//the detection box length is proportional to the agent's velocity
	float realBoxLength = m_dDBoxLength /* + 
										(m_pMovingEntity->Speed()) * m_dDBoxLength */;

	//this will keep track of the closest intersecting obstacle (CIB)
	BaseGameEntity* ClosestIntersectingObstacle = NULL;

	//this will be used to track the distance to the CIB
	double DistToClosestIP = MaxDouble;

	//this will record the transformed local coordinates of the CIB
	Vector2D LocalPosOfClosestObstacle;

	std::set<Obstacle*>::const_iterator curOb = Obstacle::getAll().begin(),
		endOb = Obstacle::getAll().end();

	while(curOb != endOb)
	{

		Obstacle* obst = (*curOb);

		Vector2D to = obst->Pos() - m_pMovingEntity->Pos();

		//the bounding radius of the other is taken into account by adding it 
		//to the range
		double range = realBoxLength + obst->BRadius();

		//if entity within range, tag for further consideration. (working in
		//distance-squared space to avoid sqrts)
		if ((to.LengthSq() < range*range))
		{

			//calculate this obstacle's position in local space
			Vector2D LocalPos = PointToLocalSpace(obst->Pos(),
				m_pMovingEntity->Heading(),
				m_pMovingEntity->Pos());

			//if the local position has a negative x value then it must lay
			//behind the agent. (in which case it can be ignored)
			if (LocalPos.x >= 0)
			{
				//if the distance from the x axis to the object's position is less
				//than its radius + half the width of the detection box then there
				//is a potential intersection.
				double ExpandedRadius = obst->BRadius() + m_pMovingEntity->BRadius();

				/*if (fabs(LocalPos.y) < ExpandedRadius)
				{*/
				//now to do a line/circle intersection test. The center of the 
				//circle is represented by (cX, cY). The intersection points are 
				//given by the formula x = cX +/-sqrt(r^2-cY^2) for y=0. 
				//We only need to look at the smallest positive value of x because
				//that will be the closest point of intersection.
				double cX = LocalPos.x;
				double cY = LocalPos.y;

				//we only need to calculate the sqrt part of the above equation once
				double SqrtPart = sqrt(ExpandedRadius*ExpandedRadius - cY*cY);

				double ip = cX - SqrtPart;

				if (ip <= 0.0)
				{
					ip = cX + SqrtPart;
				}

				//test to see if this is the closest so far. If it is keep a
				//record of the obstacle and its local coordinates
				if (ip < DistToClosestIP)
				{
					DistToClosestIP = ip;

					ClosestIntersectingObstacle = obst;

					LocalPosOfClosestObstacle = LocalPos;
				}         
			}
			//}
		}

		++curOb;
	}

	//if we have found an intersecting obstacle, calculate a steering 
	//force away from it
	Vector2D SteeringForce;

	if (ClosestIntersectingObstacle)
	{
		//the closer the agent is to an object, the stronger the 
		//steering force should be
		float multiplier = 1.0 + (m_dDBoxLength - LocalPosOfClosestObstacle.x) /
			m_dDBoxLength;

		//calculate the lateral force
		SteeringForce.y = (ClosestIntersectingObstacle->BRadius()-
			LocalPosOfClosestObstacle.y)  * multiplier;   

		//apply a braking force proportional to the obstacles distance from
		//the MovingEntity. 
		const float BrakingWeight = 0.2f;

		SteeringForce.x = (ClosestIntersectingObstacle->BRadius() - 
			LocalPosOfClosestObstacle.x) * 
			BrakingWeight;
	}

	//finally, convert the steering vector from local to world space
	return VectorToWorldSpace(SteeringForce,
		m_pMovingEntity->Heading());
}


//--------------------------- BoundsAvoidance --------------------------------
//
//  This returns a steering force that will keep the agent in an area
//------------------------------------------------------------------------
Vector2D SteeringBehavior::BoundsAvoidance()
{

	std::vector<Vector2D> m_Feelers(3);
	const float m_dWallDetectionFeelerLength = 20;

	//feeler pointing straight in front
	m_Feelers[0] = m_pMovingEntity->Pos() + m_dWallDetectionFeelerLength * m_pMovingEntity->Heading();

	//feeler to left
	Vector2D temp = m_pMovingEntity->Heading();
	Vec2DRotateAroundOrigin(temp, HalfPi * 3.5f);
	m_Feelers[1] = m_pMovingEntity->Pos() + m_dWallDetectionFeelerLength/2.0f * temp;

	//feeler to right
	temp = m_pMovingEntity->Heading();
	Vec2DRotateAroundOrigin(temp, HalfPi * 0.5f);
	m_Feelers[2] = m_pMovingEntity->Pos() + m_dWallDetectionFeelerLength/2.0f * temp;

	float DistToThisIP    = 0.0;
	float DistToClosestIP = MaxDouble;




	Vector2D wallsv[5] = {Vector2D(m_bounds.left, m_bounds.top),
		Vector2D(m_bounds.left, m_bounds.top+m_bounds.height),
		Vector2D(m_bounds.left+m_bounds.width, m_bounds.top+m_bounds.height),
		Vector2D(m_bounds.left+m_bounds.width, m_bounds.top),
		Vector2D(m_bounds.left, m_bounds.top)
	};


	//this will hold an index into the vector of walls
	int ClosestWall = -1;

	Vector2D SteeringForce,
		point,         //used for storing temporary info
		ClosestPoint;  //holds the closest intersection point

	//examine each feeler in turn
	for (unsigned int flr=0; flr<m_Feelers.size(); ++flr) {

		//run through each wall checking for any intersection points
		for (int i = 0; i < 4; i++) {

			if (LineIntersection2D(m_pMovingEntity->Pos(),
				m_Feelers[flr],
				wallsv[i],
				wallsv[i+1],
				DistToThisIP,
				point))
			{
				//is this the closest found so far? If so keep a record
				if (DistToThisIP < DistToClosestIP)
				{
					DistToClosestIP = DistToThisIP;

					ClosestWall = i;

					ClosestPoint = point;
				}
			}
		}//next wall


		//if an intersection point has been detected, calculate a force  
		//that will direct the agent away
		if (ClosestWall != -1)
		{
			//calculate by what distance the projected position of the agent
			//will overshoot the wall
			Vector2D OverShoot = m_Feelers[flr] - ClosestPoint;

			Vector2D temp = Vec2DNormalize(wallsv[ClosestWall] - wallsv[ClosestWall+1]);
			Vector2D normal (-temp.y,temp.x);

			//create a force in the direction of the wall normal, with a 
			//magnitude of the overshoot
			SteeringForce = normal * OverShoot.Length();
		}

	}//next feeler

	return SteeringForce;
}




//--------------------------- Hide ---------------------------------------
//
//------------------------------------------------------------------------
Vector2D SteeringBehavior::Hide()
{
	double    DistToClosest = MaxDouble;
	Vector2D BestHidingSpot;

	std::set<Obstacle*>::const_iterator curOb = Obstacle::getAll().begin();
	std::set<Obstacle*>::const_iterator closest;

	while(curOb != Obstacle::getAll().end())
	{
		//calculate the position of the hiding spot for this obstacle
		Vector2D HidingSpot = GetHidingPosition((*curOb)->Pos(),
			(*curOb)->BRadius(),
			hideTarget->Pos());

		//work in distance-squared space to find the closest hiding
		//spot to the agent
		double dist = Vec2DDistanceSq(HidingSpot, m_pMovingEntity->Pos());

		if (dist < DistToClosest)
		{
			DistToClosest = dist;

			BestHidingSpot = HidingSpot;

			closest = curOb;
		}  

		++curOb;

	}//end while

	//if no suitable obstacles found then Evade the hunter
	if (DistToClosest == MaxFloat)
	{
		return Flee(hideTarget->Pos());
	}

	//else use Arrive on the hiding spot
	return Arrive(BestHidingSpot, fast);
}

//------------------------- GetHidingPosition ----------------------------
//
//  Given the position of a hunter, and the position and radius of
//  an obstacle, this method calculates a position DistanceFromBoundary 
//  away from its bounding radius and directly opposite the hunter
//------------------------------------------------------------------------
Vector2D SteeringBehavior::GetHidingPosition(const Vector2D& posOb,
	const double     radiusOb,
	const Vector2D& posHunter)
{
	//calculate how far away the agent is to be from the chosen obstacle's
	//bounding radius
	const double DistanceFromBoundary = 30.0;
	double       DistAway    = radiusOb + DistanceFromBoundary;

	//calculate the heading toward the object from the hunter
	Vector2D ToOb = Vec2DNormalize(posOb - posHunter);

	//scale it to size and add to the obstacles position to get
	//the hiding spot.
	return (ToOb * DistAway) + posOb;
}

/*
//------------------------------- FollowPath -----------------------------
//
//  Given a series of Vector2Ds, this method produces a force that will
//  move the agent along the waypoints in order. The agent uses the
// 'Seek' behavior to move to the next waypoint - unless it is the last
//  waypoint, in which case it 'Arrives'
//------------------------------------------------------------------------
Vector2D SteeringBehavior::FollowPath()
{ 
//move to next target if close enough to current target (working in
//distance squared space)
if(Vec2DDistanceSq(m_pPath->CurrentWaypoint(), m_pMovingEntity->Pos()) <
m_dWaypointSeekDistSq)
{
m_pPath->SetNextWaypoint();
}

if (!m_pPath->Finished())
{
return Seek(m_pPath->CurrentWaypoint());
}

else
{
return Arrive(m_pPath->CurrentWaypoint(), normal);
}
}
*/

//------------------------- Offset Pursuit -------------------------------
//
//  Produces a steering force that keeps a MovingEntity at a specified offset
//  from a leader MovingEntity
//------------------------------------------------------------------------
Vector2D SteeringBehavior::OffsetPursuit(const MovingEntity*  leader,
	const float offset)
{
	Vector2D displacement = leader->Heading() * - offset;

	//calculate the offset's position in world space
	Vector2D WorldOffsetPos = PointToWorldSpace(displacement,
		leader->Heading(),
		leader->Pos());

	Vector2D ToOffset = WorldOffsetPos - m_pMovingEntity->Pos();

	//the lookahead time is propotional to the distance between the leader
	//and the pursuer; and is inversely proportional to the sum of both
	//agent's velocities
	double LookAheadTime = ToOffset.Length() / 
		(m_pMovingEntity->MaxSpeed() + leader->Speed());

	//now Arrive at the predicted future position of the offset
	return Arrive(WorldOffsetPos + leader->Velocity() * LookAheadTime, fast);
}






