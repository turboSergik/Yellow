#include "methodWrapper.hpp"

void MethodWrapper::operator()() {
    wrappedMethod(obj, method);
}

MethodWrapper::~MethodWrapper() {
    ::operator delete(method);
}
