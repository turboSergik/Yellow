//
// Created by Олег Бобров on 15.11.2019.
//

#include "Point.h"
#include "../static/Database.h"

void Point::applyLayer10(const nlohmann::json &json) {
    float x = 0, y = 0;
    json.tryGetValue("x", x);
    json.tryGetValue("y", y);
    transform->setLocalPosition({x, y});
}

void Point::applyLayer0(const nlohmann::json &json) {
    nlohmann::json post_idx;
    json.tryGetValue("post_idx", post_idx);
    Point::post = post_idx == nullptr ? nullptr : Database::posts[post_idx];
}

Point::Point(int idx) : Behaviour(idx) {

}
