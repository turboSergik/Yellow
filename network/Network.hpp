//
// Created by Олег Бобров on 02.12.2019.
//

#ifndef WG_NETWORK_HPP
#define WG_NETWORK_HPP


#include <utility>
#include <list>
#include "../utility/json.hpp"
#include "Packet.hpp"

template <class... Args>
class Event {
private:
    std::list <std::pair<void *, void (*)(void *, Args...)>> listeners;

    template<class TClass, void (TClass::*Method)(Args...)>
    static void getMethod(void * objPtr, Args... args);
public:
    template<class TClass, void (TClass::*Method)(Args...)>
    void addListener(TClass *objPtr);
    template<class TClass, void (TClass::*Method)(Args...)>
    void removeListener(TClass *objPtr);
    void invoke(Args... arg);
};

template<class... Args>
template<class TClass, void (TClass::*Method)(Args...)>
void Event<Args...>::getMethod(void * objPtr, Args... args) {
    return (static_cast<TClass*>(objPtr)->*Method)(std::forward(args)...);
}

template<class... Args>
template<class TClass, void (TClass::*Method)(Args...)>
void Event<Args...>::addListener(TClass *objPtr) {
    listeners.emplace_back(objPtr, &Event<Args...>::getMethod<TClass, Method>);
}

template<class... Args>
template<class TClass, void (TClass::*Method)(Args...)>
void Event<Args...>::removeListener(TClass *objPtr) {
    listeners.erase(std::find_if(listeners.begin(), listeners.end(),
                                 [&objPtr](std::pair<void *, void (*)(void *, Args...)> &pair) {
                                     return pair.first == objPtr &&
                                            pair.second == &Event<Args...>::getMethod<TClass, Method>;
                                 }));
}

template<class... Args>
void Event<Args...>::invoke(Args... args) {
    for (auto & listener : listeners) {
        listener.second(listener.first, std::forward<Args>(args)...);
    }
}

class Network {
private:
public:
    static Event<const nlohmann::json &> onMap0Response;
    static Event<const nlohmann::json &> onMap1Response;
    static Event<const nlohmann::json &> onLoginResponse;
    static Event<const nlohmann::json &> onPlayerResponse;
    static Event<const nlohmann::json &> onGamesResponse;
    
    static void update();
    static void send(Action action, nlohmann::json &);
};



#endif //WG_NETWORK_HPP
