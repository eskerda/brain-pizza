#pragma once

#include "constants.h"

#include "math/utils.h"
#include "math/Vector2D.h"
#include "math/C2DMatrix.h"
#include "math/transformations.h"
#include "BaseGameEntity.h"

#include <math.h>
#include <vector>
#include <set>
#include <list>

template <class conT>
inline typename conT::value_type FindClosest(Vector2D pos, const conT& t, float* out_distance = NULL)
{
    if (t.empty()) {
		if (out_distance) *out_distance = MaxFloat;
		return NULL;
	}

    typename conT::value_type closest = (*t.begin());
	float closestDist = pos.DistanceSq(closest->Pos());

    for (typename conT::iterator it = t.begin(); it != t.end(); it++) {

        typename conT::value_type candidate = *it;
		float candidateDist = pos.DistanceSq(candidate->Pos());

		if (closestDist > candidateDist) {
			closest = *it;
			closestDist = candidateDist;
		}

	}

	if (out_distance) *out_distance = sqrt(closestDist);

	return closest;
}



//////////////////////////////////////////////////////////////////////////
//
//  Some useful template functions
//
//////////////////////////////////////////////////////////////////////////

//------------------------- Overlapped -----------------------------------
//
//  tests to see if an entity is overlapping any of a number of entities
//  stored in a std container
//------------------------------------------------------------------------
template <class T, class conT>
bool Overlapped(const T* ob, const conT& conOb, double MinDistBetweenObstacles = 40.0)
{
  typename conT::const_iterator it;

  for (it=conOb.begin(); it != conOb.end(); ++it)
  {
    if (TwoCirclesOverlapped(ob->Pos(),
                             ob->BRadius()+MinDistBetweenObstacles,                             
                             (*it)->Pos(),
                             (*it)->BRadius()))
    {
      return true;
    }
  }

  return false;
}

//------------------- EnforceNonPenetrationConstraint ---------------------
//
//  Given a pointer to an entity and a std container of pointers to nearby
//  entities, this function checks to see if there is an overlap between
//  entities. If there is, then the entities are moved away from each
//  other
//------------------------------------------------------------------------
template <class T, class conT>
void EnforceNonPenetrationConstraint(const T&    entity, 
                                    const conT& ContainerOfEntities)
{
  //iterate through all entities checking for any overlap of bounding radii
  for (typename conT::const_iterator curEntity =ContainerOfEntities.begin();
       curEntity != ContainerOfEntities.end();
       ++curEntity)
  {
    //make sure we don't check against the individual
    if (*curEntity == entity) continue;

    //calculate the distance between the positions of the entities
    Vector2D ToEntity = entity->Pos() - (*curEntity)->Pos();

    double DistFromEachOther = ToEntity.Length();

    //if this distance is smaller than the sum of their radii then this
    //entity must be moved away in the direction parallel to the
    //ToEntity vector   
    double AmountOfOverLap = (*curEntity)->BRadius() + entity->BRadius() -
                             DistFromEachOther;

    if (AmountOfOverLap >= 0)
    {
      //move the entity a distance away equivalent to the amount of overlap.
      entity->SetPos(entity->Pos() + (ToEntity/DistFromEachOther) *
                     AmountOfOverLap);
    }
  }//next entity
}




//-------------------- GetEntityLineSegmentIntersections ----------------------
//
//  tests a line segment AB against a container of entities. First of all
//  a test is made to confirm that the entity is within a specified range of 
//  the one_to_ignore (positioned at A). If within range the intersection test
//  is made.
//
//  returns a list of all the entities that tested positive for intersection
//-----------------------------------------------------------------------------
template <class T, class conT>
std::list<T> GetEntityLineSegmentIntersections(const conT& entities,
                                               int         the_one_to_ignore,
                                               Vector2D    A,
                                               Vector2D    B,
                                               double       range = MaxDouble)
{
  typename conT::const_iterator it;

  std::list<T> hits;

  //iterate through all entities checking against the line segment AB
  for (it=entities.begin(); it != entities.end(); ++it)
  {
    //if not within range or the entity being checked is the_one_to_ignore
    //just continue with the next entity
    if ( ((*it)->ID() == the_one_to_ignore) ||
         (Vec2DDistanceSq((*it)->Pos(), A) > range*range) )
    {
      continue;
    }

    //if the distance to AB is less than the entities bounding radius then
    //there is an intersection so add it to hits
    if (DistToLineSegment(A, B, (*it)->Pos()) < (*it)->BRadius())
    {
      hits.push_back(*it);
    }

  }

  return hits;
}

