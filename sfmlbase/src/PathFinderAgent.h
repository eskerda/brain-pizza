#pragma once

#include "constants.h"

#include <SFML/Graphics.hpp>

#include "math/Bounds.h"
#include "math/Vector2D.h"
#include "InputManager.h"
#include "Sprite.h"
#include "MovingEntity.h"
#include "SteeringBehaviors.h"
#include "AStar.h"

#include <iostream>

#include "AiWorldScene.h"

class PathFinderAgent : public MovingEntity {
public:
	PathFinderAgent(Vector2D pos, AiWorldScene* worldscene) :
		MovingEntity(pos,20,200) ,
		world(worldscene)
	{
		sprite = new Sprite("media/pathfinder.png");
		sprite->OriginToCenter();
	};

	~PathFinderAgent() {
		delete sprite;
	};

	void Update(float dt) {

		if (Input::IsMouseJustPressed()) {
            Vector2D clickpos = Vector2D(Input::GetMousePositionInWorld());
			astar.Compute(world->GetWalkabilityMap(), Pos(), clickpos);
		}
			
		if (!astar.IsFinished()) {

			Vector2D nextPos = astar.GetNextPos(Pos());
			Vector2D direction = nextPos - Pos();
			direction.Normalize();
			m_vPos += (direction * m_dMaxSpeed * dt);

		} else {

			
			if (Input::IsKeyPressed(Input::MOVE_RIGHT)) m_vVelocity.x += 8*dt;
			if (Input::IsKeyPressed(Input::MOVE_LEFT))  m_vVelocity.x -= 8*dt;
			if (Input::IsKeyPressed(Input::MOVE_DOWN))  m_vVelocity.y += 8*dt;
			if (Input::IsKeyPressed(Input::MOVE_UP))    m_vVelocity.y -= 8*dt;

		
			//this prevents going faster in diagonal
			if (m_vVelocity.LengthSq() > 1) m_vVelocity.Normalize();
		
			//Add a vector of length m_dMaxSpeed (if m_vVelocity.Length() >= 1, 
			// so now is 1) or less (if m_vVelocity was < 1) to the position
			Vector2D newPos = m_vPos + (m_vVelocity * m_dMaxSpeed * dt);

			const Bounds screen_bounds(0,0,800,600);
			if (screen_bounds.IsInside(newPos)) {
				m_vPos = newPos;
			} else {
				//Just dont move
			}

			m_vVelocity -= m_vVelocity*8*dt;
		}

	}

	void Draw(sf::RenderTarget& rt) {
		sprite->setPosition(Pos());
		sprite->Draw(rt);
		if (!astar.IsFinished()) astar.Draw(rt);
	}

private:
	AiWorldScene* world;
	Sprite* sprite;
	AStar astar;
	
};
