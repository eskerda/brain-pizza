#pragma once

#include "constants.h"

#include "SceneManager.h"
#include "Scene.h"

#include <SFML/Graphics.hpp>

class Game {
	
	public:
		int init();
		void run(Scene* scene); 

	private:
		sf::RenderWindow renderWindow;
		float lastFrameTime;
};