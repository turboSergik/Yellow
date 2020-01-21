//
// Created by Олег Бобров on 29.12.2019.
//

#ifndef WG_FORCEMETHODCONFIG_HPP
#define WG_FORCEMETHODCONFIG_HPP

#include <SFML/Graphics.hpp>

namespace ForceMethodConfig {
    const float charge = 100.f;
    const float stiffnessK = 10.0f;
    const float springLength = 1.f;
    const float frictionK = 0.05f;
    const float dampK = 0.9f;
}

namespace PlayerConfig {
    const std::string playerName = "YellowTeam";
    const std::string hostName = "DvaNoska1488";
    const int numPlayers = 1;
}

namespace InterfaceConfig {
    const float largeR = 1.0f;
    const float smallR = 0.8f * largeR;
    const sf::Color graphColor = {190, 190, 190};
    const sf::Color playerTownColor = {190, 190, 0};
    const sf::Color playerTrainColor = {250, 250, 0};
    const sf::Color townColor = {0, 190, 0};
    const sf::Color storageColor = {0, 0, 190};
    const sf::Color marketColor = {190, 0, 0};
    const sf::Color trainColor = {0, 250, 0};
}

#endif //WG_FORCEMETHODCONFIG_HPP
