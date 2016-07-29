#pragma once

#include "constants.h"

#include <SFML/Graphics.hpp>

#include "math/Bounds.h"
#include "math/Vector2D.h"
#include "BaseSteeringAgent.h"
#include "MovingEntity.h"
#include "SteeringBehaviors.h"
#include "AnimatedSprite.h"

#include <iostream>


#define ADD_STATE(Type, Instance) \
struct Type : public State { \
    void Enter(Zombie* z) override; \
    void Update(Zombie* z, float dt) override; \
    void Exit(Zombie* z) override; \
}; \
static Type Instance; \
friend Type;


class Zombie : public BaseSteeringAgent, public SelfRegister<Zombie*> {
private:
    struct State {
        virtual void Enter(Zombie* z) { }
        virtual void Update(Zombie* z, float dt) { }
        virtual void Exit(Zombie* z) { }
    };

public:
    ADD_STATE(StateWanderingType, StateWandering);
    ADD_STATE(StatePursuingType, StatePursuing);
    ADD_STATE(StateAttackingType, StateAttacking);

    Zombie(Vector2D pos, Bounds _bounds, MovingEntity* _target)
        : BaseSteeringAgent(pos, new AnimatedSprite("media/enemy.anim"))
        , bounds(_bounds)
        , target(_target)
	{
        steering->BoundsAvoidanceOn(bounds);
        steering->ObstacleAvoidanceOn();

        state = &StateWandering;
        state->Enter(this);
    }

    void Update(float dt) {
        BaseSteeringAgent::Update(dt);
        animatedSprite()->AddTime(dt);
        state->Update(this, dt);
    }

	void Draw(sf::RenderTarget& rt) override {
        const float MaxRotationDegs = 10;
        float targetRotation = RadsToDegs(Vector2D::ORIGIN.Angle(this->Heading()));
        float rotation = sprite->getRotation();
/*
        if (abs(rotation-targetRotation) > MaxRotationDegs) {
            if (targetRotation > rotation) targetRotation = rotation+MaxRotationDegs;
            else targetRotation = rotation-MaxRotationDegs;
        }*/

        sprite->setRotation(targetRotation);

		BaseSteeringAgent::Draw(rt);
	}

    void ChangeState(State& s) {
        if (state!=&s) {
            state->Exit(this);
            state = &s;
            state->Enter(this);
        }
    }

private:
	Bounds bounds;
    MovingEntity* target;
    State* state;

    //accessor shorter than casting each time
    AnimatedSprite* animatedSprite() {
        return ((AnimatedSprite*)sprite);
    }

};
