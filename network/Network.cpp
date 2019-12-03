//
// Created by Олег Бобров on 02.12.2019.
//

#include "Network.hpp"
#include "PacketQueue.hpp"

Event<const nlohmann::json &> Network::onMap0Response;
Event<const nlohmann::json &> Network::onMap1Response;
Event<const nlohmann::json &> onLoginResponse;
Event<const nlohmann::json &> onPlayerResponse;
Event<const nlohmann::json &> onGamesResponse;

void Network::update() {
    auto & packetQueue = PacketQueue::instance();
    packetQueue.update();
    if (packetQueue.anyReceived()) {
        auto pair = packetQueue.receivePacket();
        nlohmann::json receivedJson = pair.second.getJson();
        nlohmann::json sendedJson = pair.first.getJson();
        Action actionCode = static_cast<Action>(pair.first.getFlag());
        switch (actionCode) {
            case Action::MAP:
                if (sendedJson["layer"] == 0) {
                    onMap0Response.invoke(receivedJson);                    
                } else if (sendedJson["layer"] == 1) {
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

void Network::send(Action action, nlohmann::json & json) {
    auto & packetQueue = PacketQueue::instance();
    packetQueue.sendPacket(Packet(action, json));
}
