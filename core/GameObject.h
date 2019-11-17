//
// Created by Олег Бобров on 17.11.2019.
//

#ifndef WG_GAMEOBJECT_H
#define WG_GAMEOBJECT_H


#include "Transform.h"
#include "Component.h"
#include <utility>

class GameObject {
private:
    std::list<Component*> components;
public:
    Transform *transform; //TODO: prevent changes of this field
    GameObject();
    ~GameObject();
    void update();
    void draw();

    template <class T>
    T* addComponent(const T *component);

    template<class T, class... Args>
    T *addComponent(Args &&... args);
};

template<class T, class... Args>
T *GameObject::addComponent(Args &&... args) {
    T *instance = new T(std::forward<Args>(args)...);
    components.push_back(instance);
    return instance;
}

template<class T>
T *GameObject::addComponent(const T *component) {
    components.push_back(component);
    return component;
}

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
    for (auto child : *transform) {
        delete child->gameObject;
    }
    delete transform;
}


#endif //WG_GAMEOBJECT_H