//------------------------ GetClosestEntityLineSegmentIntersection ------------
//
//  tests a line segment AB against a container of entities. First of all
//  a test is made to confirm that the entity is within a specified range of 
//  the one_to_ignore (positioned at A). If within range the intersection test
//  is made.
//
//  returns the closest entity that tested positive for intersection or NULL
//  if none found
//-----------------------------------------------------------------------------

template <class T, class conT>
T* GetClosestEntityLineSegmentIntersection(const conT& entities,
                                          int         the_one_to_ignore,
                                          Vector2D    A,
                                          Vector2D    B,
                                          double       range = MaxDouble)
{
  typename conT::const_iterator it;

  T* ClosestEntity = NULL;

  double ClosestDist = MaxDouble;

  //iterate through all entities checking against the line segment AB
  for (it=entities.begin(); it != entities.end(); ++it)
  {
    double distSq = Vec2DDistanceSq((*it)->Pos(), A);

    //if not within range or the entity being checked is the_one_to_ignore
    //just continue with the next entity
    if ( ((*it)->ID() == the_one_to_ignore) || (distSq > range*range) )
    {
      continue;
    }

    //if the distance to AB is less than the entities bounding radius then
    //there is an intersection so add it to hits
    if (DistToLineSegment(A, B, (*it)->Pos()) < (*it)->BRadius())
    {
      if (distSq < ClosestDist)
      {
        ClosestDist = distSq;

        ClosestEntity = *it;
      }
    }

  }

  return ClosestEntity;
}


//given a plane and a ray this function determins how far along the ray 
//an interestion occurs. Returns negative if the ray is parallel
inline float DistanceToRayPlaneIntersection(Vector2D RayOrigin,
                                             Vector2D RayHeading,
                                             Vector2D PlanePoint,  //any point on the plane
                                             Vector2D PlaneNormal)
{
  
  float d     = - PlaneNormal.Dot(PlanePoint);
  float numer = PlaneNormal.Dot(RayOrigin) + d;
  float denom = PlaneNormal.Dot(RayHeading);
  
  // normal is parallel to vector
  if ((denom < 0.000001) && (denom > -0.000001))
  {
   return (-1.0);
  }

  return -(numer / denom);	
}

//------------------------- WhereIsPoint --------------------------------------
enum span_type{plane_backside, plane_front, on_plane};
inline span_type WhereIsPoint(Vector2D point,
                              Vector2D PointOnPlane, //any point on the plane
                              Vector2D PlaneNormal) 
{
 Vector2D dir = PointOnPlane - point;

 float d = dir.Dot(PlaneNormal);
 
 if (d<-0.000001)
 {
  return plane_front;	
 }

 else if (d>0.000001)
 {
  return plane_backside;	
 }

  return on_plane;	
}


const float pi = 3.14159f;
//-------------------------- GetRayCircleIntersec -----------------------------
inline float GetRayCircleIntersect(Vector2D RayOrigin,
                                    Vector2D RayHeading,
                                    Vector2D CircleOrigin,
                                    float  radius)
{
	
   Vector2D ToCircle = CircleOrigin-RayOrigin;
   float length      = ToCircle.Length();
   float v           = ToCircle.Dot(RayHeading);
   float d           = radius*radius - (length*length - v*v);

   // If there was no intersection, return -1
   if (d < 0.0) return (-1.0);

   // Return the distance to the [first] intersecting point
   return (v - sqrt(d));
}

