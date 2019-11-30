//
// Created by Олег Бобров on 17.11.2019.
//

#include "GameObject.h"

void GameObject::update() {
    for (UpdateWrapper & method : updatePool) {
        method();
    }
    for (Transform * child : *transform) {
        child->gameObject->update();
    }
//    for (Component * component : components) {
//        component->update();
//    }

}

void GameObject::instantiate() {
    for (StartWrapper & method : startPool) {
        MethodsPool::startPool.push_front(std::move(method));
    }
    startPool.clear();
    for (UpdateWrapper & method : updatePool) {
        reinterpret_cast<Component *>(method.getObject())->updatePosition = 
                MethodsPool::updatePool.size();
        MethodsPool::updatePool.push_back(method);
    }
    onScene = true;
}

void GameObject::destroy(GameObject * gameObject) {
    if (gameObject->onScene) {
        for (UpdateWrapper & method : gameObject->updatePool) {
            MethodsPool::removeFromUpdate(reinterpret_cast<Component *>(method.getObject())->updatePosition);
        }
        for (OnDestroyWrapper & method : gameObject->onDestroyPool) {
            method();
        }
    }
    delete gameObject;
}

GameObject::GameObject() {
    transform = new Transform(this);
}

GameObject::~GameObject() {
    for (Component * component : components) {
        delete component;
    }        
    delete transform;
}

