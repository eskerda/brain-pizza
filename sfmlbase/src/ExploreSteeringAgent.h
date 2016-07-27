#pragma once

#include "constants.h"

#include <iostream>

#include <SFML/Graphics.hpp>

#include "math/Vector2D.h"
#include "math/Bounds.h"
#include "Sprite.h"
#include "MovingEntity.h"
#include "BaseSteeringAgent.h"
#include "SteeringBehaviors.h"

class ExploreSteeringAgent : public BaseSteeringAgent {
public:
	ExploreSteeringAgent(Vector2D pos, Bounds area = Bounds(50,50,700,500)) :
		BaseSteeringAgent(pos, "media/navi.png", 170),
		bounds(area)
	{
		steering->ForwardOn();
		steering->BoundsAvoidanceOn(bounds);
		SetHeading(Vector2D(1,-0.1));
    }
	void Update(float dt) {
		BaseSteeringAgent::Update(dt);
	}
	void Draw(sf::RenderTarget& rt) {
		bounds.Draw(rt,sf::Color::Blue);
		BaseSteeringAgent::Draw(rt);
	}
private:
	Bounds bounds;
};
