#include "InputManager.h"

sf::Keyboard::Key Input::key_map[KEYS_SIZE];
Input::KeyStates Input::key_states[KEYS_SIZE];
Input::KeyStates Input::button_states[sf::Mouse::ButtonCount];
sf::RenderWindow* Input::window;
sf::View Input::defaultView;
sf::View Input::guiView;
bool Input::focus;
float Input::zoom;
//Input::RunAtStartupHack Input::runAtStartup;



void Input::SetCameraCenter(const Vector2D& center) {
    defaultView.setCenter(center);
    window->setView(defaultView);
    ProcessWindowEvents();
}

Vector2D Input::GetCameraCenter() {
    return defaultView.getCenter();
}

Bounds Input::GetCameraBounds() {
    sf::Vector2f viewSize = defaultView.getSize();
    Vector2D viewOrigin = defaultView.getCenter() - (viewSize/2.f);
    return Bounds(viewOrigin.x, viewOrigin.y, viewSize.x, viewSize.y);
}

//Useful for debug pourposes
void Input::MoveCameraWithArrows(float velocity, float dt) {
    Vector2D c = GetCameraCenter();
    if (Input::IsKeyPressed(Input::MOVE_RIGHT)) {
        c.x += velocity*dt*10/zoom;
    }
    if (Input::IsKeyPressed(Input::MOVE_LEFT))  {
        c.x -= velocity*dt*10/zoom;
    }
    if (Input::IsKeyPressed(Input::MOVE_DOWN))  {
        c.y += velocity*dt*10/zoom;
    }
    if (Input::IsKeyPressed(Input::MOVE_UP))    {
        c.y -= velocity*dt*10/zoom;
    }
    SetCameraCenter(c);
}

//Useful for debug pourposes
void Input::ChangeZoomWithPlusAndMinus(float zoomVel, float dt) {
    float zoom = Input::GetZoom();
    if (Input::IsKeyPressed(Input::PLUS) ||
        Input::IsKeyPressed(Input::PG_UP)) {
        zoom += zoomVel*dt;
    }
    if (Input::IsKeyPressed(Input::MINUS) ||
        Input::IsKeyPressed(Input::PG_DOWN)) {
        zoom -= zoomVel*dt;
    }
    Input::SetZoom(zoom);
}

void Input::ClampCameraTo(const Bounds& limit) {
    Vector2D c = GetCameraCenter();

    Vector2D screenSize = GetWindowSize();
    screenSize /= zoom;
    float halfScreenWidth = screenSize.x / 2.f;
    float halfScreenHeight = screenSize.y / 2.f;

    //TODO: Center if viewport is bigger than limits
    if (c.x+halfScreenWidth > limit.Right()) c.x = limit.Right() - halfScreenWidth;
    if (c.x-halfScreenWidth < limit.Left()) c.x = limit.Left() + halfScreenWidth;
    if (c.y+halfScreenHeight > limit.Bottom()) c.y = limit.Bottom() - halfScreenHeight;
    if (c.y-halfScreenHeight < limit.Top()) c.y = limit.Top() + halfScreenHeight;

    SetCameraCenter(c);
}

void Input::ResetCamera() {
    defaultView.setSize(sf::Vector2f(window->getSize()));
    defaultView.setCenter(Vector2D(window->getSize())/2);
    zoom = 1.f;
    defaultView.zoom(1.f/zoom);
    defaultView.setViewport(sf::FloatRect(0, 0, 1, 1));
    //GUI View is never moved so it shouldn't be necessary to reset it
    window->setView(defaultView);
    ProcessWindowEvents();
}

void Input::SetZoom(float z) {
    defaultView.zoom(zoom);
    zoom = z;
    defaultView.zoom(1.f/zoom);
    window->setView(defaultView);
    ProcessWindowEvents();
}

float Input::GetZoom() {
    return zoom;
}