//----------------------------- DoRayCircleIntersect --------------------------
inline bool DoRayCircleIntersect(Vector2D RayOrigin,
                                 Vector2D RayHeading,
                                 Vector2D CircleOrigin,
                                 float     radius)
{
	
   Vector2D ToCircle = CircleOrigin-RayOrigin;
   float length      = ToCircle.Length();
   float v           = ToCircle.Dot(RayHeading);
   float d           = radius*radius - (length*length - v*v);

   // If there was no intersection, return -1
   return (d < 0.0);
}


//------------------------------------------------------------------------
//  Given a point P and a circle of radius R centered at C this function
//  determines the two points on the circle that intersect with the 
//  tangents from P to the circle. Returns false if P is within the circle.
//
//  thanks to Dave Eberly for this one.
//------------------------------------------------------------------------
inline bool GetTangentPoints (Vector2D C, float R, Vector2D P, Vector2D& T1, Vector2D& T2)
{
  Vector2D PmC = P - C;
  float SqrLen = PmC.LengthSq();
  float RSqr = R*R;
  if ( SqrLen <= RSqr )
  {
      // P is inside or on the circle
      return false;
  }

  float InvSqrLen = 1/SqrLen;
  float Root = sqrt(fabs(SqrLen - RSqr));

  T1.x = C.x + R*(R*PmC.x - PmC.y*Root)*InvSqrLen;
  T1.y = C.y + R*(R*PmC.y + PmC.x*Root)*InvSqrLen;
  T2.x = C.x + R*(R*PmC.x + PmC.y*Root)*InvSqrLen;
  T2.y = C.y + R*(R*PmC.y - PmC.x*Root)*InvSqrLen;

  return true;
}




//------------------------- DistToLineSegment ----------------------------
//
//  given a line segment AB and a point P, this function calculates the 
//  perpendicular distance between them
//------------------------------------------------------------------------
inline float DistToLineSegment(Vector2D A,
                                Vector2D B,
                                Vector2D P)
{
  //if the angle is obtuse between PA and AB is obtuse then the closest
  //vertex must be A
  float dotA = (P.x - A.x)*(B.x - A.x) + (P.y - A.y)*(B.y - A.y);

  if (dotA <= 0) return Vec2DDistance(A, P);

  //if the angle is obtuse between PB and AB is obtuse then the closest
  //vertex must be B
  float dotB = (P.x - B.x)*(A.x - B.x) + (P.y - B.y)*(A.y - B.y);
 
  if (dotB <= 0) return Vec2DDistance(B, P);
    
  //calculate the point along AB that is the closest to P
  Vector2D Point = A + ((B - A) * dotA)/(dotA + dotB);

  //calculate the distance P-Point
  return Vec2DDistance(P,Point);
}

//------------------------- DistToLineSegmentSq ----------------------------
//
//  as above, but avoiding sqrt
//------------------------------------------------------------------------
inline float DistToLineSegmentSq(Vector2D A,
                                 Vector2D B,
                                 Vector2D P)
{
  //if the angle is obtuse between PA and AB is obtuse then the closest
  //vertex must be A
  float dotA = (P.x - A.x)*(B.x - A.x) + (P.y - A.y)*(B.y - A.y);

  if (dotA <= 0) return Vec2DDistanceSq(A, P);

  //if the angle is obtuse between PB and AB is obtuse then the closest
  //vertex must be B
  float dotB = (P.x - B.x)*(A.x - B.x) + (P.y - B.y)*(A.y - B.y);
 
  if (dotB <= 0) return Vec2DDistanceSq(B, P);
    
  //calculate the point along AB that is the closest to P
  Vector2D Point = A + ((B - A) * dotA)/(dotA + dotB);

  //calculate the distance P-Point
  return Vec2DDistanceSq(P,Point);
}


//--------------------LineIntersection2D-------------------------
//
//	Given 2 lines in 2D space AB, CD this returns true if an 
//	intersection occurs.
//
//----------------------------------------------------------------- 

