#pragma once

#include "constants.h"

#include <SFML/Graphics.hpp>

#include "math/Bounds.h"
#include "math/Vector2D.h"
#include "BaseSteeringAgent.h"
#include "MovingEntity.h"
#include "SteeringBehaviors.h"

#include <iostream>

class WanderSteeringAgent : public BaseSteeringAgent, public SelfRegister<WanderSteeringAgent*> {
public:
	WanderSteeringAgent(Vector2D pos, Bounds area = Bounds(50,50,700,500)) :
		BaseSteeringAgent(pos, "media/enemy.png"),
		bounds(area)
	{
		steering->WanderOn();
		steering->BoundsAvoidanceOn(bounds);
		steering->ObstacleAvoidanceOn();
	}

	void Draw(sf::RenderTarget& rt) override {
		bounds.Draw(rt);
		BaseSteeringAgent::Draw(rt);
	}
private:
	Bounds bounds;
};
