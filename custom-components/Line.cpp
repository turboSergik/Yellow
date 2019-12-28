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

void Line::start() {
    lineRenderer = gameObject->getComponent<LineRenderer>();
}

void Line::update() {
    Line::transform->setLocalPosition(Line::points[0]->transform->getLocalPosition());
    lineRenderer->setVertices({0, 0},
            transform->toLocalPosition(points[1]->transform->getPosition()));

    sf::Vector2f direction = points[1]->transform->getPosition() - points[0]->transform->getPosition();
    float magnitude = sqrtf(direction.x*direction.x + direction.y*direction.y);
    direction /= magnitude;
    float deltaLength = magnitude - Line::springLength;
    points[0]->rigidBody->addForce(stiffnessK*deltaLength*direction);
    points[1]->rigidBody->addForce(-stiffnessK*deltaLength*direction);
}




