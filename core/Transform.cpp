//
// Created by Олег Бобров on 17.11.2019.
//

#include "Transform.h"
#include "GameObject.h"
#include "../linalg/Vector2.hpp"

const Vector2 &Transform::getPosition() const {
    return position;
}

void Transform::setPosition(const Vector2 & position) {
    if (this->parent) {
        this->setLocalPosition(this->parent->toLocalPosition(position));
    } else {
        this->setLocalPosition(position);
    }
}

const Vector2 &Transform::getLocalPosition() const {
    return this->localPosition;
}

void Transform::setLocalPosition(const Vector2 &localPosition) {
    this->localPosition = localPosition;
    recalculateAccordingToLocals();
}

float Transform::getRotation() const {
    return this->rotation;
}

void Transform::setRotation(float rotation) {
    if (this->parent) {
        this->setLocalRotation(rotation - this->parent->rotation);
    } else {
        this->setLocalRotation(rotation);
    }
}

float Transform::getLocalRotation() const {
    return this->localRotation;
}

void Transform::setLocalRotation(float localRotation) {
    this->localRotation = localRotation;
    this->recalculateAccordingToLocals();
}

const Vector2 &Transform::getScale() const {
    return this->scale;
}

void Transform::setScale(const Vector2 &scale) {
    this->scale = scale;
    if (this->parent) {
        this->localScale = {scale.x / this->parent->scale.x, scale.y / this->parent->scale.y};
    } else {
        this->localScale = scale;
    }
}

const Vector2 &Transform::getLocalScale() const {
    return this->localScale;
}

void Transform::setLocalScale(const Vector2 &localScale) {
    this->localScale = localScale;
    recalculateAccordingToLocals();
}

void Transform::setPositionAndRotation(const Vector2 &position, float rotation) {
    if (this->parent) {
        this->localPosition = this->parent->toLocalPosition(position);
        this->localRotation = rotation - this->parent->rotation;
    } else {
        this->localPosition = position;
        this->localRotation = rotation;
    }
    recalculateAccordingToLocals();
}


Transform *Transform::getParent() const {
    return this->parent;
}

void Transform::setParent(Transform *parent, bool worldPositionStays) {
    if (this->parent) {
        this->parent->children.erase(this->it);
    }
    this->parent = parent;
    if (worldPositionStays) {
        setPositionAndRotation(this->position, this->rotation);
        if (this->parent) {
            this->parent->children.push_back(this);
            this->it = std::prev(parent->children.end());
        }
    } else {
        recalculateAccordingToLocals();
        if (this->parent) {
            this->parent->children.push_back(this);
            this->it = std::prev(parent->children.end());
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
    right = toGlobalDirection(Vector2::RIGHT);
    up = toGlobalDirection(Vector2::UP);
    for (auto child : children) {
        child->recalculateAccordingToLocals();
    }
}

Vector2 Transform::toLocalPosition(const Vector2 & position) const {
    return this->worldToLocal.transformPoint(position);
}

Vector2 Transform::toLocalDirection(const Vector2 & direction) const {
    return sf::Transform(sf::Transform::Identity).rotate(-rotation).transformPoint(direction);
}

Vector2 Transform::toGlobalPosition(const Vector2 & position) const {
    return this->localToWorld.transformPoint(position);
}

Vector2 Transform::toGlobalDirection(const Vector2 & direction) const {
    return sf::Transform(sf::Transform::Identity).rotate(rotation).transformPoint(direction);
}

const Vector2 &Transform::getUp() const {
    return this->up;
}

const Vector2 &Transform::getRight() const {
    return this->right;
}

std::list<Transform *>::iterator Transform::begin() { return children.begin(); }

std::list<Transform *>::iterator Transform::end() { return children.end(); }

Transform::~Transform() {
    for (Transform * child : this->children) {
        delete child->gameObject;
    }
}

Transform::Transform(GameObject *gameObject) {
    this->gameObject = gameObject;
}

const sf::Transform &Transform::getLocalToWorldTransform() const {
    return this->localToWorld;
}

const sf::Transform &Transform::getWorldToLocalTransform() const {
    return this->worldToLocal;
}

