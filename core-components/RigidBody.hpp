//
// Created by Олег Бобров on 28.12.2019.
//

#ifndef WG_RIGIDBODY_HPP
#define WG_RIGIDBODY_HPP


#include <SFML/System/Vector2.hpp>
#include "../core/Component.h"
#include "../static/Time.h"

class RigidBody : public Component {
public:
    float mass = 1;
    float drag = 0.05;
    lng::Vector2 centerOfMass = {0, 0};
    lng::Vector2 worldCenterOfMass = {0, 0};
    lng::Vector2 velocity = {0, 0};
    lng::Vector2 acceleration = {0, 0};
    float angularVelocity = 0;
    float angularAcceleration = 0;

    void update();
    void addForce(lng::Vector2 force);
    void addForceAtPosition(lng::Vector2 force, lng::Vector2 position);
//    lng::Vector2 getRelativePointVelocity(const lng::Vector2 & point) {
//        return velocity + transform.transformDirection(
//                Matrix3x3.Rotate(90).dot(point).mul(Math.toRadians(angularVelocity))));
//    }
};


#endif //WG_RIGIDBODY_HPP
