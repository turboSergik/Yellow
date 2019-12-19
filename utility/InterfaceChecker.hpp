#ifndef INTERFACECHECKER_HPP
#define INTERFACECHECKER_HPP

#include <type_traits>

// compile time check of having awake, update and start

class CheckHasUpdate {
public:
    template <typename T>
    static std::true_type checkFunc(decltype(std::declval<T>().update()) *);
    template <typename T>
    static std::false_type checkFunc(...);
};

template <class T>
struct HasUpdate : decltype (CheckHasUpdate::template checkFunc<T>(nullptr)) {};

class CheckHasStart {
public:
    template <typename T>
    static std::true_type checkFunc(decltype(std::declval<T>().start()) *);
    template <typename T>
    static std::false_type checkFunc(...);
};

template <class T>
struct HasStart : decltype (CheckHasStart::template checkFunc<T>(nullptr)) {};

class CheckHasAwake {
public:
    template <typename T>
    static std::true_type checkFunc(decltype(std::declval<T>().awake()) *);
    template <typename T>
    static std::false_type checkFunc(...);
};

template <class T>
struct HasAwake : decltype (CheckHasAwake::template checkFunc<T>(nullptr)) {};

class CheckHasOnDestroy {
public:
    template <typename T>
    static std::true_type checkFunc(decltype(std::declval<T>().onDestroy()) *);
    template <typename T>
    static std::false_type checkFunc(...);
};

template <class T>
struct HasOnDestroy : decltype (CheckHasOnDestroy::template checkFunc<T>(nullptr)) {};


#endif // INTERFACECHECKER_HPP
