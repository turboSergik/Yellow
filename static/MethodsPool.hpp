#ifndef METHODSPOOL_HPP
#define METHODSPOOL_HPP

#include <list>
#include <vector>
#include "../utility/methodWrapper.hpp"
#include "../core/GameObject.h"

class MethodsPool
{
    static std::vector<UpdateWrapper> updatePool;
    static std::list<StartWrapper> startPool;
    static std::list<GameObject *> destroyObjectPool;
    static std::list<Component *> destroyComponentPool;
    friend class GameObject;
    friend class Component;
    static void removeFromUpdate(size_t index);
public:
    static void update();
};

#endif // METHODSPOOL_HPP
