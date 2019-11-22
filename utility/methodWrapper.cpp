#include "methodWrapper.hpp"

void MethodWrapper::operator()() const {
    wrappedMethod(obj, method);
}

MethodWrapper::~MethodWrapper() {
    ::operator delete(method);
}
