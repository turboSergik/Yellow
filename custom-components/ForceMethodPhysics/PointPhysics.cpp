//
// Created by Олег Бобров on 19.01.2020.
//

#include "PointPhysics.hpp"
#include "../../network/Network.hpp"

void PointPhysics::start() {
    if (!point) {
        point = gameObject->getComponent<Point>();
    }
    Network::onMapResponse10.addListener<PointPhysics, &PointPhysics::onMapLayer10>(this);
}

void PointPhysics::fixedUpdate() {
    Vector2 direction = -point->rigidBody->velocity;
    point->rigidBody->addForce(
            direction.normalized() *
            (ForceMethodConfig::frictionK * direction.sqrMagnitude() +
            ForceMethodConfig::dampK * sqrtf(direction.magnitude())));
    //rigidBody->addForce(direction.normalized() *(frictionK * direction.sqrMagnitude()));
}

void PointPhysics::onDestroy() {
    Network::onMapResponse10.removeListener<PointPhysics, &PointPhysics::onMapLayer10>(this);
}

void PointPhysics::onMapLayer10(const nlohmann::json &json) {
    destroy();
}