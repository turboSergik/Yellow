#include "Button.hpp"
#include "../../static/Input.hpp"
#include "../../utility/Config.hpp"
#include "../Camera.h"
#include <iostream>

size_t Button::ButtonCounter;

Button::Button() {
    this->buttonIdx = Button::ButtonCounter++;
}

sf::Color multipleColorByValue(const sf::Color & color, float val) {
    sf::Color result = {static_cast<sf::Uint8>(static_cast<float>(color.r) * val), 
            static_cast<sf::Uint8>(static_cast<float>(color.g) * val), 
            static_cast<sf::Uint8>(static_cast<float>(color.b) * val)};
    return result;
}

void Button::update() {
//    if (Input::getMouseButtonPressed(sf::Mouse::Left)) {
//        std::cout << "here" << std::endl;
//    }
    Vector2 worldPoint = Camera::mainCamera->screenToWorldPoint(Input::mousePosition);    
    bool overlaps = this->buttonCollider->isOverlapsPoint(worldPoint);
    if (overlaps) {
        if (Input::getMouseButtonHeld(sf::Mouse::Left)) {
            renderer->rectangle.setFillColor(
                multipleColorByValue(InterfaceConfig::buttonColor, 
                                     InterfaceConfig::buttonDarkFloat * 
                                     InterfaceConfig::buttonDarkFloat));
        } else if (Input::getMouseButtonReleased(sf::Mouse::Left)) {
            renderer->rectangle.setFillColor(InterfaceConfig::buttonColor);
            onPress.invoke(buttonIdx);
        } else {
            renderer->rectangle.setFillColor(
                multipleColorByValue(InterfaceConfig::buttonColor, 
                                     InterfaceConfig::buttonDarkFloat));
        }
    } else {
        renderer->rectangle.setFillColor(InterfaceConfig::buttonColor);
    }
}

