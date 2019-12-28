//
// Created by Олег Бобров on 28.12.2019.
//

#include "RigidBody.hpp"

void RigidBody::addForceAtPosition(sf::Vector2f force, sf::Vector2f position) {
    sf::Vector2f rVector = worldCenterOfMass - position;
    sf::Vector2f rForce = normalized(rVector) * dot(normalized(rVector), force);
    sf::Vector2f iForce = force - rForce;
    addForce(rForce);

    //TODO: apply moment of inertia
//        angularAcceleration += Math.toDegrees(
//                3 * Math.signum(iForce.cross(rVector)) * iForce.magnitude() * rVector.magnitude() / mass / 2 / 10225);

}

sf::Vector2f RigidBody::normalized(const sf::Vector2f &v) {
    float m = v.x*v.x + v.y*v.y;
    return v/m;
}

float RigidBody::dot(const sf::Vector2f &v1, const sf::Vector2f &v2) {
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

void RigidBody::addForce(sf::Vector2f force) {
    acceleration += force/mass;
}
