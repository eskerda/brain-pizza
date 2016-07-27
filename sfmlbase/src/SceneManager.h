#pragma once

#include "constants.h"

#include <stack>
#include <queue>
#include <string>
#include <typeinfo>

#include <SFML/Graphics.hpp>

#include "Scene.h"
#include "InputManager.h"
#include "Logger.h"

class SceneManager {
public:
	friend class Game;

	static void Change(Scene* scene) {
		actions.push(NULL);
		actions.push(scene);
	}

	static void Push(Scene* scene) {
		actions.push(scene);
	}

	static void Pop() {
		actions.push(NULL);
	}

private:

	static void Update(float dt) {
		while(!actions.empty()) {
			Scene* nova = actions.front();
			actions.pop();
			if (nova == NULL) Log::d("SceneManager POP");
			else Log::d(std::string("SceneManager PUSH ") + (typeid(*nova).name()));
			if (nova == NULL) { //pop
				Scene* antiga = sceneStack.top();
				antiga->Exit();
                Log::d(std::string("Destroying: ") + (typeid(*antiga).name()));
				delete antiga;
				sceneStack.pop();
                if (!sceneStack.empty()) {
                    Input::ResetCamera();
                    sceneStack.top()->Enter();
                }
            } else { //push
                if (!sceneStack.empty()) {
                    sceneStack.top()->Exit();
                }
                sceneStack.push(nova);
                Input::ResetCamera();
				nova->Enter();
			}
		}
		if (sceneStack.empty()) {
			Input::CloseWindow();
			return;
		}
		sceneStack.top()->Update(dt);
	}

	static void Draw(sf::RenderTarget& rw) {
		if (!sceneStack.empty()) sceneStack.top()->Draw(rw);
	}

	static void DestroyAll() {
		while(!actions.empty()) {
			Scene* s = actions.front();
			if (s != NULL) delete s;
			actions.pop();
		}
		while(!sceneStack.empty()) {
			delete sceneStack.top();
			sceneStack.pop();
		}
	}

	static std::stack<Scene*> sceneStack;
	static std::queue<Scene*> actions; //Scene* = push, NULL = pop

};
