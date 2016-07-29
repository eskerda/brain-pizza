#pragma once

#include "constants.h"

#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "Scene.h"
#include "selfgrowingpool.h"
#include "Bike.h"
#include "RotoMessage.h"
#include "ZombieTileMap.h"

class ZombieScene : public Scene
{
public:
	void Enter() override;
	void Update(float dt) override;
    void AliveUpdate(float dt);
    void DeadUpdate(float dt);
	void Draw(sf::RenderTarget& rw) override;
	void Exit() override;

private:
    std::vector< std::vector<char> > tiles;
    ZombieTileMap* shaderTileMap;
    Sprite* hearth;
	sf::Text timer;
	int lives;
    RotoMessage loser;
    Bike* bike;
    float immunity;
    float fase;
    int steps;

    //SelfGrowingPool<Zombie> zombies;

	static const Vector2D mapSize;

};
