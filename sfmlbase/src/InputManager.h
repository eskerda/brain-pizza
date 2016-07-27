#pragma once

#include "constants.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <iostream>

#include "helpers/noncopyable.h"
#include "math/Bounds.h"

/*
//Aproach A: Nomes un enum o defines. Te el problema que necessitem preinicialitzar un map de bools per al justpressed i ens caldria iterar per totes les tecles del enum (que no es pot)
//Solucio: Podriem simplement, si demanen una que no esta al map de justpresseds... retornar true el primer cop i la fiquem al map
//Cal pensar implicacions d'aixo... pero en principi sembla guai
//Lo dolent es que no es poden redefinir les tecles (ie: a les opcions del joc) sense recompilar
enum eKey {
	MOVE_UP = sf::Keyboard::Up
	MOVE_DOWN = sf::Keyboard::Down,
	MOVE_LEFT = sf::Keyboard::Left,
	MOVE_RIGHT = sf::Keyboard::Right
};


//Approach B: map intermig. Es el que faig servir a l'IsoClinous
//Mmes lent, pero a canvi no hi ha el problema d'inicialitzar el map de isJustPressed (va des de 0 fins a KEY_SIZE)
//Amb C++11 es lleugerament millor que abans, ja que no cal funcio init... perque segueix tenint un puto map xD
//Lo bo del map intermig es que es poden redifinir les tecles sense recompilar (ie: a les opcions del joc)
enum eKey {
	MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT,
	KEYS_SIZE
};
std::map<eKey, int> keyMapping = {
	{MOVE_UP,		sf::Keyboard::Up},
	{MOVE_DOWN,		sf::Keyboard::Down},
	{MOVE_LEFT,		sf::Keyboard::Left},
	{MOVE_RIGHT,	sf::Keyboard::Right},
};


//Approach C: Com el B pero amb array en lloc de map. Es el que faig servir aqui
//jo crec que es lo puto millor dels dos mons: es pot editar en runtime, coneixem el total de KEYS_SIZE, es rapid d'accedir...
enum eKey {
	MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT,
	KEYS_SIZE
};
int key_map[KEYS_SIZE];
key_map[MOVE_UP] = sf::Keyboard::Up;
key_map[MOVE_DOWN] = sf::Keyboard::Down;



A part, tambe hi ha dos approaches en cuant a fer-ho static o singleton. 
A mi singleton no m'agrada, l'unic que te de bo es que t'assegures que es crida el constructor.
Aqui feia servir un hack per evitar aixo (esta al final per comentat), perque al final he necessitat
passar un parametre al constructor, aixi que he fet una funcio Init que m'he d'enrecordar de cridar
(cosa que, per cert, amb singleton tampoc es soluciona, perque singleton = shit)
*/


//Static class. Fuck singletons and all the patterns bullshit.
//Input::GetInstance()->IsPressed vs Input::IsPressed... fast'n clean bro
class Input : noncopyable {
public:
	friend class Game; //Init and Update are private, only Game should call them


	//KEY DEFINITION
	//==============

	enum Keys {
		POSITIVE, NEGATIVE, ESCAPE, SKIP,
		MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT,
        MOVE_UP2, MOVE_DOWN2, MOVE_LEFT2, MOVE_RIGHT2,
		MENU_UP, MENU_DOWN,
        PLUS, MINUS,
        DEBUG_R, DEBUG_G, DEBUG_D, DEBUG_F, DEBUG_C, DEBUG_H, DEBUG_Z, DEBUG_P, DEBUG_B,
		DEBUG_F1, DEBUG_F2, DEBUG_F3,
		KEYS_SIZE
	};

