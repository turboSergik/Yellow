//
// Created by Олег Бобров on 15.11.2019.
//

#include "Line.h"
#include "../static/Database.h"
#include "../core/GameObject.h"

Line::Line(int idx) : Behaviour(idx) {

}

void Line::applyLayer0(const nlohmann::json &json) {
    Line::length = json.value("length", Line::length);
    if (json.contains("points")) {
        auto &item_points = json["points"];
        //TODO: check null array
        Line::points[0] = Database::points[item_points[0]];
        Line::points[1] = Database::points[item_points[1]];
        Line::transform->setLocalPosition(Line::points[0]->transform->getLocalPosition());
        Line::points[0]->adjacent.emplace_back(Line::points[1], this);
        Line::points[1]->adjacent.emplace_back(Line::points[0], this);
    }
}

void Line::update() {
    if (!lineRenderer) {
        lineRenderer = gameObject->getComponent<LineRenderer>();
    }
    Line::transform->setLocalPosition(Line::points[0]->transform->getLocalPosition());
    lineRenderer->setVertices({0, 0},
            transform->toLocalPosition(points[1]->transform->getPosition()));
}




