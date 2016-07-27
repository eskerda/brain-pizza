#pragma once

#include "constants.h"

#include "AnimatedSprite.h"
#include "MovingEntity.h"
#include "math/Bounds.h"
#include "SceneManager.h"
#include "BaseSteeringAgent.h"
#include "Stroke.hpp"

class Ship : public BaseSteeringAgent {
public:

    Ship(const Vector2D& pos = Vector2D(0,0))
		: BaseSteeringAgent(pos, "media/ship.png", 220.f, 0.05f)
	{
		sprite->scale(0.5f,0.5f);
		decelerationPercent = 0.2;
		stroke = new Stroke();
		stroke->SetMaxJoints(20);
		stroke->SetInnerColor(sf::Color(25,255,255,0));
		stroke->SetEndThickness(0.0f);
		timer = 0;
	}

	void Update(float dt) {
		float vel = m_vVelocity.Length();

        if (Input::IsKeyPressed(Input::MOVE_UP) || Input::IsKeyPressed(Input::MOVE_UP2)) {
			vel += 90*dt;
		}
        if (Input::IsKeyPressed(Input::MOVE_DOWN) || Input::IsKeyPressed(Input::MOVE_DOWN2)) {
			vel -= 90*dt;
		}
        if (Input::IsKeyPressed(Input::MOVE_LEFT) || Input::IsKeyPressed(Input::MOVE_LEFT2)) {
			RotateHeadingByAngle(-0.0035*dt*(vel+25));
		}
        if (Input::IsKeyPressed(Input::MOVE_RIGHT) || Input::IsKeyPressed(Input::MOVE_RIGHT2)) {
			RotateHeadingByAngle(0.0035*dt*(vel+25));
		}

		m_vVelocity = vel*Heading()*0.996;
		m_vVelocity.Truncate(m_dMaxSpeed);
		m_vPos += m_vVelocity * dt;

		float angle = Vector2D(0,0).Angle(Heading());
		sprite->setRotation(RadsToDegs(angle));

		stroke->Calculate(dt);

		timer += dt;
		if (timer > 0.15) {
			timer -= 0.15;
			stroke->AddJoint(Pos(),9.f);
		}

	}

	void Draw(sf::RenderTarget& rt) override {
		BaseSteeringAgent::Draw(rt);
	}
	
	void DrawStroke(sf::RenderTarget& rt) {
		rt.draw(*stroke);
	}


private:
	Stroke* stroke;
	float timer;
};
