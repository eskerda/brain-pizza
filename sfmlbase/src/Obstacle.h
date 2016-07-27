#pragma once

#include "constants.h"

#include "math/Vector2D.h"
#include "helpers/selfregister.h"
#include "BaseGameEntity.h"
#include "Sprite.h"


class Obstacle : public BaseGameEntity, public SelfRegister<Obstacle*>
{
public:

	Obstacle(Vector2D pos = Vector2D(0,0), float radius = 24.f) : BaseGameEntity(pos, radius)
	{
		sprite = new Sprite("media/obstacle.png");
		sprite->OriginToCenter();
	}

	virtual ~Obstacle(){

	}

	void Update(float time_elapsed) {}

	void Draw(sf::RenderTarget& rt) {
		sprite->setPosition(Pos());
		sprite->Draw(rt);
	}

private:
	Sprite * sprite;
};

