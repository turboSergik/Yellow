//
// Created by Олег Бобров on 17.11.2019.
//

#include "GameObject.h"

void GameObject::update() {
//    for (Component * component : components) {
//        component->update();
//    }
//    for (Transform * child : *transform) {
//        child->gameObject->update();
//    }
}

void GameObject::instantiate() {
    for (MethodWrapper & method : awakePool) {
        method();
    }
    MethodsPool::startPool.insert(
                MethodsPool::startPool.begin(), 
                this->startPool.begin(),
                this->startPool.end());
    MethodsPool::updatePool.insert(
                MethodsPool::updatePool.begin(), 
                this->updatePool.begin(),
                this->updatePool.end());
    onScene = true;
}

GameObject::GameObject() {
    transform = new Transform(this);
}

GameObject::~GameObject() {
    if (onScene) {
        for (Component * component : components) {
            MethodsPool::updatePool.erase(component->updatePosition);
            delete component;
        }
    } else {
        for (Component * component : components) {
            delete component;
        }        
    }
    
    delete transform;
}

