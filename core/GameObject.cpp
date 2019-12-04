//
// Created by Олег Бобров on 17.11.2019.
//

#include "GameObject.h"

//void GameObject::update() {
//    for (UpdateWrapper & method : updatePool) {
//        method();
//    }
//    for (Transform * child : *transform) {
//        child->gameObject->update();
//    }
//}

GameObject * GameObject::instantiate() {
    for (StartWrapper & method : startPool) {
        MethodsPool::startPool.push_back(std::move(method));
    }
    startPool.clear();
    for (UpdateWrapper & method : updatePool) {
        reinterpret_cast<Component *>(method.getObject())->updatePosition = 
                MethodsPool::updatePool.size();
        MethodsPool::updatePool.push_back(method);
    }
    onScene = true;
    return this;
}

GameObject * GameObject::instantiate(Transform * parent) {
    GameObject::instantiate();
    GameObject::transform->setParent(parent);
    return this;
}


void GameObject::destroyImmediate() {
    if (transform->parent) {
        transform->parent->children.erase(transform->it);
    }
    if (this->onScene) {
        sceneDestroy();
    }
    delete this;
}

void GameObject::destroy() {
    MethodsPool::destroyObjectPool.push_back(this);
}

void GameObject::sceneDestroy() {
    for (UpdateWrapper & method : this->updatePool) {
        MethodsPool::removeFromUpdate(reinterpret_cast<Component *>(method.getObject())->updatePosition);
    }
    for (OnDestroyWrapper & method : this->onDestroyPool) {
        method();
    }
    for (Transform * child : *transform) {
        child->gameObject->sceneDestroy();
    }
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

