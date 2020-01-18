//
// Created by Олег Бобров on 10.01.2020.
//

#include "CircleCollider.hpp"

bool CircleCollider::isOverlapsPoint(const Vector2 & point) const {
    return this->transform->toLocalPosition(point).magnitude() <= radius;
}

float CircleCollider::getRadius() const {
    return this->radius;
}

void CircleCollider::setRadius(float radius) {
    this->radius = radius;
}
