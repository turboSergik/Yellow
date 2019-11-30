#include "MethodsPool.hpp"
#include "../core/Component.h"

std::vector<UpdateWrapper> MethodsPool::updatePool;
std::forward_list<StartWrapper> MethodsPool::startPool;

void MethodsPool::removeFromUpdate(size_t index) {
    std::swap(updatePool[index], updatePool.back());
    reinterpret_cast<Component *>(updatePool[index].getObject())->updatePosition = index;
    MethodsPool::updatePool.pop_back();
}

void MethodsPool::update() {
    for (StartWrapper & start : MethodsPool::startPool) {
        start();
    }
    startPool.clear();
    // using indexing becouse vector size can change, and vector may resize
    size_t oldSize = MethodsPool::updatePool.size();
    for (size_t i = 0; i < oldSize; ++i) {
        MethodsPool::updatePool[i]();
    }
    for (Component * component : destroyComponentPool) {
        component->immideateDestroy();
    }
    for (GameObject * gameObject : destroyObjectPool) {
        gameObject->immideateDestroy();
    }
}
