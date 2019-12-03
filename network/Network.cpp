//
// Created by Олег Бобров on 02.12.2019.
//

#include "Network.hpp"
#include "PacketQueue.hpp"

Event<nlohmann::json&> Network::onMapResponse;

void Network::update() {
    auto & packetQueue = PacketQueue::instance();
    packetQueue.update();
    if (packetQueue.anyReceived()) {
        auto pair = packetQueue.receivePacket();
        nlohmann::json json = pair.first.getJson();
        Action actionCode = static_cast<Action>(pair.second);
        switch (actionCode) {
            case Action::MAP: {
                onMapResponse.invoke(json);
                break;
            }
            default: {
                break;
            }
        }
    }
}

void Network::send(Action action, nlohmann::json & json) {
    auto & packetQueue = PacketQueue::instance();
    packetQueue.sendPacket(Packet(action, json));
}
