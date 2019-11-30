#ifndef METHODSPOOL_HPP
#define METHODSPOOL_HPP

#include <forward_list>
#include <vector>
#include "../utility/methodWrapper.hpp"

class MethodsPool
{
    static std::vector<UpdateWrapper> updatePool;
    static std::forward_list<StartWrapper> startPool;
    friend class GameObject;
    static void removeFromUpdate(size_t index);
public:
    static void update();
};

#endif // METHODSPOOL_HPP
