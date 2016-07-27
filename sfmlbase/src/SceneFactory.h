#pragma once

#include "constants.h"

#include "Scene.h"

struct AbstractSceneFactory {
    virtual Scene* createInstance() = 0;
};

template<class T>
struct SceneFactory : public AbstractSceneFactory {

	Scene* createInstance() override {
		return new T;
	}

};
