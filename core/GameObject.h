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
    Transform * transform = nullptr; //TODO: prevent changes of this field
    GameObject();
    ~GameObject();
    void update();
    void draw();

    template <class T>
    T * addComponent(const T *component);

    template <class T, class... Args>
    T * addComponent(Args &&... args);
    
    template <class T>
    T * getComponent();
};

template<class T, class... Args>
T * GameObject::addComponent(Args &&... args) {
    T *instance = new T(std::forward<Args>(args)...);
    components.push_back(instance);
    instance->gameObject = this;
    instance->transform = GameObject::transform;
    return instance;
}

template<class T>
T * GameObject::addComponent(const T *component) {
    components.push_back(component);
    component->gameObject = this;
    component->transform = GameObject::transform;
    return component;
}

template<class T>
T * GameObject::getComponent() {
    T * result;
    for (Component * component : components) {
        result = dynamic_cast<T *>(component);
        if (result) {
            return result;
        }
    }
}

#endif //WG_GAMEOBJECT_H
