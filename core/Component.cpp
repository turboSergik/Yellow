//
// Created by Олег Бобров on 17.11.2019.
//

#include "GameObject.h"
#include "Component.h"
#include "../static/MethodsPool.hpp"


void Component::destroyImmediate() {
    if (this->updatePosition != std::numeric_limits<size_t>::max()) {
        MethodsPool::removeFromUpdate(this->updatePosition);        
    }
    this->gameObject->components.erase(this->componentPosition);
    delete this;
}

void Component::destroy() {
    MethodsPool::destroyComponentPool.push_back(this);
}
