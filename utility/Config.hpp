//
// Created by Олег Бобров on 29.12.2019.
//

#ifndef WG_FORCEMETHODCONFIG_HPP
#define WG_FORCEMETHODCONFIG_HPP

#include <SFML/Graphics.hpp>
#include "PlayerConfig.hpp"
#include "../linalg/Vector2.hpp"

namespace ForceMethodConfig {
    static const float charge = 100.f;
    static const float stiffnessK = 10.0f;
    static const float springLength = 1.f;
    static const float frictionK = 0.05f;
    static const float dampK = 0.9f;
}

namespace InterfaceConfig {
    static const float largeR = 1.0f;
    static const float smallR = 0.8f * largeR;
    static const sf::Color graphColor = {190, 190, 190};
    static const sf::Color playerTownColor = {190, 190, 0};
    static const sf::Color playerTrainColor = {250, 250, 0};
    static const sf::Color townColor = {0, 190, 0};
    static const sf::Color storageColor = {0, 0, 190};
    static const sf::Color marketColor = {190, 0, 0};
    static const sf::Color trainColor = {0, 250, 0};
    
    static const sf::Color buttonColor = {255, 255, 255};
    static const float buttonDarkFloat = 0.75f;
}

#endif //WG_FORCEMETHODCONFIG_HPP
