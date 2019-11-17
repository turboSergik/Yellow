//
// Created by Олег Бобров on 15.11.2019.
//

#include "Line.h"


Line::Line(int idx) {
    this->idx = idx;
}

void Line::applyLayer0(const nlohmann::json &json) {
    this->length = json["length"];
}
