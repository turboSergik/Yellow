#ifndef METHODSPOOL_HPP
#define METHODSPOOL_HPP

#include <forward_list>
#include <vector>
#include "../utility/methodWrapper.hpp"
#include "../core/GameObject.h"

class MethodsPool
{
    static std::vector<UpdateWrapper> updatePool;
    static std::forward_list<StartWrapper> startPool;
    static std::forward_list<GameObject *> destroyObjectPool;
    static std::forward_list<Component *> destroyComponentPool;
    friend class GameObject;
    friend class Component;
    static void removeFromUpdate(size_t index);
public:
    static void update();
};

#endif // METHODSPOOL_HPP
