#pragma once

#include "constants.h"

#include <SFML/Graphics.hpp>

class SceneManager;
class Scene {
public:
	Scene();
	virtual ~Scene() { }

	virtual void Enter() = 0;
	virtual void Exit() = 0;

	virtual void Update(float dt) = 0;
	virtual void Draw(sf::RenderTarget& rw) = 0;

};
