//
// Created by Олег Бобров on 15.11.2019.
//

#include <iostream>
#include "Point.h"
#include "../static/Database.h"
#include "../core/GameObject.h"
#include "../utility/ForceMethodConfig.hpp"
#include "../linalg/Vector2.hpp"

void Point::applyLayer10(const nlohmann::json &json) {
    transform->setLocalPosition({json.value("x", 0.f),
                                 json.value("y", 0.f)});
}

void Point::applyLayer0(const nlohmann::json &json) {
    adjacent.clear();
    if (json.contains("post_idx")) {
        auto & post_idx = json["post_idx"];
        Point::post = post_idx == nullptr ? nullptr : Database::posts[post_idx];
    }
}

Point::Point(int idx) : Behaviour(idx) {

}

void Point::start() {
    rigidBody = gameObject->getComponent<RigidBody>();
}

void Point::update() {
    Vector2 direction = -rigidBody->velocity;
    rigidBody->addForce(direction.normalized() * (ForceMethodConfig::frictionK
    * direction.sqrMagnitude() + ForceMethodConfig::dampK * sqrtf(direction.magnitude())));
    //rigidBody->addForce(direction.normalized() *(frictionK * direction.sqrMagnitude()));
}
