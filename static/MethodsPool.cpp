#include "MethodsPool.hpp"
#include "../core/Component.h"

std::vector<UpdateWrapper> MethodsPool::updatePool;
std::forward_list<StartWrapper> MethodsPool::startPool;

void MethodsPool::removeFromUpdate(size_t index) {
    std::swap(updatePool[index], updatePool.back());
    reinterpret_cast<Component *>(updatePool[index].getObject())->updatePosition = index;
}

void MethodsPool::update() {
    for (StartWrapper & start : MethodsPool::startPool) {
        start();
    }
    startPool.clear();
    // using indexing becouse vector size can change, and may resize
    for (int i = MethodsPool::updatePool.size() - 1; i > -1; --i) {
        MethodsPool::updatePool[static_cast<size_t>(i)]();
    }
}
