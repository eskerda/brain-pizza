#pragma once

#include "constants.h"

#include <SFML/Graphics.hpp>

#include "FontResMan.h"
#include "math/Vector2D.h"
#include "GlobalClock.h"
#include "InputManager.h"

class RotoMessage : public sf::Text
{
	float messageTimeStart;
	float messageTime;

public:

    RotoMessage() : sf::Text("NO TEXT", FontResMan::GetInstance()->Get("media/Frijole-Regular.ttf"), 58) {
		setColor(sf::Color::Green);
		messageTime = -1;
	}

	void ShowMessage(sf::String msg) {
	
		setString(msg);
		
		sf::FloatRect textRect = getLocalBounds();
		setOrigin(textRect.left + textRect.width/2.0f,
						textRect.top  + textRect.height/2.0f);

		messageTime = 0.01;

	}

	float GetElapsedTime() {
		return messageTime;
	}

	void Update(float dt) {
		
		if (messageTime >= 0) {

			messageTime += dt;

			float ang = sin(GlobalClock.getElapsedTime().asSeconds()*2)*10;
			float sc = 1;
			if(messageTime < 0.2)
			{
				sc = 1+(0.2-messageTime)*10;
				ang -= (0.2-messageTime)*600;
			}
			
			setRotation(ang);

			if(messageTime < 0.2) {
				sc = messageTime / 0.2;
			}
			setScale(sc, sc);

		}

	}

	void Draw(sf::RenderTarget& rt) {
		if (messageTime >= 0) {
			setPosition(Vector2D(Input::GetWindowSize()/2));
			rt.draw(*this);
		}
	}

};



