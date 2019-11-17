//
// Created by Олег Бобров on 17.11.2019.
//

#ifndef WG_GAMEOBJECT_H
#define WG_GAMEOBJECT_H


#include "Transform.h"
#include "Component.h"

class GameObject {
private:
    std::list<Component*> components;
public:
    Transform transform;

    void update();
    void draw();

    template <class T>
    T* addComponent(const T *component);

    template<class T>
    T *addComponent();
};

template<class T>
T *GameObject::addComponent() {
    T *instance = new T();
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
    for (auto child : transform) {
        child->gameObject->update();
    }
}


#endif //WG_GAMEOBJECT_H
