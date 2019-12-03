//
// Created by Олег Бобров on 02.12.2019.
//

#ifndef WG_NETWORK_HPP
#define WG_NETWORK_HPP


#include <utility>
#include <list>
#include "../utility/json.hpp"
#include "Packet.hpp"

template <class T0>
class Event {
private:
    std::list <std::pair<void *, void (*)(void *, const T0)>> listeners;

    template<class TClass, void (TClass::*Method)(const T0)>
    static void getMethod(void * objPtr, const T0 arg);
public:
    template<class TClass, void (TClass::*Method)(const T0)>
    void addListener(TClass *objPtr);
    template<class TClass, void (TClass::*Method)(const T0)>
    void removeListener(TClass *objPtr);
    void invoke(const T0 arg);
};

template<class T0>
template<class TClass, void (TClass::*Method)(const T0)>
void Event<T0>::getMethod(void * objPtr, const T0 arg) {
    return (static_cast<TClass*>(objPtr)->*Method)(arg);
}

template<class T0>
template<class TClass, void (TClass::*Method)(const T0)>
void Event<T0>::addListener(TClass *objPtr) {
    listeners.emplace_back(objPtr, &Event<T0>::getMethod<TClass, Method>);
}

template<class T0>
template<class TClass, void (TClass::*Method)(const T0)>
void Event<T0>::removeListener(TClass *objPtr) {
    listeners.erase(std::find_if(listeners.begin(), listeners.end(),
                                 [&objPtr](std::pair<void *, void (*)(void *, const T0)> &pair) {
                                     return pair.first == objPtr &&
                                            pair.second == &Event<T0>::getMethod<TClass, Method>;
                                 }));
}

template<class T0>
void Event<T0>::invoke(const T0 arg) {
    for (auto & listener : listeners) {
        listener.second(listener.first, arg);
    }
}

class Network {
private:
public:
    static Event<nlohmann::json &> onMapResponse;

    static void update();
    static void send(Action action, nlohmann::json &);
};



#endif //WG_NETWORK_HPP
