#ifndef INTERFACECHECKER_HPP
#define INTERFACECHECKER_HPP

#include <type_traits>

// compile time check of having awake, update and start

class checkHasUpdate {
public:
    template <typename T>
    static std::true_type checkFunc(decltype(std::declval<T>().update()) *) {}
    template <typename T>
    static std::false_type checkFunc(...);
};

template <class T>
struct HasUpdate : decltype (checkHasUpdate::template checkFunc<T>(nullptr)) {};

class checkHasStart {
public:
    template <typename T>
    static std::true_type checkFunc(decltype(std::declval<T>().start()) *) {}
    template <typename T>
    static std::false_type checkFunc(...);
};

template <class T>
struct HasStart : decltype (checkHasUpdate::template checkFunc<T>(nullptr)) {};

class checkHasAwake {
public:
    template <typename T>
    static std::true_type checkFunc(decltype(std::declval<T>().awake()) *) {}
    template <typename T>
    static std::false_type checkFunc(...);
};

template <class T>
struct HasAwake : decltype (checkHasUpdate::template checkFunc<T>(nullptr)) {};


#endif // INTERFACECHECKER_HPP