inline bool LineIntersection2D(Vector2D A,
                               Vector2D B,
                               Vector2D C, 
                               Vector2D D)
{
  float rTop = (A.y-C.y)*(D.x-C.x)-(A.x-C.x)*(D.y-C.y);
  float sTop = (A.y-C.y)*(B.x-A.x)-(A.x-C.x)*(B.y-A.y);

	float Bot = (B.x-A.x)*(D.y-C.y)-(B.y-A.y)*(D.x-C.x);

  if (Bot == 0)//parallel
  {
    return false;
  }

  float invBot = 1.0/Bot;
	float r = rTop * invBot;
	float s = sTop * invBot;

	if( (r > 0) && (r < 1) && (s > 0) && (s < 1) )
  {
    //lines intersect
    return true;
  }

  //lines do not intersect
  return false;
}

//--------------------LineIntersection2D-------------------------
//
//	Given 2 lines in 2D space AB, CD this returns true if an 
//	intersection occurs and sets dist to the distance the intersection
//  occurs along AB
//
//----------------------------------------------------------------- 

inline bool LineIntersection2D(Vector2D A,
                        Vector2D B,
                        Vector2D C, 
                        Vector2D D,
                        float &dist)
{

  float rTop = (A.y-C.y)*(D.x-C.x)-(A.x-C.x)*(D.y-C.y);
  float sTop = (A.y-C.y)*(B.x-A.x)-(A.x-C.x)*(B.y-A.y);

	float Bot = (B.x-A.x)*(D.y-C.y)-(B.y-A.y)*(D.x-C.x);


  if (Bot == 0)//parallel
  {
    if (isEqual(rTop, 0) && isEqual(sTop, 0))
    {
      return true;
    }
    return false;
  }

	float r = rTop/Bot;
	float s = sTop/Bot;

	if( (r > 0) && (r < 1) && (s > 0) && (s < 1) )
  {
  	dist = Vec2DDistance(A,B) * r;

    return true;
  }

	else
  {
		dist = 0;

    return false;
  }
}

//-------------------- LineIntersection2D-------------------------
//
//	Given 2 lines in 2D space AB, CD this returns true if an 
//	intersection occurs and sets dist to the distance the intersection
//  occurs along AB. Also sets the 2d vector point to the point of
//  intersection
//----------------------------------------------------------------- 
inline bool LineIntersection2D(Vector2D   A,
                               Vector2D   B,
                               Vector2D   C, 
                               Vector2D   D,
                               float&     dist,
                               Vector2D&  point)
{

  float rTop = (A.y-C.y)*(D.x-C.x)-(A.x-C.x)*(D.y-C.y);
	float rBot = (B.x-A.x)*(D.y-C.y)-(B.y-A.y)*(D.x-C.x);

	float sTop = (A.y-C.y)*(B.x-A.x)-(A.x-C.x)*(B.y-A.y);
	float sBot = (B.x-A.x)*(D.y-C.y)-(B.y-A.y)*(D.x-C.x);

	if ( (rBot == 0) || (sBot == 0))
	{
		//lines are parallel
		return false;
	}

	float r = rTop/rBot;
	float s = sTop/sBot;

	if( (r > 0) && (r < 1) && (s > 0) && (s < 1) )
  {
  	dist = Vec2DDistance(A,B) * r;

    point = A + r * (B - A);

    return true;
  }

	else
  {
		dist = 0;

    return false;
  }
}

//----------------------- ObjectIntersection2D ---------------------------
//
//  tests two polygons for intersection. *Does not check for enclosure*
//------------------------------------------------------------------------
inline bool ObjectIntersection2D(const std::vector<Vector2D>& object1,
                                 const std::vector<Vector2D>& object2)
{
  //test each line segment of object1 against each segment of object2
  for (unsigned int r=0; r<object1.size()-1; ++r)
  {
    for (unsigned int t=0; t<object2.size()-1; ++t)
    {
      if (LineIntersection2D(object2[t],
                             object2[t+1],
                             object1[r],
                             object1[r+1]))
      {
        return true;
      }
    }
  }

  return false;
}

//----------------------- SegmentObjectIntersection2D --------------------
//
//  tests a line segment against a polygon for intersection
//  *Does not check for enclosure*
//------------------------------------------------------------------------
inline bool SegmentObjectIntersection2D(const Vector2D& A,
                                 const Vector2D& B,
                                 const std::vector<Vector2D>& object)
{
  //test AB against each segment of object
  for (unsigned int r=0; r<object.size()-1; ++r)
  {
    if (LineIntersection2D(A, B, object[r], object[r+1]))
    {
      return true;
    }
  }

  return false;
}


