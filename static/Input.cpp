#include "Input.hpp"

//for keys
std::bitset<sf::Keyboard::KeyCount> Input::pressedKeys;
std::bitset<sf::Keyboard::KeyCount> Input::heldKeys;
std::bitset<sf::Keyboard::KeyCount> Input::releasedKeys;

//for mouse buttons
std::bitset<sf::Mouse::ButtonCount> Input::pressedMouseButtons;
std::bitset<sf::Mouse::ButtonCount> Input::releasedMouseButtons;
std::array<sf::Event::MouseButtonEvent,
        sf::Mouse::ButtonCount> Input::pressedMouseEvents;
std::array<sf::Event::MouseButtonEvent,
        sf::Mouse::ButtonCount> Input::releasedMouseEvents;

sf::Event::MouseWheelScrollEvent Input::wheelScrollEvent;
bool Input::wheelScrolled = false;

//Input & Input::instance() {
//    static Input input;
//    return input;
//}

void Input::addKeyPressed(sf::Event::KeyEvent keyEvent) {
    if (keyEvent.code >= 0) {
        pressedKeys.set(static_cast<size_t>(keyEvent.code));
        heldKeys.set(static_cast<size_t>(keyEvent.code));
    }
    // another case will trigger if 
    // sfml don't know what is this button
}

void Input::addKeyReleased(sf::Event::KeyEvent keyEvent) {
    if (keyEvent.code >= 0) {
        releasedKeys.set(static_cast<size_t>(keyEvent.code));
        heldKeys.reset(static_cast<size_t>(keyEvent.code));
    }
}

bool Input::isKeyDown(sf::Keyboard::Key key) {
    // assume you will not ask sf::Keybord::Unknown
    return pressedKeys[static_cast<size_t>(key)];
}

bool Input::isKeyUp(sf::Keyboard::Key key) {
    return releasedKeys[static_cast<size_t>(key)];
}

bool Input::isKey(sf::Keyboard::Key key) {
    return heldKeys[static_cast<size_t>(key)];
}

void Input::addMouseButtonPressed(sf::Event::MouseButtonEvent buttonEvent) {
    pressedMouseButtons.set(buttonEvent.button);
    pressedMouseEvents[buttonEvent.button] = buttonEvent;
}

void Input::addMouseButtonReleased(sf::Event::MouseButtonEvent buttonEvent) {
    releasedMouseButtons.set(buttonEvent.button);
    releasedMouseEvents[buttonEvent.button] = buttonEvent;
}

bool Input::getMouseButtonPressed(sf::Mouse::Button button) {
    return releasedMouseButtons[button];
}

bool Input::getMouseButtonReleased(sf::Mouse::Button button) {
    return releasedMouseButtons[button];
}

sf::Event::MouseButtonEvent Input::getMBPressedEvent(sf::Mouse::Button button) {
    return releasedMouseEvents[button];
}

sf::Event::MouseButtonEvent Input::getMBReleasedEvent(sf::Mouse::Button button) {
    return releasedMouseEvents[button];
}

void Input::addWheelScroll(sf::Event::MouseWheelScrollEvent mouseScrollEvent) {
    wheelScrolled = true;
    Input::wheelScrollEvent = mouseScrollEvent;
}

sf::Event::MouseWheelScrollEvent Input::getWheelScrollEvent() {
    return wheelScrollEvent;
}

bool Input::getWheelScrolled() {
    return wheelScrolled;
}

void Input::reset() {
    pressedKeys.reset();
    releasedKeys.reset();
    pressedMouseButtons.reset();
    releasedMouseButtons.reset();
    wheelScrolled = false;
}
