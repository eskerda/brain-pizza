#pragma once

#include "constants.h"

#include <SFML/Graphics.hpp>

#include "math/Vector2D.h"
#include "Sprite.h"
#include "MovingEntity.h"
#include "SteeringBehaviors.h"

#include <iostream>

class BaseSteeringAgent : public MovingEntity {
public:
    BaseSteeringAgent(Vector2D pos, Sprite* _sprite, float max_speed = 60, float max_force = 10) :
        MovingEntity(pos, 30, max_speed, max_force)
    {
        sprite = _sprite;
        sprite->OriginToCenter();
        sprite->setPosition(pos);
        steering = new SteeringBehavior(this);
        decelerationPercent = 0;
    }

    BaseSteeringAgent(Vector2D pos, const std::string& sprite_file, float max_speed = 60, float max_force = 10) :
        MovingEntity(pos, 30, max_speed, max_force) 
    {
        sprite = new Sprite(sprite_file);
        sprite->OriginToCenter();
        sprite->setPosition(pos);
        steering = new SteeringBehavior(this);
        decelerationPercent = 0;
    }

    ~BaseSteeringAgent() {
        delete steering;
        delete sprite;
    }

    void Update(float dt) override {
        
        //calculate the combined force from each steering behavior in the vehicle's list
        Vector2D SteeringForce = steering->Calculate(dt);
    
        //update velocity
        m_vVelocity += SteeringForce * dt; 

        //make sure vehicle does not exceed maximum velocity
        m_vVelocity.Truncate(m_dMaxSpeed);

        //update the position
        m_vPos += m_vVelocity * dt;
        
        //update the heading if the vehicle has a non zero velocity
        SetHeading(Vec2DNormalize(m_vVelocity));

        if (decelerationPercent > 0) {
            Vector2D nv = m_vVelocity;
            nv.Normalize();
            nv *= decelerationPercent*m_dMaxSpeed*dt; //Almost instant deceleration
            if (nv.LengthSq() > m_vVelocity.LengthSq()) nv = m_vVelocity;
            m_vVelocity -= nv;
        }

    }

    void Draw(sf::RenderTarget& rt) override {
        sprite->setPosition(Pos());
        sprite->Draw(rt);
    }
	
    Sprite* sprite;
protected:
    float decelerationPercent;
    SteeringBehavior* steering;
    
};
