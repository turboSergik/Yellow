//
// Created by Олег Бобров on 28.12.2019.
//

#include "RigidBody.hpp"

void RigidBody::addForceAtPosition(Vector2 force, Vector2 position) {
    Vector2 rVector = worldCenterOfMass - position;
    Vector2 rForce = rVector.normalized() * Vector2::dot(rVector.normalized(), force);
    Vector2 iForce = force - rForce;
    addForce(rForce);

    //TODO: apply moment of inertia
//        angularAcceleration += Math.toDegrees(
//                3 * Math.signum(iForce.cross(rVector)) * iForce.magnitude() * rVector.magnitude() / mass / 2 / 10225);

}

void RigidBody::update() {
    worldCenterOfMass = transform->toGlobalPosition(centerOfMass);
    
    //translate
    transform->setPosition(transform->getPosition() + velocity * Time::deltaTime +
                           acceleration * Time::deltaTime * Time::deltaTime / 2.f);
    velocity += acceleration * Time::deltaTime;
    float k = 1.f - drag*Time::deltaTime;
    if (k > 1.f) k = 1.f;
    else if (k < 0.f) k = 0.f;
    velocity *= k;
    //rotate
    transform->setRotation(transform->getRotation() + angularVelocity * Time::deltaTime +
                           angularAcceleration * Time::deltaTime * Time::deltaTime / 2.f);
    angularVelocity += angularAcceleration * Time::deltaTime;
    //update
    acceleration = {0, 0};
    angularAcceleration = 0;
}

void RigidBody::addForce(Vector2 force) {
    acceleration += force/mass;
}
