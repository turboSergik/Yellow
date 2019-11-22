#ifndef METHODWRAPPER_HPP
#define METHODWRAPPER_HPP

#include <cstring>

class MethodWrapper {
    void * obj;
    void * method;
    void (*wrappedMethod)(void *, void *);
    
    
    template <typename T>
    static void concreteWrapperMethod(void * obj, void * method) {
        (reinterpret_cast<T *>(obj)->**reinterpret_cast<void (T::**)()>(method))();
    }
public:
    template <class T>
    MethodWrapper(T * objPtr, void (T::* objMethod)()) : obj(objPtr) {
        method = ::operator new(sizeof(objPtr));
        std::memcpy(method, &objMethod, sizeof(objMethod));
        wrappedMethod = concreteWrapperMethod<T>;
    }
    
    void operator()() const;
    
    ~MethodWrapper();
};

#endif // METHODWRAPPER_HPP
