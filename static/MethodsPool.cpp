#include "MethodsPool.hpp"

std::list<MethodWrapper> MethodsPool::updatePool;
std::list<MethodWrapper> MethodsPool::startPool;
std::list<MethodWrapper> MethodsPool::onDestroyPool;

void MethodsPool::update() {
    for (MethodWrapper & start : MethodsPool::startPool) {
        start();
    }
    startPool.clear();
    for (MethodWrapper & update : MethodsPool::updatePool) {
        update();
    }
    for (MethodWrapper & onDestroy : MethodsPool::onDestroyPool) {
        onDestroy();
    }
}
