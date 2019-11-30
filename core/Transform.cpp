//
// Created by Олег Бобров on 17.11.2019.
//

#include "Transform.h"
#include "GameObject.h"

const sf::Vector2f &Transform::getPosition() const {
    return position;
}

void Transform::setPosition(const sf::Vector2f &position) {
    if (parent) {
        Transform::setLocalPosition(parent->toLocalPosition(position));
    } else {
        Transform::setLocalPosition(position);
    }
}

const sf::Vector2f &Transform::getLocalPosition() const {
    return localPosition;
}

void Transform::setLocalPosition(const sf::Vector2f &localPosition) {
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

const sf::Vector2f &Transform::getScale() const {
    return scale;
}

void Transform::setScale(const sf::Vector2f &scale) {
    Transform::scale = scale;
    if (parent) {
        Transform::localScale = {scale.x / parent->scale.x, scale.y / parent->scale.y};
    } else {
        Transform::localScale = scale;
    }
}

const sf::Vector2f &Transform::getLocalScale() const {
    return localScale;
}

void Transform::setLocalScale(const sf::Vector2f &localScale) {
    Transform::localScale = localScale;
}

void Transform::setPositionAndRotation(const sf::Vector2f &position, float rotation) {
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

sf::Vector2f Transform::toLocalPosition(const sf::Vector2f &position) const {
    return worldToLocal.transformPoint(position);
}

sf::Vector2f Transform::toLocalDirection(const sf::Vector2f &direction) const {
    return sf::Transform(sf::Transform::Identity).rotate(-rotation).transformPoint(direction);
}

sf::Vector2f Transform::toGlobalPosition(const sf::Vector2f &position) const {
    return localToWorld.transformPoint(position);
}

sf::Vector2f Transform::toGlobalDirection(const sf::Vector2f &direction) const {
    return sf::Transform(sf::Transform::Identity).rotate(rotation).transformPoint(direction);
}

const sf::Vector2f &Transform::getUp() const {
    return up;
}

const sf::Vector2f &Transform::getRight() const {
    return right;
}

std::list<Transform *>::iterator Transform::begin() { return children.begin(); }

std::list<Transform *>::iterator Transform::end() { return children.end(); }

Transform::~Transform() {
    if (parent) {
        parent->children.erase(it);
    }
    // children destructor will remove itself from list
    while (!children.empty()) {
        delete children.front();
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

