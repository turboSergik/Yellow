//
// Created by Олег Бобров on 19.01.2020.
//

#include "Draggable.hpp"
#include "PointPhysics.hpp"
#include "LinePhysics.hpp"
#include "../../network/Network.hpp"

void Draggable::start() {
    Network::onMapResponse10.addListener<Draggable, &Draggable::onMapLayer10>(this);
}

void Draggable::update() {
//    Vector2 worldPoint = Camera::mainCamera->screenToWorldPoint(Input::mousePosition);

//    if (Input::getMouseButtonPressed(sf::Mouse::Button::Left)) {
//        Collider * collider = Collider::overlapPoint(worldPoint);
//        if (collider) {
//            draggingCollider = collider;
//            RigidBody * rigidBody = collider->gameObject->getComponent<RigidBody>();
//            rigidBody->setKinematic(true);
//        }
//    }
//    if (draggingCollider) {
//        draggingCollider->transform->setPosition(worldPoint);
//    }
//    if (Input::getMouseButtonReleased(sf::Mouse::Button::Left) && draggingCollider) {
//        RigidBody * rigidBody = draggingCollider->gameObject->getComponent<RigidBody>();
//        rigidBody->setKinematic(false);
//        draggingCollider = nullptr;
//    }
}

void Draggable::onDestroy() {
    Network::onMapResponse10.removeListener<Draggable, &Draggable::onMapLayer10>(this);
}

void Draggable::onMapLayer10(const nlohmann::json &json) {
    destroy();
}
