//
// Created by Олег Бобров on 17.11.2019.
//

#ifndef WG_COMPONENT_H
#define WG_COMPONENT_H

#include "Transform.h"
#include "../utility/methodWrapper.hpp"
#include <vector>
#include <limits>

class Component {
    std::list<Component *>::iterator componentPosition;
    
    size_t updatePosition = std::numeric_limits<size_t>::max();
    void destroyImmediate();
    friend class GameObject;
    friend class MethodsPool;
public:
    GameObject *gameObject = nullptr; //TODO: prevent changes of this field
    Transform *transform = nullptr;
    
    void destroy();
    
    virtual ~Component() = default;
};


#endif //WG_COMPONENT_H
