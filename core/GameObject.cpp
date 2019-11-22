//
// Created by Олег Бобров on 17.11.2019.
//

#include "GameObject.h"

void GameObject::update() {
    for (auto component : components) {
        component->update();
    }
    for (auto child : *transform) {
        child->gameObject->update();
    }
}

GameObject::GameObject() {
    transform = new Transform(this);
}

GameObject::~GameObject() {
    for (auto component : components) {
        delete component;
    }
    delete transform;
}

