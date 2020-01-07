//
// Created by Олег Бобров on 02.12.2019.
//

#ifndef WG_NETWORK_HPP
#define WG_NETWORK_HPP


#include <utility>
#include <list>
#include <SFML/Network/IpAddress.hpp>
#include "../utility/json.hpp"
#include "Packet.hpp"
#include "../utility/Event.hpp"

class Network {
private:
    static bool validatePacket(const Packet & packet);
public:
    static Event<const nlohmann::json &> onMapResponse0;
    static Event<const nlohmann::json &> onMapResponse1;
    static Event<const nlohmann::json &> onMapResponse10;
    static Event<const nlohmann::json &> onLoginResponse;
    static Event<const nlohmann::json &> onPlayerResponse;
    static Event<const nlohmann::json &> onGamesResponse;
    static Event<> onTurn;

    static void connect(const sf::IpAddress & address, unsigned short port);
    static void disconnect();
    static void update();
    //TODO: optimaze this function parameters
    static void send(Action action, nlohmann::json);
    static void send(Action action);
};

#endif //WG_NETWORK_HPP
