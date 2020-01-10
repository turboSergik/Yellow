//
// Created by Олег Бобров on 28.12.2019.
//

#include "RigidBody.hpp"
#include "../utility/Mathf.hpp"

void RigidBody::addForceAtPosition(Vector2 force, Vector2 position) {
    Vector2 rVector = worldCenterOfMass - position;
    Vector2 rForce = rVector.normalized() * Vector2::dot(rVector.normalized(), force);
    Vector2 iForce = force - rForce;
    addForce(rForce);

    //TODO: apply moment of inertia
//        angularAcceleration += Math.toDegrees(
//                3 * Math.signum(iForce.cross(rVector)) * iForce.magnitude() * rVector.magnitude() / mass / 2 / 10225);

}

void RigidBody::fixedUpdate() {
    if (this->kinematic) return;
    worldCenterOfMass = transform->toGlobalPosition(centerOfMass);
    
    //translate
    transform->setPosition(transform->getPosition() + velocity * Time::fixedDeltaTime +
                           acceleration * Time::fixedDeltaTime * Time::fixedDeltaTime / 2.f);
    velocity += acceleration * Time::fixedDeltaTime;
    float k = Mathf::clamp(1.f - drag*Time::fixedDeltaTime);
    velocity *= k;
    //rotate
    transform->setRotation(transform->getRotation() + angularVelocity * Time::fixedDeltaTime +
                           angularAcceleration * Time::fixedDeltaTime * Time::fixedDeltaTime / 2.f);
    angularVelocity += angularAcceleration * Time::fixedDeltaTime;
    //update
    acceleration = {0, 0};
    angularAcceleration = 0;
}

void RigidBody::addForce(Vector2 force) {
    if (this->kinematic) return;
    acceleration += force/mass;
}

bool RigidBody::isKinematic() {
    return this->kinematic;
}

void RigidBody::setKinematic(bool state) {
    this->kinematic = state;
    this->velocity = this->acceleration = Vector2::ZERO;
    this->angularVelocity = this->angularAcceleration = 0.f;
}
