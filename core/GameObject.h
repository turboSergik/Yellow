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
    std::list<MethodWrapper> onDestroyPool;
    bool onScene = false;
    
    template <class T, class... Args>
    T * initComponent(Args &&... args);
    
    template <class T>
    typename std::enable_if<HasAwake<T>::value>::type
    addAwake(T * component);
    
    template <class T>
    typename std::enable_if<!HasAwake<T>::value>::type
    addAwake(T * ) {}

    template <class T>
    typename std::enable_if<HasStart<T>::value>::type
    addStart(T * component);

    template <class T>
    typename std::enable_if<!HasStart<T>::value>::type
    addStart(T * ) {}
    
    template <class T>
    typename std::enable_if<HasUpdate<T>::value>::type
    addUpdate(T * component);
    
    template <class T>
    typename std::enable_if<!HasUpdate<T>::value>::type
    addUpdate(T * ) {}
    
    template <class T>
    typename std::enable_if<HasOnDestroy<T>::value>::type
    addOnDestroy(T * component);
    
    template <class T>
    typename std::enable_if<!HasOnDestroy<T>::value>::type
    addOnDestroy(T * ) {}
    
    template <class T>
    typename std::enable_if<HasAwake<T>::value>::type
    onSceneAwake(T * component);
    
    template <class T>
    typename std::enable_if<!HasAwake<T>::value>::type
    onSceneAwake(T * ) {}

    template <class T>
    typename std::enable_if<HasStart<T>::value>::type
    onSceneStart(T * component);

    template <class T>
    typename std::enable_if<!HasStart<T>::value>::type
    onSceneStart(T * ) {}
    
    template <class T>
    typename std::enable_if<HasUpdate<T>::value>::type
    onSceneUpdate(T * component);
    
    template <class T>
    typename std::enable_if<!HasUpdate<T>::value>::type
    onSceneUpdate(T * ) {}
    
public:
    Transform * transform = nullptr; //TODO: prevent changes of this field
    GameObject();
    ~GameObject();
    void update();
    void instantiate();

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
    if (onScene) {
        onSceneAwake(instance);
        onSceneStart(instance);
        onSceneUpdate(instance);
    } else {
        addAwake(instance);
        addStart(instance);
        addUpdate(instance);
    }
    addOnDestroy(instance);
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
    awakePool.push_front(MethodWrapper(component, &T::awake));
}

template <class T>
typename std::enable_if<HasStart<T>::value>::type 
GameObject::addStart(T * component) {
    startPool.push_front(MethodWrapper(component, &T::start));
}

template <class T>
typename std::enable_if<HasUpdate<T>::value>::type 
GameObject::addUpdate(T * component) {
    updatePool.push_front(MethodWrapper(component, &T::update));
}

template <class T>
typename std::enable_if<HasOnDestroy<T>::value>::type
GameObject::addOnDestroy(T * component) {
    onDestroyPool.push_front(MethodWrapper(component, &T::onDestroy));
}

template <class T>
typename std::enable_if<HasAwake<T>::value>::type
GameObject::onSceneAwake(T * component) {
    component->awake();
}

template <class T>
typename std::enable_if<HasStart<T>::value>::type
GameObject::onSceneStart(T * component) {
    MethodsPool::startPool.push_front(MethodWrapper(component, &T::start));
}

template <class T>
typename std::enable_if<HasUpdate<T>::value>::type
GameObject::onSceneUpdate(T * component) {
    MethodsPool::updatePool.push_front(MethodWrapper(component, &T::update));
    component->updatePosition = updatePool.begin();  
}

//template<class T>
//T * GameObject::addComponent(const T *component) {
//    components.push_back(component);
//    component->gameObject = this;
//    component->transform = GameObject::transform;
//    return component;
//}


#endif //WG_GAMEOBJECT_H
