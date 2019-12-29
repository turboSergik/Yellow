#include "Input.hpp"
#include <iostream>
//for keys
ThreadSafeBitset<sf::Keyboard::KeyCount> Input::pressedKeys;
ThreadSafeBitset<sf::Keyboard::KeyCount> Input::heldKeys;
ThreadSafeBitset<sf::Keyboard::KeyCount> Input::releasedKeys;

//for mouse buttons
ThreadSafeBitset<sf::Mouse::ButtonCount> Input::pressedMouseButtons;
ThreadSafeBitset<sf::Mouse::ButtonCount> Input::releasedMouseButtons;
std::array<std::pair<sf::Event::MouseButtonEvent, std::mutex>,
        sf::Mouse::ButtonCount> Input::pressedMouseEvents;
std::array<std::pair<sf::Event::MouseButtonEvent, std::mutex>,
        sf::Mouse::ButtonCount> Input::releasedMouseEvents;

std::mutex Input::wheelScrollMutex;
sf::Event::MouseWheelScrollEvent Input::wheelScrollEvent;
bool Input::wheelScrolled = false;


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
    return pressedKeys.get(static_cast<size_t>(key));
}

bool Input::isKeyUp(sf::Keyboard::Key key) {
    return releasedKeys.get(static_cast<size_t>(key));
}

bool Input::isKey(sf::Keyboard::Key key) {
    return heldKeys.get(static_cast<size_t>(key));
}

void Input::addMouseButtonPressed(sf::Event::MouseButtonEvent buttonEvent) {
    pressedMouseButtons.set(buttonEvent.button);
    pressedMouseEvents[buttonEvent.button].second.lock();
    pressedMouseEvents[buttonEvent.button].first = buttonEvent;
    pressedMouseEvents[buttonEvent.button].second.unlock();
}

void Input::addMouseButtonReleased(sf::Event::MouseButtonEvent buttonEvent) {
    releasedMouseButtons.set(buttonEvent.button);
    releasedMouseEvents[buttonEvent.button].second.lock();
    releasedMouseEvents[buttonEvent.button].first = buttonEvent;
    releasedMouseEvents[buttonEvent.button].second.unlock();
}

bool Input::getMouseButtonPressed(sf::Mouse::Button button) {
    return pressedMouseButtons.get(button);
}

bool Input::getMouseButtonReleased(sf::Mouse::Button button) {
    return releasedMouseButtons.get(button);
}

sf::Event::MouseButtonEvent Input::getMBPressedEvent(sf::Mouse::Button button) {
    std::lock_guard<std::mutex> lock(pressedMouseEvents[button].second);
    return pressedMouseEvents[button].first;
}

sf::Event::MouseButtonEvent Input::getMBReleasedEvent(sf::Mouse::Button button) {
    std::lock_guard<std::mutex> lock(releasedMouseEvents[button].second);
    return releasedMouseEvents[button].first;
}

void Input::addWheelScroll(sf::Event::MouseWheelScrollEvent mouseScrollEvent) {
    std::lock_guard<std::mutex> lock(wheelScrollMutex);
    wheelScrolled = true;
    Input::wheelScrollEvent = mouseScrollEvent;
}

sf::Event::MouseWheelScrollEvent Input::getWheelScrollEvent() {
    std::lock_guard<std::mutex> lock(wheelScrollMutex);    
    return wheelScrollEvent;
}

bool Input::getWheelScrolled() {
    std::lock_guard<std::mutex> lock(wheelScrollMutex);    
    return wheelScrolled;
}

void Input::reset() {
    pressedKeys.reset();
    releasedKeys.reset();
    pressedMouseButtons.reset();
    releasedMouseButtons.reset();
    std::lock_guard<std::mutex> lock(wheelScrollMutex);    
    wheelScrolled = false;
}
