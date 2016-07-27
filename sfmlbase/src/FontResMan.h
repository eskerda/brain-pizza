#pragma once

#include <string>

#include "ResMan.h"
#include "Singleton.h"
#include "Logger.h"

#include <SFML/Graphics.hpp>

class FontResMan : public ResMan<sf::Font>, public Singleton<FontResMan> {
public:
	static const sf::Font& GetDefaultFont() {
		return GetInstance()->Get("media/arial.ttf");
	}

private:
	virtual sf::Font* Load(const std::string& file) {
		sf::Font* f = new sf::Font();
		f->loadFromFile(file);
		return f;
	}

	virtual void Unload(sf::Font* f) {
		delete f;
	}

};
