//
// Created by Олег Бобров on 17.11.2019.
//

#include "Component.h"
#include "../static/MethodsPool.hpp"


void Component::immideateDestroy() {
    if (this->updatePosition != std::numeric_limits<size_t>::max()) {
        MethodsPool::removeFromUpdate(this->updatePosition);        
    }
    this->gameObject->components.erase(this->componentPosition);
    delete this;
}

void Component::destroy() {
    MethodsPool::destroyComponentPool.push_front(this);
}
