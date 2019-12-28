//
// Created by Олег Бобров on 28.12.2019.
//

#include "RigidBody.hpp"

void RigidBody::addForceAtPosition(lng::Vector2 force, lng::Vector2 position) {
    lng::Vector2 rVector = worldCenterOfMass - position;
    lng::Vector2 rForce = normalized(rVector) * dot(normalized(rVector), force);
    lng::Vector2 iForce = force - rForce;
    addForce(rForce);

    //TODO: apply moment of inertia
//        angularAcceleration += Math.toDegrees(
//                3 * Math.signum(iForce.cross(rVector)) * iForce.magnitude() * rVector.magnitude() / mass / 2 / 10225);

}

lng::Vector2 RigidBody::normalized(const lng::Vector2 &v) {
    float m = v.x*v.x + v.y*v.y;
    return v/m;
}

float RigidBody::dot(const lng::Vector2 &v1, const lng::Vector2 &v2) {
    return v1.x*v2.x + v1.y*v2.y;
}

void RigidBody::update() {
    worldCenterOfMass = transform->toGlobalPosition(centerOfMass);
    
    //translate
    transform->setPosition(transform->getPosition() + velocity * Time::deltaTime +
                           acceleration * Time::deltaTime * Time::deltaTime / 2.f);
    velocity = velocity + acceleration * Time::deltaTime;
    //rotate
    transform->setRotation(transform->getRotation() + angularVelocity * Time::deltaTime +
                           angularAcceleration * Time::deltaTime * Time::deltaTime / 2);
    angularVelocity += angularAcceleration * Time::deltaTime;
    //update
    acceleration = {0, 0};
    angularAcceleration = 0;
}

void RigidBody::addForce(lng::Vector2 force) {
    acceleration += force/mass;
}
