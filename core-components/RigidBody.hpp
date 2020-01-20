//
// Created by Олег Бобров on 28.12.2019.
//

#ifndef WG_RIGIDBODY_HPP
#define WG_RIGIDBODY_HPP


#include <SFML/System/Vector2.hpp>
#include "../core/Component.h"
#include "../static/Time.h"
#include "../linalg/Vector2.hpp"

class RigidBody : public Component {
private:
    bool kinematic = false;
public:
    float mass = 1;
    float drag = 0.05;
    Vector2 centerOfMass = {0, 0};
    Vector2 worldCenterOfMass = {0, 0};
    Vector2 velocity = {0, 0};
    Vector2 acceleration = {0, 0};
    float angularVelocity = 0;
    float angularAcceleration = 0;

    bool isKinematic();
    void setKinematic(bool state = true);
    void fixedUpdate();
    void addForce(Vector2 force);
    void addForceAtPosition(Vector2 force, Vector2 position);
//    lng::Vector2 getRelativePointVelocity(const lng::Vector2 & point) {
//        return velocity + transform.transformDirection(
//                Matrix3x3.Rotate(90).dot(point).mul(Math.toRadians(angularVelocity))));
//    }
};


#endif //WG_RIGIDBODY_HPP
