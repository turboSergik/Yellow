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
    sf::Vector2f centerOfMass = {0, 0};
    sf::Vector2f worldCenterOfMass = {0, 0};
    sf::Vector2f velocity = {0, 0};
    sf::Vector2f acceleration = {0, 0};
    float angularVelocity = 0;
    float angularAcceleration = 0;

    sf::Vector2f normalized(const sf::Vector2f & v);
    float dot (const sf::Vector2f & v1, const sf::Vector2f & v2);
    void update();
    void addForce(sf::Vector2f force);
    void addForceAtPosition(sf::Vector2f force, sf::Vector2f position);
//    sf::Vector2f getRelativePointVelocity(const sf::Vector2f & point) {
//        return velocity + transform.transformDirection(
//                Matrix3x3.Rotate(90).dot(point).mul(Math.toRadians(angularVelocity))));
//    }
};


#endif //WG_RIGIDBODY_HPP
