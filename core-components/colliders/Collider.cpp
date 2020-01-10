//
// Created by Олег Бобров on 10.01.2020.
//

#include "Collider.hpp"

std::list<Collider*> Collider::colliders;

void Collider::start() {
    Collider::colliders.push_back(this);
    this->it = std::prev(Collider::colliders.end());
}

void Collider::onDestroy() {
    Collider::colliders.erase(it);
}

Collider *Collider::overlapPoint(const Vector2 &point) {
    return *std::find(Collider::colliders.begin(), Collider::colliders.end(),
            [&point](const Collider * collider) {
                return collider->isOverlapsPoint(point);
            });
}