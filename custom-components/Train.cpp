//
// Created by Олег Бобров on 15.11.2019.
//

#include "Train.h"
#include "../static/Database.h"

Train::Train(int idx) : Behaviour(idx) {
    
}

void Train::applyLayer1(const nlohmann::json &json) {
    json.tryGetValue("cooldown", Train::cooldown);
    json.tryGetValue("fuel", Train::fuel);
    json.tryGetValue("fuel_capacity", Train::fuel_capacity);
    json.tryGetValue("fuel_consumption", Train::fuel_consumption);
    json.tryGetValue("goods", Train::goods);
    json.tryGetValue("goods_capacity", Train::goods_capacity);
    json.tryGetValue("level", Train::level);
    json.tryGetValue("next_level_price", Train::next_level_price);
    json.tryGetValue("position", Train::position);
    json.tryGetValue("speed", Train::speed);
    json.tryGetValue("player_idx", Train::player_idx);
    nlohmann::json line_idx;
    json.tryGetValue("line_idx", line_idx);
    if (line_idx != nullptr) {
        Train::line = Database::lines[line_idx];
        Train::transform->setParent(Train::line->transform);
    } else {
        //TODO: alert that line_idx = null (exception or smth else)
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
