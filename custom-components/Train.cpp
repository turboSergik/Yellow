//
// Created by Олег Бобров on 15.11.2019.
//

#include "Train.h"
#include "../static/Database.h"
#include "../static/Time.h"
#include <cmath>
#include <iostream>

const float PI = 3.14159265358f;

Train::Train(int idx) : Behaviour(idx) {

}

void Train::applyLayer1(const nlohmann::json &json) {
    Train::cooldown = json.value("cooldown", Train::cooldown);
    Train::fuel = json.value("fuel", Train::fuel);
    Train::fuel_capacity = json.value("fuel_capacity", Train::fuel_capacity);
    Train::fuel_consumption = json.value("fuel_consumption", Train::fuel_consumption);
    Train::goods = json.value("goods", Train::goods);
    Train::goods_capacity = json.value("goods_capacity", Train::goods_capacity);

    Train::level = json.value("level", Train::level);

    /// Train::next_level_price = json.value("next_level_price", Train::next_level_price);
    if (json.contains("next_level_price")){
        auto &next_level_price = json["next_level_price"];
        Train::next_level_price = next_level_price == nullptr ? 0 : next_level_price.get<int>();
    }


    //applying order important
    if (json.contains("line_idx")) {
        auto & line_idx = json["line_idx"];
        if (line_idx != nullptr) {
//            if (Train::line) {
//                const sf::Vector2f &startPosition = Train::line->points[0]->transform->getPosition();
//                const sf::Vector2f &endPosition = Train::line->points[1]->transform->getPosition();
//                sf::Vector2f step = (endPosition - startPosition) / static_cast<float>(Train::line->length);
//                sf::Vector2f targetPosition = startPosition + step * static_cast<float>(Train::position);
//                Train::transform->setPosition(targetPosition);
//            }
            Train::line = Database::lines[line_idx];
        } else {
            //TODO: alert that line_idx = null (exception or smth else)
        }
    }
    Train::position = json.value("position", Train::position);
    Train::speed = json.value("speed", Train::speed);
    Train::player_idx = json.value("player_idx", Train::player_idx);

    //TODO: ask what this field store
    //"goods_type": null, WTF?
}

void Train::update() {
    const sf::Vector2f & startPosition = Train::line->points[0]->transform->getPosition();
    const sf::Vector2f & endPosition = Train::line->points[1]->transform->getPosition();
    sf::Vector2f step = (endPosition - startPosition) / static_cast<float>(Train::line->length);
    sf::Vector2f targetPosition = startPosition + step * static_cast<float>(Train::position);
    const sf::Vector2f & currentPosition = Train::transform->getPosition();
    sf::Vector2f direction = targetPosition - currentPosition;
    float magnitude = Train::magnitude(direction);
    if (magnitude != 0) {
        float k = cross( {1, 0}, direction) >= 0 ? 1 : -1;
        float angle = k * acos(dot(direction, {1, 0})/magnitude)*180/PI;
        Train::transform->setRotation(angle);
    }
    Train::transform->setPosition(Train::lerp(currentPosition, targetPosition, 0.3f));
}

sf::Vector2f Train::lerp(const sf::Vector2f & a, const sf::Vector2f & b, float t) {
    return a + (b-a)*t;
}

float Train::cross(const sf::Vector2f &a, const sf::Vector2f &b) {
    return a.x*b.y - a.y*b.x;
}

float Train::dot(const sf::Vector2f &a, const sf::Vector2f &b) {
    return a.x*b.x + a.y*b.y;
}

float Train::magnitude(const sf::Vector2f &v) {
    return sqrt(v.x*v.x + v.y*v.y);
}