//----------------------------- TwoCirclesOverlapped ---------------------
//
//  Returns true if the two circles overlap
//------------------------------------------------------------------------
inline bool TwoCirclesOverlapped(float x1, float y1, float r1,
                          float x2, float y2, float r2)
{
  float DistBetweenCenters = sqrt( (x1-x2) * (x1-x2) +
                                    (y1-y2) * (y1-y2));

  if ((DistBetweenCenters < (r1+r2)) || (DistBetweenCenters < fabs(r1-r2)))
  {
    return true;
  }

  return false;
}

//----------------------------- TwoCirclesOverlapped ---------------------
//
//  Returns true if the two circles overlap
//------------------------------------------------------------------------
inline bool TwoCirclesOverlapped(Vector2D c1, float r1,
                          Vector2D c2, float r2)
{
  float DistBetweenCenters = sqrt( (c1.x-c2.x) * (c1.x-c2.x) +
                                    (c1.y-c2.y) * (c1.y-c2.y));

  if ((DistBetweenCenters < (r1+r2)) || (DistBetweenCenters < fabs(r1-r2)))
  {
    return true;
  }

  return false;
}

//--------------------------- TwoCirclesEnclosed ---------------------------
//
//  returns true if one circle encloses the other
//-------------------------------------------------------------------------
inline bool TwoCirclesEnclosed(float x1, float y1, float r1,
                        float x2, float y2, float r2)
{
  float DistBetweenCenters = sqrt( (x1-x2) * (x1-x2) +
                                    (y1-y2) * (y1-y2));

  if (DistBetweenCenters < fabs(r1-r2))
  {
    return true;
  }

  return false;
}

//------------------------ TwoCirclesIntersectionPoints ------------------
//
//  Given two circles this function calculates the intersection points
//  of any overlap.
//
//  returns false if no overlap found
//
// see http://astronomy.swin.edu.au/~pbourke/geometry/2circle/
//------------------------------------------------------------------------ 
inline bool TwoCirclesIntersectionPoints(float x1, float y1, float r1,
                                  float x2, float y2, float r2,
                                  float &p3X, float &p3Y,
                                  float &p4X, float &p4Y)
{
  //first check to see if they overlap
  if (!TwoCirclesOverlapped(x1,y1,r1,x2,y2,r2))
  {
    return false;
  }

  //calculate the distance between the circle centers
  float d = sqrt( (x1-x2) * (x1-x2) + (y1-y2) * (y1-y2));
  
  //Now calculate the distance from the center of each circle to the center
  //of the line which connects the intersection points.
  float a = (r1 - r2 + (d * d)) / (2 * d);
  //float b = (r2 - r1 + (d * d)) / (2 * d);
  

  //MAYBE A TEST FOR EXACT OVERLAP? 

  //calculate the point P2 which is the center of the line which 
  //connects the intersection points
  float p2X, p2Y;

  p2X = x1 + a * (x2 - x1) / d;
  p2Y = y1 + a * (y2 - y1) / d;

  //calculate first point
  float h1 = sqrt((r1 * r1) - (a * a));

  p3X = p2X - h1 * (y2 - y1) / d;
  p3Y = p2Y + h1 * (x2 - x1) / d;


  //calculate second point
  float h2 = sqrt((r2 * r2) - (a * a));

  p4X = p2X + h2 * (y2 - y1) / d;
  p4Y = p2Y - h2 * (x2 - x1) / d;

  return true;

}

