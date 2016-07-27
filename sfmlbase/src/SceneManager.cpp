#include "SceneManager.h"

std::stack<Scene*> SceneManager::sceneStack;
std::queue<Scene*> SceneManager::actions;