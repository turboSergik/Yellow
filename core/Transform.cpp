//
// Created by Олег Бобров on 17.11.2019.
//

#include "Transform.h"
#include "GameObject.h"
#include "../linalg/Vector2.hpp"

const Vector2 &Transform::getPosition() const {
    return position;
}

void Transform::setPosition(const Vector2 &position) {
    if (parent) {
        Transform::setLocalPosition(parent->toLocalPosition(position));
    } else {
        Transform::setLocalPosition(position);
    }
}

const Vector2 &Transform::getLocalPosition() const {
    return localPosition;
}

void Transform::setLocalPosition(const Vector2 &localPosition) {
    Transform::localPosition = localPosition;
    recalculateAccordingToLocals();
}

float Transform::getRotation() const {
    return rotation;
}

void Transform::setRotation(float rotation) {
    if (parent) {
        Transform::setLocalRotation(rotation - parent->rotation);
    } else {
        Transform::setLocalRotation(rotation);
    }
}

float Transform::getLocalRotation() const {
    return localRotation;
}

void Transform::setLocalRotation(float localRotation) {
    Transform::localRotation = localRotation;
}

const Vector2 &Transform::getScale() const {
    return scale;
}

void Transform::setScale(const Vector2 &scale) {
    Transform::scale = scale;
    if (parent) {
        Transform::localScale = {scale.x / parent->scale.x, scale.y / parent->scale.y};
    } else {
        Transform::localScale = scale;
    }
}

const Vector2 &Transform::getLocalScale() const {
    return localScale;
}

void Transform::setLocalScale(const Vector2 &localScale) {
    Transform::localScale = localScale;
}

void Transform::setPositionAndRotation(const Vector2 &position, float rotation) {
    if (parent) {
        Transform::localPosition = parent->toLocalPosition(position);
        Transform::localRotation = rotation - parent->rotation;
    } else {
        Transform::localPosition = position;
        Transform::localRotation = rotation;
    }
    recalculateAccordingToLocals();
}


Transform *Transform::getParent() const {
    return parent;
}

void Transform::setParent(Transform *parent, bool worldPositionStays) {
    if (Transform::parent) {
        Transform::parent->children.erase(it);
    }
    Transform::parent = parent;
    if (worldPositionStays) {
        setPositionAndRotation(Transform::position, Transform::rotation);
        if (Transform::parent) {
            Transform::parent->children.push_back(this);
            it = std::prev(parent->children.end());
        }
    } else {
        recalculateAccordingToLocals();
        if (Transform::parent) {
            Transform::parent->children.push_back(this);
            it = std::prev(parent->children.end());
        }
    }

}

void Transform::recalculateAccordingToLocals() {
    localToWorld = worldToLocal = sf::Transform::Identity;
    localToWorld
            .translate(localPosition)
            .rotate(localRotation)
            .scale(localScale);
    worldToLocal
            .scale(1.f/localScale.x, 1.f/localScale.y)
            .rotate(-localRotation)
            .translate(-localPosition);
    if (parent) {
        localToWorld = parent->localToWorld * localToWorld;
        worldToLocal = worldToLocal * parent->worldToLocal;
        position = parent->toGlobalPosition(localPosition);
        rotation = parent->rotation + localRotation;
        scale = {parent->scale.x * localScale.x, parent->scale.y * localScale.y};
    }
    else {
        position = localPosition;
        rotation = localRotation;
        scale = localScale;
    }
    right = toGlobalDirection({1, 0});
    up = toGlobalDirection({0, 1});
    for (auto child : children) {
        child->recalculateAccordingToLocals();
    }
}

Vector2 Transform::toLocalPosition(const Vector2 &position) const {
    return worldToLocal.transformPoint(position);
}

Vector2 Transform::toLocalDirection(const Vector2 &direction) const {
    return sf::Transform(sf::Transform::Identity).rotate(-rotation).transformPoint(direction);
}

Vector2 Transform::toGlobalPosition(const Vector2 &position) const {
    return localToWorld.transformPoint(position);
}

Vector2 Transform::toGlobalDirection(const Vector2 &direction) const {
    return sf::Transform(sf::Transform::Identity).rotate(rotation).transformPoint(direction);
}

const Vector2 &Transform::getUp() const {
    return up;
}

const Vector2 &Transform::getRight() const {
    return right;
}

std::list<Transform *>::iterator Transform::begin() { return children.begin(); }

std::list<Transform *>::iterator Transform::end() { return children.end(); }

Transform::~Transform() {
    for (Transform * child : children) {
        delete child->gameObject;
    }
}

Transform::Transform(GameObject *gameObject) {
    Transform::gameObject = gameObject;
}

const sf::Transform &Transform::getLocalToWorldTransform() const {
    return localToWorld;
}

const sf::Transform &Transform::getWorldToLocalTransform() const {
    return worldToLocal;
}

