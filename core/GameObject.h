//
// Created by Олег Бобров on 17.11.2019.
//

#ifndef WG_GAMEOBJECT_H
#define WG_GAMEOBJECT_H


#include "Transform.h"
#include "Component.h"
#include <utility>
#include <cstdint>
#include "../utility/methodWrapper.hpp"
#include "../static/MethodsPool.hpp"
#include "../utility/InterfaceChecker.hpp"

class GameObject {
private:
    std::list<Component*> components;
    std::list<MethodWrapper> awakePool;
    std::list<MethodWrapper> startPool;
    std::list<MethodWrapper> updatePool;
    
    template <class T, class... Args>
    T * initComponent(Args &&... args);
    
    template <class T>
    typename std::enable_if<HasAwake<T>::value>::type
    addAwake(T * component);
    
    template <class T>
    typename std::enable_if<!HasAwake<T>::value>::type
    addAwake(T * component);

    template <class T>
    typename std::enable_if<HasStart<T>::value>::type
    addStart(T * component);

    template <class T>
    typename std::enable_if<!HasStart<T>::value>::type
    addStart(T * component);
    
    template <class T>
    typename std::enable_if<HasUpdate<T>::value>::type
    addUpdate(T * component);
    
    template <class T>
    typename std::enable_if<!HasUpdate<T>::value>::type
    addUpdate(T * component);
    
public:
    Transform * transform = nullptr; //TODO: prevent changes of this field
    GameObject();
    ~GameObject();
    void update();

//    template <class T>
//    T * addComponent(const T *component);

    template <class T, class... Args>
    T * addComponent(Args &&... args);
    
    template <class T>
    T * getComponent();
};

template <class T, class... Args>
T * GameObject::initComponent(Args &&... args) {
    T * instance = new T(std::forward<Args>(args)...);
    components.push_back(instance);
    instance->componentPosition = std::prev(components.end());
    instance->gameObject = this;
    instance->transform = GameObject::transform;
    return instance;
}

template <class T, class... Args>
T * GameObject::addComponent(Args &&... args) {
    T * instance = this->template initComponent<T>(std::forward<Args>(args)...);
    addAwake(instance);
    addStart(instance);
    addUpdate(instance);
    return instance;
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
    return nullptr;
}


template <class T>
typename std::enable_if<HasAwake<T>::value>::type
GameObject::addAwake(T * component) {
    awakePool.push_back(MethodWrapper(component, &T::awake));
}

template <class T>
typename std::enable_if<!HasAwake<T>::value>::type
GameObject::addAwake(T * ) {}

template <class T>
typename std::enable_if<HasStart<T>::value>::type 
GameObject::addStart(T * component) {
    startPool.push_back(MethodWrapper(component, &T::start));
}

template <class T>
typename std::enable_if<!HasStart<T>::value>::type 
GameObject::addStart(T * ) {}

template <class T>
typename std::enable_if<HasUpdate<T>::value>::type 
GameObject::addUpdate(T * component) {
    updatePool.push_back(MethodWrapper(component, &T::start));
}

template <class T>
typename std::enable_if<!HasUpdate<T>::value>::type 
GameObject::addUpdate(T * ) {}


//template<class T>
//T * GameObject::addComponent(const T *component) {
//    components.push_back(component);
//    component->gameObject = this;
//    component->transform = GameObject::transform;
//    return component;
//}


#endif //WG_GAMEOBJECT_H
