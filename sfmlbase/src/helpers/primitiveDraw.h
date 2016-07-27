#pragma once

#include "constants.h"

#include "math/Vector2D.h"

#include <SFML/Graphics.hpp>

inline void DrawQuad(sf::RenderTarget& rt, Vector2D pos, Vector2D size, sf::Color colorA = sf::Color::Red) {

	sf::Vertex vertices[] = {
		sf::Vertex(sf::Vector2f(pos.x,			pos.y),			colorA),
		sf::Vertex(sf::Vector2f(pos.x,			pos.y+size.y),	colorA),
		sf::Vertex(sf::Vector2f(pos.x+size.x,	pos.y+size.y),	colorA),
		sf::Vertex(sf::Vector2f(pos.x+size.x,	pos.y),			colorA)
	};

	rt.draw(vertices, 4, sf::Quads);

}
