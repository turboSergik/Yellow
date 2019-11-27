//
// Created by Олег Бобров on 15.11.2019.
//

#include "Train.h"
#include "../static/Database.h"

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
    Train::next_level_price = json.value("next_level_price", Train::next_level_price);
    Train::position = json.value("position", Train::position);
    Train::speed = json.value("speed", Train::speed);
    Train::player_idx = json.value("player_idx", Train::player_idx);
    if (json.contains("line_idx")) {
        auto & line_idx = json["line_idx"];
        if (line_idx != nullptr) {
            Train::line = Database::lines[line_idx];
            Train::transform->setParent(Train::line->transform);
        } else {
            //TODO: alert that line_idx = null (exception or smth else)
        }
    }
    //TODO: ask what this field store
    //"goods_type": null, WTF?
}

void Train::update() {
    sf::Vector2f endPosition = Train::line->transform->toLocalPosition(
            Train::line->points[1]->transform->getPosition());
    sf::Vector2f step = endPosition / static_cast<float>(Train::line->length);
    Train::transform->setLocalPosition(step* static_cast<float>(Train::position));
}
