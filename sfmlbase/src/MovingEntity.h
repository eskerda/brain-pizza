#pragma once
//------------------------------------------------------------------------
//
//  Name:   MovingEntity.h
//
//  Desc:   A base class defining an entity that moves. The entity has 
//          a local coordinate system and members for defining its
//          mass and velocity.
//
//  Author: Mat Buckland 2003 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------
#include <cassert>

#include "math/Vector2D.h"
#include "BaseGameEntity.h"
#include "helpers/selfregister.h"

class SteeringBehavior;

class MovingEntity : public BaseGameEntity, public SelfRegister<MovingEntity*>
{
protected:

  //the steering behavior class
  SteeringBehavior*     m_pSteering;

  Vector2D    m_vVelocity;

  //a normalized vector pointing in the direction the entity is heading. 
  Vector2D    m_vHeading;

  //the maximum speed this entity may travel at.
  float       m_dMaxSpeed;

  //the maximum force this entity can produce to power itself 
  //(think rockets and thrust)
  float        m_dMaxForce;

public:

  MovingEntity(Vector2D position,
                float	radius,
                float	max_speed,
                float    max_force = 10.0,
                Vector2D velocity = Vector2D(0,0),
                Vector2D heading = Vector2D(1,0) )
      : BaseGameEntity(position, radius),
              m_vHeading(heading),
              m_vVelocity(velocity),
              m_dMaxSpeed(max_speed),
              m_dMaxForce(max_force)

  {
  }

  virtual ~MovingEntity(){}

  SteeringBehavior* Steering() { return m_pSteering; }

  //accessors
  Vector2D  Velocity() const {return m_vVelocity;}
  void      SetVelocity(const Vector2D& NewVel) { m_vVelocity = NewVel;}

  float     MaxSpeed()const{return m_dMaxSpeed;}
  void      SetMaxSpeed(float new_speed){m_dMaxSpeed = new_speed;}

  float     MaxForce()const{return m_dMaxForce;}
  void      SetMaxForce(float mf){m_dMaxForce = mf;}

  bool      IsSpeedMaxedOut()const{return m_dMaxSpeed*m_dMaxSpeed >= m_vVelocity.LengthSq();}
  float     Speed()const{return m_vVelocity.Length();}
  float     SpeedSq()const{return m_vVelocity.LengthSq();}

  Vector2D  Heading()const{return m_vHeading;}
  void      SetHeading(Vector2D new_heading);
  bool      RotateHeadingByAngle(float angle);
  bool      RotateHeadingToFacePosition(Vector2D target, float m_dMaxTurnRate);


};

//--------------------------- RotateHeadingToFacePosition ---------------------
//
//  given a target position, this method rotates the entity's heading and
//  side vectors by an amount not greater than m_dMaxTurnRate until it
//  directly faces the target.
//
//  returns true when the heading is facing in the desired direction
//-----------------------------------------------------------------------------
inline bool MovingEntity::RotateHeadingToFacePosition(Vector2D target, float m_dMaxTurnRate)
{
  Vector2D toTarget = Vec2DNormalize(target - m_vPos);
  
  std::cout << toTarget << std::endl;

  //first determine the angle between the heading vector and the target
  float angle = acos(m_vHeading.Dot(toTarget));
  
  //return true if the player is facing the target
  if (angle < 0.00001) return true;

  //clamp the amount to turn to the max turn rate
  if (fabs(angle) > m_dMaxTurnRate) angle = m_dMaxTurnRate;
  
  return RotateHeadingByAngle(angle * m_vHeading.Sign(toTarget));
}

//----------------------------- RotateHeadingByAngle --------------------------
//
// This method rotates the entity's heading and side vectors by an amount
//
//  returns true when the heading is facing in the desired direction
//-----------------------------------------------------------------------------
inline bool MovingEntity::RotateHeadingByAngle(float angle)
{
  //The next few lines use a rotation matrix to rotate the player's heading
  //vector accordingly
  C2DMatrix RotationMatrix;
  RotationMatrix.Rotate(angle);
  RotationMatrix.TransformVector2Ds(m_vHeading);
  RotationMatrix.TransformVector2Ds(m_vVelocity);

  return false;

}


//------------------------- SetHeading ----------------------------------------
//
//  first checks that the given heading is not a vector of zero length. If the
//  new heading is valid this fumction sets the entity's heading accordingly
//-----------------------------------------------------------------------------
inline void MovingEntity::SetHeading(Vector2D new_heading)
{
  //assert( (new_heading.LengthSq() - 1.0) < 0.00001);
  m_vHeading = new_heading;
}
