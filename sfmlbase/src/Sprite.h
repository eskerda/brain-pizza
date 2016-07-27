#pragma once

#include "constants.h"

#include "TextureResMan.h"
#include "Logger.h"
#include "helpers/stringhelpers.h"
#include "math/Vector2D.h"

#include <SFML/Graphics.hpp>

//Abstraction for sf::Sprite that also manages the texture loading, using the resource manager
class Sprite : public sf::Sprite {
public:
	explicit Sprite(const std::string& s) { Load(s); }
	explicit Sprite(const std::string& s, const Vector2D& pos) { Load(s); setPosition(pos); }
    Sprite(const Sprite& s) : sf::Sprite(s) {
        filename = s.filename;
        TextureResMan::GetInstance()->Get(s.filename); //Increase resource count
    }

	void Load(const std::string& s) {
		filename = s;
		setTexture(TextureResMan::GetInstance()->Get(s));
	}

	virtual ~Sprite() {
		TextureResMan::GetInstance()->Unget(filename);
	}

	void Draw(sf::RenderTarget& rt) {
		rt.draw(*this);
	}

	virtual void OriginToCenter() {
		sf::Vector2u s = getTexture()->getSize();
		setOrigin(s.x/2,s.y/2);
	}

    void SetPos(float x, float y)
    {
        setPosition(x,y);
    }

    void SetPos(const Vector2D& pos)
    {
        setPosition(pos);
    }

    Vector2D Pos() const
    {
        return getPosition();
    }

protected:
	Sprite() { }

private:
	std::string filename;

};
