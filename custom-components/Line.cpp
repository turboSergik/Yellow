//
// Created by Олег Бобров on 15.11.2019.
//

#include "Line.h"
#include "../static/Database.h"
#include "../core/GameObject.h"

Line::Line(int idx) : Behaviour(idx) {

}

void Line::applyLayer0(const nlohmann::json &json) {
    Line::length = json["length"];
    auto & item_points = json["points"];
    Line::points[0] = Database::points[item_points[0]];
    Line::points[1] = Database::points[item_points[1]];
}

void Line::update() {
    if (!lineRenderer) {
        lineRenderer = gameObject->getComponent<LineRenderer>();
    }
    lineRenderer->setVertices(
            points[0]->transform->getLocalPosition(),
            points[1]->transform->getLocalPosition());
}




