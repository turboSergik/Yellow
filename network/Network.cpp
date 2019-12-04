//
// Created by Олег Бобров on 02.12.2019.
//

#include "Network.hpp"
#include "PacketQueue.hpp"

Event<const nlohmann::json &> Network::onMap0Response;
Event<const nlohmann::json &> Network::onMap1Response;
Event<const nlohmann::json &> Network::onLoginResponse;
Event<const nlohmann::json &> Network::onPlayerResponse;
Event<const nlohmann::json &> Network::onGamesResponse;


void Network::connect(const sf::IpAddress &address, unsigned short port) {
    Connection::instance().connect(address, port);
    Connection::instance().setBlocking(false);
}

void Network::update() {
    auto & packetQueue = PacketQueue::instance();
    packetQueue.update();
    while (!packetQueue.is_empty()) {
        auto pair = packetQueue.receivePacket();
        nlohmann::json receivedJson = pair.second.getJson();
        nlohmann::json sentJson = pair.first.getJson();
        Action actionCode = static_cast<Action>(pair.first.getFlag());
        switch (actionCode) {
            case Action::MAP:
                if (sentJson["layer"] == 0) {
                    onMap0Response.invoke(receivedJson);                    
                } else if (sentJson["layer"] == 1) {
                    onMap1Response.invoke(receivedJson);
                }
                break;
            case Action::LOGIN:
                onLoginResponse.invoke(receivedJson);
                break;
            case Action::PLAYER:
                onPlayerResponse.invoke(receivedJson);
                break;
            case Action::GAMES:
                onGamesResponse.invoke(receivedJson);
            default: 
                break;
        }
    }
}

void Network::send(Action action, nlohmann::json json) {
    auto & packetQueue = PacketQueue::instance();
    packetQueue.sendPacket(Packet(action, std::move(json)));
}

void Network::disconnect() {
    Connection::instance().disconnect();
}
