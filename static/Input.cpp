#include "Input.hpp"

Input & Input::instance() {
    static Input input;
    return input;
}

void Input::addKey(sf::Event::KeyEvent keyEvent) {
    if (keyEvent.code >= 0) {
        pressedKeys.set(static_cast<size_t>(keyEvent.code));
    }
    // another case will trigger if 
    // sfml don't know what is this button
}

bool Input::getKeyPressed(sf::Keyboard::Key key) {
    // assume you will not ask sf::Keybord::Unknown
    return pressedKeys[static_cast<size_t>(key)];
}

void Input::reset() {
    pressedKeys.reset();
}
