//
// Created by Олег Бобров on 15.11.2019.
//

#include "Point.h"

Point::Point(int idx) {
    this->idx = idx;
}

void Point::applyLayer10(const nlohmann::json &json) {
    transform->setLocalPosition({json["x"], json["y"]});
}
