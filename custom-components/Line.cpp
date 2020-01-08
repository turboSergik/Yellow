//
// Created by Олег Бобров on 15.11.2019.
//

#include <iostream>
#include "Line.h"
#include "../static/Database.h"
#include "../core/GameObject.h"


const float PI = 3.14159265358f;

Line::Line(int idx) : Behaviour(idx) {

}

void Line::applyLayer0(const nlohmann::json &json) {
    this->length = json.value("length", this->length);
    if (json.contains("points")) {
        auto &item_points = json["points"];
        //TODO: check null array
        this->points[0] = Database::points[item_points[0]];
        this->points[1] = Database::points[item_points[1]];
        this->transform->setParent(this->points[0]->transform);
        this->points[0]->adjacent.emplace_back(this->points[1], this);
        this->points[1]->adjacent.emplace_back(this->points[0], this);
    }
}

void Line::start() {
    lineRenderer = gameObject->getComponent<LineRenderer>();
}

void Line::fixedUpdate() {
    Vector2 direction =
            this->points[1]->transform->getPosition() -
            this->points[0]->transform->getPosition();
    this->worldLength = direction.magnitude();
    this->transform->setRotation(180 / PI * atan2f(direction.y, direction.x));
    this->lineRenderer->setVertices({0, 0}, {this->worldLength, 0});
    float deltaLength = this->worldLength - ForceMethodConfig::springLength;
    this->points[0]->rigidBody->addForce(
            ForceMethodConfig::stiffnessK * deltaLength * direction.normalized());
    this->points[1]->rigidBody->addForce(
            -ForceMethodConfig::stiffnessK * deltaLength * direction.normalized());
}

Vector2 Line::toWorldGlobalPosition(int position) {
    return this->transform->toGlobalPosition(
            {this->worldLength * position / this->length, 0});
}

Vector2 Line::toWorldLocalPosition(int position) {
    return {this->worldLength * position / this->length, 0};
}




