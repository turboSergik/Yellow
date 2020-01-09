//
// Created by Олег Бобров on 15.11.2019.
//

#include "Train.h"
#include "../static/Database.h"
#include <cmath>

const float PI = 3.14159265358f;

Train::Train(int idx) : Behaviour(idx) {

}

void Train::applyLayer1(const nlohmann::json &json) {
    this->cooldown = json.value("cooldown", this->cooldown);
    this->fuel = json.value("fuel", this->fuel);
    this->fuel_capacity = json.value("fuel_capacity", this->fuel_capacity);
    this->fuel_consumption = json.value("fuel_consumption", this->fuel_consumption);
    this->goods = json.value("goods", this->goods);
    this->goods_capacity = json.value("goods_capacity", this->goods_capacity);

    this->level = json.value("level", this->level);

    /// this->next_level_price = json.value("next_level_price", this->next_level_price);
    if (json.contains("next_level_price")) {
        this->next_level_price = json["next_level_price"].is_null() ? 0 : json["next_level_price"].get<int>();
    }


    //applying order is important
    this->prevPosition = this->position;
    this->position = json.value("position", this->position);
    if (json.contains("line_idx")) {
        this->prevLine = this->line != nullptr ? this->line : Database::lines[json["line_idx"]];
        this->line = Database::lines[json["line_idx"]];
        Vector2 prevPosition = this->prevLine->toWorldGlobalPosition(this->prevPosition);
        Vector2 nextPosition = this->line->toWorldGlobalPosition(this->position);
        Vector2 direction = nextPosition - prevPosition;
        this->transform->setRotation(180 / PI * atan2f(direction.y, direction.x));
        if (this->cooldown) {
            this->transform->setPosition(nextPosition);
        } else {
            this->transform->setPosition(prevPosition);
        }
        this->transform->setParent(this->line->transform, true);
        //this->targetPosition = this->line->toWorldLocalPosition(this->position);
    }
    this->speed = json.value("speed", this->speed);
    this->player_idx = json.value("player_idx", this->player_idx);

    //TODO: ask what this field store
    //"goods_type": null, WTF?
}

void Train::update() {
    this->targetPosition = this->line->toWorldLocalPosition(this->position);
    this->transform->setLocalPosition(lerp(
            this->transform->getLocalPosition(),targetPosition, this->animationSpeed*Time::deltaTime));
}

Vector2 Train::lerp(const Vector2 & a, const Vector2 & b, float t) {
    return a + (b-a)*t;
}
