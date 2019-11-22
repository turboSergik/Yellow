#include "MethodsPool.hpp"

void MethodsPool::update() {
    for (MethodWrapper & start : startPool) {
        start();
    }
    startPool.clear();
    for (MethodWrapper & update : updatePool) {
        update();
    }
}
