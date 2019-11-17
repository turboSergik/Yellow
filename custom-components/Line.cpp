//
// Created by Олег Бобров on 15.11.2019.
//

#include "Line.h"
#include "../static/Database.h"

Line::Line(int idx) : Behaviour(idx) {

}

void Line::applyLayer0(const nlohmann::json &json) {
    Line::length = json["length"];
    auto & item_points = json["points"];
    Line::points[0] = Database::points[item_points[0]];
    Line::points[1] = Database::points[item_points[1]];
}




