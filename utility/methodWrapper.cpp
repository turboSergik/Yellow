#include "methodWrapper.hpp"
#include <utility>
#include <new>

MethodWrapper::MethodWrapper(const MethodWrapper & mw) {
    obj = mw.obj;
    memcpy(method, mw.method, methodSize);
    wrappedMethod = mw.wrappedMethod;
    methodSize = mw.methodSize;
}

MethodWrapper::MethodWrapper(MethodWrapper && mw) {
    obj = mw.obj;
    method = mw.method;
    wrappedMethod = mw.wrappedMethod;
    methodSize = mw.methodSize;
    mw.method = nullptr;
}

MethodWrapper & MethodWrapper::operator=(const MethodWrapper & mw) {
    this->~MethodWrapper();
    new (this) MethodWrapper(mw);
    return * this;
}

MethodWrapper & MethodWrapper::operator=(MethodWrapper && mw) {
    this->~MethodWrapper();
    new (this) MethodWrapper(std::move(mw));
    return * this;
}

void * MethodWrapper::getObject() const {
    return obj;
}

void MethodWrapper::operator()() const {
    wrappedMethod(obj, method);
}

MethodWrapper::~MethodWrapper() {
    ::operator delete(method);
}

void * UpdateWrapper::getObject() const {
    return obj;
}

void UpdateWrapper::operator()() const {
    wrappedMethod(obj);
}

void * FixedUpdateWrapper::getObject() const {
    return obj;
}

void FixedUpdateWrapper::operator()() const {
    wrappedMethod(obj);
}

void * StartWrapper::getObject() const {
    return obj;
}

void StartWrapper::operator()() const {
    wrappedMethod(obj);
}

void * OnDestroyWrapper::getObject() const {
    return obj;
}

void OnDestroyWrapper::operator()() const {
    wrappedMethod(obj);
}


