#pragma once

#include "constants.h"

#include <vector>
#include <string>
#include <list>

#include "math/Vector2D.h"

#include <SFML/Graphics.hpp>

class MovingEntity;
class BaseGameEntity;


//------------------------------------------------------------------------

class SteeringBehavior
{
public:
  
  enum summing_method{weighted_average, prioritized};

private:

  enum behavior_type
  {
    none               = 0,
    seek               = 1,
    flee               = 2,
    arrive             = 4,
    wander             = 8,
    obstacle_avoidance = 16,
    bounds_avoidance   = 32,
    pursuit            = 64,
    hide               = 128,
    flock              = 256,
    offset_pursuit     = 512,
	forward		       = 1024
  };

private:

  
  //a pointer to the owner of this instance
  MovingEntity*     m_pMovingEntity;   

  //For bounds avoidance
  sf::Rect<float> m_bounds;
    
  //the steering force created by the combined effect of all
  //the selected behaviors
  Vector2D    m_vSteeringForce;
 
  MovingEntity* pursuitTarget;
  BaseGameEntity*  seekTarget;
  BaseGameEntity*  fleeTarget;
  BaseGameEntity*  hideTarget;
  
  //any offset used for formations or offset pursuit
  float     m_vOffset;


  //length of the 'detection box' utilized in obstacle avoidance
  float                 m_dDBoxLength;
  
  //the current position on the wander circle the agent is
  //attempting to steer towards
  Vector2D     m_vWanderTarget; 


  //multipliers. These can be adjusted to effect strength of the  
  //appropriate behavior. Useful to get flocking the way you require
  //for example.
  double        m_dWeightWander;
  double        m_dWeightObstacleAvoidance;
  double        m_dWeightBoundsAvoidance;
  double        m_dWeightSeek;
  double        m_dWeightForward;
  double        m_dWeightFlee;
  double        m_dWeightArrive;
  double        m_dWeightPursuit;
  double        m_dWeightOffsetPursuit;
  double        m_dWeightInterpose;
  double        m_dWeightHide;


  //binary flags to indicate whether or not a behavior should be active
  int           m_iFlags;

  
  //Arrive makes use of these to determine how quickly a MovingEntity
  //should decelerate to its target
  enum Deceleration{slow = 3, normal = 2, fast = 1};

  //default
  Deceleration m_Deceleration;

  //what type of method is used to sum any active behavior
  summing_method m_SummingMethod;


  //this function tests if a specific bit of m_iFlags is set
  bool On(behavior_type bt){return (m_iFlags & bt) == bt;}

  bool AccumulateForce(Vector2D &sf, Vector2D ForceToAdd);




   /* .......................................................

                    BEGIN BEHAVIOR DECLARATIONS

      .......................................................*/
  
  Vector2D Forward();

  //this behavior moves the agent towards a target position
  Vector2D Seek(Vector2D TargetPos);

  //this behavior returns a vector that moves the agent away
  //from a target position
  Vector2D Flee(Vector2D TargetPos);

  //this behavior is similar to seek but it attempts to arrive 
  //at the target position with a zero velocity
  Vector2D Arrive(Vector2D     TargetPos,
                  Deceleration deceleration);

  //this behavior predicts where an agent will be in time T and seeks
  //towards that point to intercept it.
  Vector2D Pursuit(const MovingEntity* evader);

  //this behavior maintains a position, in the direction of offset
  //from the target MovingEntity
  Vector2D OffsetPursuit(const MovingEntity* leader, const float offset);
  
  //this behavior makes the agent wander about randomly
  Vector2D Wander(float dt);

  //this returns a steering force which will attempt to keep the agent 
  //away from any obstacles it may encounter
  Vector2D ObstacleAvoidance();

  //this returns a steering force which will keep the agent in its bounds
  Vector2D BoundsAvoidance();

  //given another agent position to hide from and a list of BaseGameEntitys this
  //method attempts to put an obstacle between itself and its opponent
  Vector2D Hide();

  /* .......................................................

                       END BEHAVIOR DECLARATIONS

      .......................................................*/

  //calculates and sums the steering forces from any active behaviors
  Vector2D CalculateWeightedSum(float dt);
  Vector2D CalculatePrioritized(float dt);

  //helper method for Hide. Returns a position located on the other
  //side of an obstacle to the pursuer
  Vector2D GetHidingPosition(const Vector2D& posOb,
                              const double     radiusOb,
                              const Vector2D& posHunter);



  
  
public:

  SteeringBehavior(MovingEntity* agent);

  virtual ~SteeringBehavior();

  //calculates and sums the steering forces from any active behaviors
  Vector2D Calculate(float dt);

  Vector2D Force()const{return m_vSteeringForce;}

  void SetSummingMethod(summing_method sm){m_SummingMethod = sm;}

  
  void ForwardOn(){ m_iFlags |= forward;}
  void FleeOn(BaseGameEntity* target){fleeTarget = target; m_iFlags |= flee;}
  void SeekOn(BaseGameEntity* target){seekTarget = target; m_iFlags |= seek;}
  void ArriveOn(BaseGameEntity* target){seekTarget = target; m_iFlags |= arrive;}
  void WanderOn(){m_iFlags |= wander;}
  void PursuitOn(MovingEntity* v){m_iFlags |= pursuit; pursuitTarget = v;}
  void ObstacleAvoidanceOn(){m_iFlags |= obstacle_avoidance;}
  void BoundsAvoidanceOn(sf::Rect<float> bounds = sf::Rect<float>(50,50,700,500)){m_bounds = bounds; m_iFlags |= bounds_avoidance;}
  void HideOn(BaseGameEntity* v){m_iFlags |= hide; hideTarget = v;}
  void OffsetPursuitOn(MovingEntity* v1, const float offset){m_iFlags |= offset_pursuit; m_vOffset = offset; pursuitTarget = v1;}  

  void ForwardOff()  {if(On(forward))   m_iFlags ^=forward;}
  void FleeOff()  {if(On(flee))   m_iFlags ^=flee;}
  void SeekOff()  {if(On(seek))   m_iFlags ^=seek;}
  void ArriveOff(){if(On(arrive)) m_iFlags ^=arrive;}
  void WanderOff(){if(On(wander)) m_iFlags ^=wander;}
  void PursuitOff(){if(On(pursuit)) m_iFlags ^=pursuit;}
  void ObstacleAvoidanceOff(){if(On(obstacle_avoidance)) m_iFlags ^=obstacle_avoidance;}
  void BoundsAvoidanceOff(){if(On(bounds_avoidance)) m_iFlags ^=bounds_avoidance;}
  void HideOff(){if(On(hide)) m_iFlags ^=hide;}
  void OffsetPursuitOff(){if(On(offset_pursuit)) m_iFlags ^=offset_pursuit;}

  bool isFleeOn(){return On(flee);}
  bool isSeekOn(){return On(seek);}
  bool isArriveOn(){return On(arrive);}
  bool isWanderOn(){return On(wander);}
  bool isPursuitOn(){return On(pursuit);}
  bool isObstacleAvoidanceOn(){return On(obstacle_avoidance);}
  bool isBoundsAvoidanceOn(){return On(bounds_avoidance);}
  bool isHideOn(){return On(hide);}
  bool isOffsetPursuitOn(){return On(offset_pursuit);}

  double DBoxLength()const{return m_dDBoxLength;}
  


};


