//
// Created by Олег Бобров on 15.11.2019.
//

#ifndef WG_POINT_H
#define WG_POINT_H

#include <SFML/Graphics.hpp>
#include "../json.hpp"
#include "Behaviour.h"

class Post;

class Point : public sf::CircleShape, public Behaviour {
public:
    Post* post;

    explicit Point(int idx);
    void applyLayer10(const nlohmann::json &json) override;
};


#endif //WG_POINT_H
