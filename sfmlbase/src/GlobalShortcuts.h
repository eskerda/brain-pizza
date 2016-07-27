#pragma once

#include "constants.h"

#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include "InputManager.h"
#include "SceneManager.h"

class GlobalShortcuts
{
	public:
		static void CheckInput() {
			if (Input::IsKeyJustPressed(Input::ESCAPE)) {
				SceneManager::Pop();
			}

			//TODO: Mutejar jukebox (primer caldra fer una jukebox xD)

		}

};
