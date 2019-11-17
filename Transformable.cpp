//
// Created by Олег Бобров on 17.11.2019.
//

#include "Transformable.h"

const sf::Vector2f &Transformable::getPosition() const {
    return position;
}

void Transformable::setPosition(const sf::Vector2f &position) {
    if (parent) {
        Transformable::setLocalPosition(parent->transformToLocalPosition(position));
    } else {
        Transformable::setLocalPosition(position);
    }
}

const sf::Vector2f &Transformable::getLocalPosition() const {
    return localPosition;
}

void Transformable::setLocalPosition(const sf::Vector2f &localPosition) {
    Transformable::localPosition = localPosition;
    recalculateAccordingToLocals();
}

float Transformable::getRotation() const {
    return rotation;
}

void Transformable::setRotation(float rotation) {
    if (parent) {
        Transformable::setLocalRotation(rotation - parent->rotation);
    } else {
        Transformable::setLocalRotation(rotation);
    }
}

float Transformable::getLocalRotation() const {
    return localRotation;
}

void Transformable::setLocalRotation(float localRotation) {
    Transformable::localRotation = localRotation;
}

const sf::Vector2f &Transformable::getScale() const {
    return scale;
}

void Transformable::setScale(const sf::Vector2f &scale) {
    Transformable::scale = scale;
    if (parent) {
        Transformable::localScale = {scale.x / parent->scale.x, scale.y / parent->scale.y};
    } else {
        Transformable::localScale = scale;
    }
}

const sf::Vector2f &Transformable::getLocalScale() const {
    return localScale;
}

void Transformable::setLocalScale(const sf::Vector2f &localScale) {
    Transformable::localScale = localScale;
}

void Transformable::setPositionAndRotation(const sf::Vector2f &position, float rotation) {
    if (parent) {
        Transformable::localPosition = parent->transformToLocalPosition(position);
        Transformable::localRotation = rotation - parent->rotation;
    } else {
        Transformable::localPosition = position;
        Transformable::localRotation = rotation;
    }
    recalculateAccordingToLocals();
}


Transformable *Transformable::getParent() const {
    return parent;
}

void Transformable::setParent(Transformable *parent, bool worldPositionStays) {
    Transformable::parent = parent;
    if (worldPositionStays) {
        setPositionAndRotation(Transformable::position, Transformable::rotation);
        if (Transformable::parent) {
            Transformable::parent->children.push_back(this);
        }
    } else {
        recalculateAccordingToLocals();
        if (Transformable::parent) {
            Transformable::parent->children.push_back(this);
        }
    }

}

void Transformable::recalculateAccordingToLocals() {
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
        position = parent->transformToGlobalPosition(localPosition);
        rotation = parent->rotation + localRotation;
        scale = {parent->scale.x * localScale.x, parent->scale.y * localScale.y};
    }
    else {
        position = localPosition;
        rotation = localRotation;
        scale = localScale;
    }
    right = transformToGlobalDirection({1, 0});
    up = transformToGlobalDirection({0, 1});
    for (auto child : children) {
        child->recalculateAccordingToLocals();
    }
}

sf::Vector2f Transformable::transformToLocalPosition(const sf::Vector2f &position) const {
    return worldToLocal.transformPoint(position);
}

sf::Vector2f Transformable::transformToLocalDirection(const sf::Vector2f &direction) const {
    return sf::Transform(sf::Transform::Identity).rotate(-rotation).transformPoint(direction);
}

sf::Vector2f Transformable::transformToGlobalPosition(const sf::Vector2f &position) const {
    return localToWorld.transformPoint(position);
}

sf::Vector2f Transformable::transformToGlobalDirection(const sf::Vector2f &direction) const {
    return sf::Transform(sf::Transform::Identity).rotate(rotation).transformPoint(direction);
}

const sf::Vector2f &Transformable::getUp() const {
    return up;
}

const sf::Vector2f &Transformable::getRight() const {
    return right;
}

