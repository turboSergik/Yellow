//
// Created by Олег Бобров on 19.01.2020.
//

#include "ForceMethodPhysics.hpp"
#include "PointPhysics.hpp"
#include "LinePhysics.hpp"
#include "../../network/Network.hpp"

void ForceMethodPhysics::start() {
    Network::onMapResponse10.addListener<ForceMethodPhysics, &ForceMethodPhysics::onMapLayer10>(this);
}

void ForceMethodPhysics::update() {
    // Vector2 screenPoint = Input::mousePosition;
    Vector2 worldPoint = Camera::mainCamera->screenToWorldPoint(Input::mousePosition);

    if (Input::getMouseButtonPressed(sf::Mouse::Button::Left)) {
        Collider * collider = Collider::overlapPoint(worldPoint);
        if (collider) {
            draggingCollider = collider;
            RigidBody * rigidBody = collider->gameObject->getComponent<RigidBody>();
            rigidBody->setKinematic(true);
        }
    }
    if (draggingCollider) {
        draggingCollider->transform->setPosition(worldPoint);
    }
    if (Input::getMouseButtonReleased(sf::Mouse::Button::Left) && draggingCollider) {
        RigidBody * rigidBody = draggingCollider->gameObject->getComponent<RigidBody>();
        rigidBody->setKinematic(false);
        draggingCollider = nullptr;
    }
}

void ForceMethodPhysics::fixedUpdate() {
    for (auto it1 = Database::points.begin(); it1 != Database::points.end(); it1++) {
        const auto & point1 = it1->second;
        for (auto it2 = std::next(it1); it2 != Database::points.end(); it2++) {
            const auto &point2 = it2->second;
            Vector2 direction = point2->transform->getPosition() - point1->transform->getPosition();
            float k = direction.magnitude() != 0.f ? ForceMethodConfig::charge / direction.magnitude() : 0.f;
            //float k = direction.sqrMagnitude() != 0.f ? ForceMethodConfig::charge * ForceMethodConfig::charge / direction.sqrMagnitude() : 0.f;
            point2->rigidBody->addForce(direction.normalized() * k);
            point1->rigidBody->addForce(-direction.normalized() * k);
        }
    }
}

void ForceMethodPhysics::onDestroy() {
    Network::onMapResponse10.removeListener<ForceMethodPhysics, &ForceMethodPhysics::onMapLayer10>(this);
}

void ForceMethodPhysics::onMapLayer10(const nlohmann::json &json) {
    destroy();
}
