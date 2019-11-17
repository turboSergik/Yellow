//
// Created by Олег Бобров on 15.11.2019.
//

#include "Point.h"

Point::Point(int idx) : CircleShape(10) {
    this->idx = idx;
    this->setOrigin(10, 10);
}

void Point::applyLayer10(const nlohmann::json &json) {
    this->setPosition({json["x"], json["y"]});
}