	static void DefineDefaultControls() {
		key_map[POSITIVE] = sf::Keyboard::Return;
		key_map[NEGATIVE] = sf::Keyboard::Escape;
		key_map[SKIP] = sf::Keyboard::Space;
		key_map[ESCAPE] = sf::Keyboard::Escape;
		key_map[MENU_UP] = sf::Keyboard::Up;
		key_map[MENU_DOWN] = sf::Keyboard::Down;
		key_map[MOVE_UP] = sf::Keyboard::Up;
		key_map[MOVE_DOWN] = sf::Keyboard::Down;
		key_map[MOVE_LEFT] = sf::Keyboard::Left;
		key_map[MOVE_RIGHT] = sf::Keyboard::Right;
        key_map[MOVE_UP2] = sf::Keyboard::W;
        key_map[MOVE_DOWN2] = sf::Keyboard::S;
        key_map[MOVE_LEFT2] = sf::Keyboard::A;
        key_map[MOVE_RIGHT2] = sf::Keyboard::D;
		key_map[DEBUG_G] = sf::Keyboard::G;
		key_map[DEBUG_R] = sf::Keyboard::R;
		key_map[DEBUG_D] = sf::Keyboard::D;
		key_map[DEBUG_F] = sf::Keyboard::F;
		key_map[DEBUG_C] = sf::Keyboard::C;
		key_map[DEBUG_H] = sf::Keyboard::H;
		key_map[DEBUG_Z] = sf::Keyboard::Z;
		key_map[DEBUG_P] = sf::Keyboard::P;
        key_map[DEBUG_B] = sf::Keyboard::B;
		key_map[DEBUG_F1] = sf::Keyboard::F1;
		key_map[DEBUG_F2] = sf::Keyboard::F2;
		key_map[DEBUG_F3] = sf::Keyboard::F3;
        key_map[PLUS] = sf::Keyboard::Add;
        key_map[MINUS] = sf::Keyboard::Subtract;

		for(int i = 0; i < KEYS_SIZE; i++) key_states[i] = RELEASED;
		for(int i = 0; i < sf::Mouse::ButtonCount; i++) button_states[i] = RELEASED;
	}

	static void DefineKey(Keys action, sf::Keyboard::Key key) {
		key_map[action] = key;
		key_states[action] = RELEASED;
	}


	
	//KEYBOARD ACCESS
	//===============

	static bool IsKeyPressed(Keys k) {
		return (key_states[k] == PRESSED || key_states[k] == JUST_PRESSED);
	}

	static bool IsKeyJustPressed(Keys k) {
		return (key_states[k] == JUST_PRESSED);
	}

	static bool IsKeyReleased(Keys k) {
		return (key_states[k] == RELEASED || key_states[k] == JUST_RELEASED);
	}

	static bool IsKeyJustReleased(Keys k) {
		return (key_states[k] == JUST_RELEASED);
	}



	//MOUSE ACCESS
	//============

	static bool IsMousePressed(sf::Mouse::Button b = sf::Mouse::Left) {
		if (!WindowHasFocus()) return false;
		if (!IsMouseInsideWindow()) return false;
		return (button_states[b] == PRESSED || button_states[b] == JUST_PRESSED);
	}
	
	static bool IsMouseJustPressed(sf::Mouse::Button b = sf::Mouse::Left) {
		if (!WindowHasFocus()) return false;
		if (!IsMouseInsideWindow()) return false;
		return (button_states[b] == JUST_PRESSED);
	}
	
	static bool IsMouseReleased(sf::Mouse::Button b = sf::Mouse::Left) {
		if (!WindowHasFocus()) return false;
		if (!IsMouseInsideWindow()) return false;
		return (button_states[b] == RELEASED || button_states[b] == JUST_RELEASED);
	}
	
	static bool IsMouseJustReleased(sf::Mouse::Button b = sf::Mouse::Left) {
		if (!WindowHasFocus()) return false;
		if (!IsMouseInsideWindow()) return false;
		return (button_states[b] == JUST_RELEASED);
	}
	
    static sf::Vector2i GetMousePositionInWindow() {
		return sf::Mouse::getPosition(*window); //window arg is needed for relative coords
	}

    static Vector2D GetMousePositionInWorld() {
        Vector2D displacement = defaultView.getCenter() - (defaultView.getSize()/2.f);
        return Vector2D(GetMousePositionInWindow()) + displacement;
    }

	//WINDOW MANAGEMENT
	//=================

	static void SetWindowCaption(const std::string& s) {
		window->setTitle(s);
	}

	static bool WindowHasFocus() {
		return focus;
	}

	static void CloseWindow() {
		window->close();
	}

    static void SetWindowSize(sf::Vector2u size, bool centerCamera = true) {
        window->setSize(size);
        ProcessWindowEvents();
        if (centerCamera) SetCameraCenter(size/2);
	}

