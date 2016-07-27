#include "Game.h"

#include <SFML/Graphics.hpp>

#include "Scene.h"

#include "InputManager.h"
#include "GlobalShortcuts.h"

#include <iostream>
#include <string>
#include <GL/gl.h>

const float maxDeltaTime = 1.f/30.f; //Under 30 FPS, game will run slow. Dt is used between 30-60 FPS

int Game::init() {
	renderWindow.create(sf::VideoMode(200, 200), "Loading...");
	renderWindow.setVerticalSyncEnabled(true);

	Input::Init(renderWindow);

	srand((unsigned int)time(NULL));

	#ifdef LOG_TO_FILE
		FILE *outfile, *errfile;
		fopen_s(&outfile,"stdout.txt","w");
		fopen_s(&errfile,"stdout.txt","w");
		*stdout = *outfile;
		*stderr = *errfile;
		setvbuf(stdout, NULL, _IONBF, 0); //Sense buffer
		setvbuf(stderr, NULL, _IONBF, 0);
		std::ios::sync_with_stdio(true); //Per al cout i cerr
	#endif

	std::cout << "OpenGL Driver: " << glGetString(GL_VENDOR) << std::endl;

	return 0;
}

void Game::run(Scene* scene) {

	SceneManager::Push(scene);

	/*
	sf::Font font;
	font = fontMan->GetFont("Font/PALATINO.TTF");
	*/

	sf::Clock framecountClock;
	int frames = 0;

	sf::Clock dtClock;
	lastFrameTime = dtClock.getElapsedTime().asSeconds();

	//Game loop
	while(renderWindow.isOpen()) {

		float currentFrameTime = dtClock.getElapsedTime().asSeconds();
		float timeSinceLastFrame = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;
		if (timeSinceLastFrame > maxDeltaTime) timeSinceLastFrame = maxDeltaTime;

        Input::ProcessWindowEvents();
		Input::UpdateInputState();
		GlobalShortcuts::CheckInput();

		SceneManager::Update(timeSinceLastFrame);
		SceneManager::Draw(renderWindow);

		/*
		char* str = new char[30];
		sprintf(str, "%.4g", Framerate);
		sf::String FPStext(str, font, 40);
		FPStext.SetColor(sf::Color::Red);
		FPStext.Move(10, 15);
		*/

        renderWindow.display();

		frames++;
		if (framecountClock.getElapsedTime().asSeconds() > 1.0f) {
			std::cout << "FPS: " <<  frames << std::endl;
			frames = 0;
			framecountClock.restart();
		}

	}

	SceneManager::DestroyAll();

}