//------------------------ TwoCirclesIntersectionArea --------------------
//
//  Tests to see if two circles overlap and if so calculates the area
//  defined by the union
//
// see http://mathforum.org/library/drmath/view/54785.html
//-----------------------------------------------------------------------
inline float TwoCirclesIntersectionArea(float x1, float y1, float r1,
                                  float x2, float y2, float r2)
{
  //first calculate the intersection points
  float iX1, iY1, iX2, iY2;

  if(!TwoCirclesIntersectionPoints(x1,y1,r1,x2,y2,r2,iX1,iY1,iX2,iY2))
  {
    return 0.0; //no overlap
  }

  //calculate the distance between the circle centers
  float d = sqrt( (x1-x2) * (x1-x2) + (y1-y2) * (y1-y2));

  //find the angles given that A and B are the two circle centers
  //and C and D are the intersection points
  float CBD = 2 * acos((r2*r2 + d*d - r1*r1) / (r2 * d * 2)); 

  float CAD = 2 * acos((r1*r1 + d*d - r2*r2) / (r1 * d * 2));


  //Then we find the segment of each of the circles cut off by the 
  //chord CD, by taking the area of the sector of the circle BCD and
  //subtracting the area of triangle BCD. Similarly we find the area
  //of the sector ACD and subtract the area of triangle ACD.

  float area = 0.5f*CBD*r2*r2 - 0.5f*r2*r2*sin(CBD) +
                0.5f*CAD*r1*r1 - 0.5f*r1*r1*sin(CAD);

  return area;
}

//-------------------------------- CircleArea ---------------------------
//
//  given the radius, calculates the area of a circle
//-----------------------------------------------------------------------
inline float CircleArea(float radius)
{
  return pi * radius * radius;
}


//----------------------- PointInCircle ----------------------------------
//
//  returns true if the point p is within the radius of the given circle
//------------------------------------------------------------------------
inline bool PointInCircle(Vector2D Pos,
						  float    radius,
                          Vector2D p)
{
  float DistFromCenterSquared = (p-Pos).LengthSq();

  if (DistFromCenterSquared < (radius*radius))
  {
    return true;
  }

  return false;
}

//--------------------- LineSegmentCircleIntersection ---------------------------
//
//  returns true if the line segemnt AB intersects with a circle at
//  position P with radius radius
//------------------------------------------------------------------------
inline bool   LineSegmentCircleIntersection(Vector2D A,
                                            Vector2D B,
                                            Vector2D P,
                                            float    radius)
{
  //first determine the distance from the center of the circle to
  //the line segment (working in distance squared space)
  float DistToLineSq = DistToLineSegmentSq(A, B, P);

  if (DistToLineSq < radius*radius)
  {
    return true;
  }

  else
  {
    return false;
  }

}

//------------------- GetLineSegmentCircleClosestIntersectionPoint ------------
//
//  given a line segment AB and a circle position and radius, this function
//  determines if there is an intersection and stores the position of the 
//  closest intersection in the reference IntersectionPoint
//
//  returns false if no intersection point is found
//-----------------------------------------------------------------------------
inline bool GetLineSegmentCircleClosestIntersectionPoint(Vector2D A,
                                                         Vector2D B,
                                                         Vector2D pos,
                                                         float    radius,
                                                         Vector2D& IntersectionPoint)
{
  Vector2D toBNorm = Vec2DNormalize(B-A);

  //move the circle into the local space defined by the vector B-A with origin
  //at A
  Vector2D LocalPos = PointToLocalSpace(pos, toBNorm, A);

  bool ipFound = false;

  //if the local position + the radius is negative then the circle lays behind
  //point A so there is no intersection possible. If the local x pos minus the 
  //radius is greater than length A-B then the circle cannot intersect the 
  //line segment
  if ( (LocalPos.x+radius >= 0) &&
     ( (LocalPos.x-radius)*(LocalPos.x-radius) <= Vec2DDistanceSq(B, A)) )
  {
     //if the distance from the x axis to the object's position is less
     //than its radius then there is a potential intersection.
     if (fabs(LocalPos.y) < radius)
     {
        //now to do a line/circle intersection test. The center of the 
        //circle is represented by A, B. The intersection points are 
        //given by the formulae x = A +/-sqrt(r^2-B^2), y=0. We only 
        //need to look at the smallest positive value of x.
        float a = LocalPos.x;
        float b = LocalPos.y;       

        float ip = a - sqrt(radius*radius - b*b);

        if (ip <= 0)
        {
          ip = a + sqrt(radius*radius - b*b);
        }

        ipFound = true;

        IntersectionPoint = A+ toBNorm*ip;
     }
   }

  return ipFound;
}



