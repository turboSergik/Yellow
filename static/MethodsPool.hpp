#ifndef METHODSPOOL_HPP
#define METHODSPOOL_HPP

#include <list>
#include <vector>
#include "../utility/methodWrapper.hpp"

class MethodsPool
{
    static std::vector<MethodWrapper> updatePool;
    static std::list<MethodWrapper> startPool;
    friend class GameObject;
public:
    static void update();
};

#endif // METHODSPOOL_HPP
