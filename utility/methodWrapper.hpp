#ifndef METHODWRAPPER_HPP
#define METHODWRAPPER_HPP

#include <cstring>

class MethodWrapper {
    void * obj;
    void * method;
    size_t methodSize;
    void (*wrappedMethod)(void *, void *);
    
    
    template <typename T>
    static void concreteWrapperMethod(void * obj, void * method) {
        (reinterpret_cast<T *>(obj)->**reinterpret_cast<void (T::**)()>(method))();
    }
public:
    MethodWrapper() = delete;
    MethodWrapper(const MethodWrapper & mw);
    MethodWrapper(MethodWrapper &&);
    MethodWrapper & operator=(const MethodWrapper & mw);
    MethodWrapper & operator=(MethodWrapper &&);    
    template <class T>
    MethodWrapper(T * objPtr, void (T::* objMethod)()) : obj(objPtr) {
        methodSize = sizeof(objMethod);
        method = ::operator new(methodSize);
        std::memcpy(method, &objMethod, methodSize);
        wrappedMethod = MethodWrapper::concreteWrapperMethod<T>;
    }
    
    void * getObject() const;
    
    void operator()() const;
    
    ~MethodWrapper();
};

class UpdateWrapper {
    void * obj;
    void (*wrappedMethod)(void *);
    
    template <typename T>
    static void concreteWrapperMethod(void * obj) {
        reinterpret_cast<T *>(obj)->update();
    }
public:
    UpdateWrapper() = delete;
    template <class T>
    UpdateWrapper(T * objPtr) : obj(objPtr) ,
    wrappedMethod(UpdateWrapper::concreteWrapperMethod<T>) {}
    
    void * getObject() const;
    
    void operator()() const;
    
    ~UpdateWrapper() = default;
};

class FixedUpdateWrapper {
    void * obj;
    void (*wrappedMethod)(void *);
    
    template <typename T>
    static void concreteWrapperMethod(void * obj) {
        reinterpret_cast<T *>(obj)->fixedUpdate();
    }
public:
    FixedUpdateWrapper() = delete;
    template <class T>
    FixedUpdateWrapper(T * objPtr) : obj(objPtr) ,
    wrappedMethod(UpdateWrapper::concreteWrapperMethod<T>) {}
    
    void * getObject() const;
    
    void operator()() const;
    
    ~FixedUpdateWrapper() = default;
};

class StartWrapper {
    void * obj;
    void (*wrappedMethod)(void *);
    
    template <typename T>
    static void concreteWrapperMethod(void * obj) {
        reinterpret_cast<T *>(obj)->start();
    }
public:
    StartWrapper() = delete;
    template <class T>
    StartWrapper(T * objPtr) : obj(objPtr) ,
    wrappedMethod(StartWrapper::concreteWrapperMethod<T>) {}
    
    void * getObject() const;
    
    void operator()() const;
    
    ~StartWrapper() = default;
};

class OnDestroyWrapper {
    void * obj;
    void (*wrappedMethod)(void *);
    
    template <typename T>
    static void concreteWrapperMethod(void * obj) {
        reinterpret_cast<T *>(obj)->onDestroy();
    }
public:
    OnDestroyWrapper() = delete;
    template <class T>
    OnDestroyWrapper(T * objPtr) : obj(objPtr) ,
    wrappedMethod(OnDestroyWrapper::concreteWrapperMethod<T>) {}
    
    void * getObject() const;
    
    void operator()() const;
    
    ~OnDestroyWrapper() = default;
};

#endif // METHODWRAPPER_HPP