	static sf::Vector2u GetWindowSize() {
		return window->getSize();
	}

	static Bounds GetWindowBounds() {
		return Bounds(0.f, 0.f, (float)window->getSize().x, (float)window->getSize().y);
	}

	static bool IsMouseInsideWindow() {
		sf::Vector2i vec = sf::Mouse::getPosition(*window);
		if (vec.x < 0 || vec.x > (int)window->getSize().x) return false;
		if (vec.y < 0 || vec.y > (int)window->getSize().y) return false;
		return true;
	}

	//CAMERA MANAGEMENT
	//=================

    static void SetCameraCenter(const Vector2D& center);

    static Vector2D GetCameraCenter();

    static Bounds GetCameraBounds();

    //Useful for debug pourposes
    static void MoveCameraWithArrows(float velocity, float dt);
    static void ChangeZoomWithPlusAndMinus(float velocity, float dt);

    static void ClampCameraTo(const Bounds& b);

    static void ResetCamera();

    static void SetZoom(float z);

    static float GetZoom();

	static void StartGuiDraw() {
        window->setView(guiView);
	}

	static void EndGuiDraw() {
		window->setView(defaultView);
	}

private:
	Input() { }

	static void Init(sf::RenderWindow& renderwindow) {
		focus = true;
		window = &renderwindow;
		ResetCamera();
		DefineDefaultControls();
	}

    static void ProcessWindowEvents()
    {

		sf::Event sfmlevent;
		while (window->pollEvent(sfmlevent)) {
			switch(sfmlevent.type) {
				case sf::Event::LostFocus:
					focus = false;
					break;
				case sf::Event::GainedFocus:
					focus = true;
					break;
				case sf::Event::Closed:
					window->close();
					break;
				case sf::Event::Resized: //To disable sfml's automatic scaling
                    {
                    Vector2D currentCenter = defaultView.getCenter();
                    defaultView.setSize(sf::Vector2f(window->getSize()));
                    defaultView.setCenter(currentCenter);
                    defaultView.zoom(1.f/zoom);
                    guiView.setSize(sf::Vector2f(window->getSize()));
                    guiView.setCenter(Vector2D(window->getSize())/2);
					window->setView(defaultView);
                    }
                    break;
				default:
					break;
			}
		}
    }

     static void UpdateInputState()
     {
		for(int i = 0; i < KEYS_SIZE; i++) {
			if (sf::Keyboard::isKeyPressed(key_map[i]) && WindowHasFocus()) {
				if (key_states[i] == JUST_PRESSED || key_states[i] == PRESSED) {
					key_states[i] = PRESSED;
				} else {
					key_states[i] = JUST_PRESSED;
				}
			} else {
				if (key_states[i] == JUST_RELEASED || key_states[i] == RELEASED) {
					key_states[i] = RELEASED;
				} else {
					key_states[i] = JUST_RELEASED;
				}
			}
		}
		
        for(int i = 0; i < sf::Mouse::ButtonCount; i++) {
			if (sf::Mouse::isButtonPressed((sf::Mouse::Button)i)) {
				if (button_states[i] == JUST_PRESSED || button_states[i] == PRESSED) {
					button_states[i] = PRESSED;
				} else {
					button_states[i] = JUST_PRESSED;
				}
			} else {
				if (button_states[i] == JUST_RELEASED || button_states[i] == RELEASED) {
					button_states[i] = RELEASED;
				} else {
					button_states[i] = JUST_RELEASED;
				}
			}
		}
	}

	enum KeyStates { JUST_RELEASED, RELEASED, JUST_PRESSED, PRESSED };
	
	static sf::Keyboard::Key key_map[KEYS_SIZE];
	static KeyStates key_states[KEYS_SIZE];
	static KeyStates button_states[sf::Mouse::ButtonCount];
	static sf::RenderWindow* window;
	static sf::View defaultView, guiView;
    static bool focus;
    static float zoom;

	/*
	//Hack to call Init at the startup so we never forget to call it
	struct RunAtStartupHack {
		RunAtStartupHack() {
			Input::Init();
		}
	};
	static RunAtStartupHack runAtStartup;
	*/

};
