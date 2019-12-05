//
// Created by Олег Бобров on 02.12.2019.
//

#include "Network.hpp"
#include "PacketQueue.hpp"
#include <iostream>

Event<const nlohmann::json &> Network::onMap0Response;
Event<const nlohmann::json &> Network::onMap1Response;
Event<const nlohmann::json &> Network::onLoginResponse;
Event<const nlohmann::json &> Network::onPlayerResponse;
Event<const nlohmann::json &> Network::onGamesResponse;
Event<> Network::onTurn;


//enum Result
//{
//    OKEY = 0,
//    BAD_COMMAND = 1,
//    RESOURCE_NOT_FOUND = 2,
//    ACCESS_DENIED = 3,
//    INAPPROPRIATE_GAME_STATE = 4,
//    TIMEOUT = 5,
//    INTERNAL_SERVER_ERROR = 500
//};

bool Network::validatePacket(const Packet & packet) {
    Result code = static_cast<Result>(packet.getFlag());
    switch (code) {
        case OKEY:
            return true;
        case BAD_COMMAND:
            std::cerr << "Bad command" << std::endl;
            std::cerr << packet.getJson() << std::endl;
            return false;
        case RESOURCE_NOT_FOUND:
            std::cerr << "Resource not found" << std::endl;
            std::cerr << packet.getJson() << std::endl;
            return false;
        case ACCESS_DENIED:
            std::cerr << "access denied" << std::endl;
            std::cerr << packet.getJson() << std::endl;
            return false;
        case INAPPROPRIATE_GAME_STATE:
            std::cerr << "Inappopriate game state" << std::endl;
            std::cerr << packet.getJson() << std::endl;
            return false;
        case TIMEOUT:
            std::cerr << "timeout" << std::endl;
            std::cerr << packet.getJson() << std::endl;
            return false;
        case INTERNAL_SERVER_ERROR:
            std::cerr << "Internal server error" << std::endl;
            return false;
    }
}

void Network::connect(const sf::IpAddress &address, unsigned short port) {
    Connection::instance().connect(address, port);
    Connection::instance().setBlocking(false);
}

void Network::update() {
    auto & packetQueue = PacketQueue::instance();
    packetQueue.update();
    while (!packetQueue.is_empty()) {
        auto pair = packetQueue.receivePacket();
        if (!validatePacket(pair.second)) {
            continue;
        }
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
                break;
            case Action::TURN:
                onTurn.invoke();
                break;
            default: 
                break;
        }
    }
}

void Network::send(Action action, nlohmann::json json) {
    auto & packetQueue = PacketQueue::instance();
    packetQueue.sendPacket(Packet(action, std::move(json)));
}

void Network::send(Action action) {
    auto & packetQueue = PacketQueue::instance();
    packetQueue.sendPacket(Packet(action));
}

void Network::disconnect() {
    Connection::instance().disconnect();
}
