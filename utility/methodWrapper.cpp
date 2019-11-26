#include "methodWrapper.hpp"

void * MethodWrapper::getObject() const {
    return obj;
}

void MethodWrapper::operator()() const {
    wrappedMethod(obj, method);
}

MethodWrapper::~MethodWrapper() {
    ::operator delete(method);
}
