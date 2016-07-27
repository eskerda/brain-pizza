#pragma once

#include <string>

#include "ResMan.h"
#include "Singleton.h"

#include <SFML/Graphics.hpp>

class TextureResMan : public ResMan<sf::Texture>, public Singleton<TextureResMan> {
private:
	virtual sf::Texture* Load(const std::string& file) {
		sf::Texture* t = new sf::Texture();
		t->loadFromFile(file);
		return t;
	}
	virtual void Unload(sf::Texture* obj) {
		delete obj;
	}
};

