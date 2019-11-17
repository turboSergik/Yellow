//
// Created by Олег Бобров on 15.11.2019.
//

#include "Point.h"
#include "../static/Database.h"

void Point::applyLayer10(const nlohmann::json &json) {
    transform->setLocalPosition({json["x"], json["y"]});
}

void Point::applyLayer0(const nlohmann::json &json) {
    auto& post_idx = json["post_idx"];
    Point::post = post_idx == nullptr ? nullptr : Database::posts[post_idx];
}

Point::Point(int idx) : Behaviour(idx) {

}
