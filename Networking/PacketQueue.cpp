#include "PacketQueue.hpp"
#include "Connection.hpp"
#include <iostream>

void PacketQueue::sendUpdate() {
    if (!sendQueue.empty()) {
        trySend();
    }
}

void PacketQueue::trySend() {
    Connection & connection = Connection::instance();
    size_t sended;
    size_t remained = sendQueue.front().remained();
    connection.send(sendQueue.front().getPtr(), remained, sended);
    if (sended == remained) {
        sendQueue.pop();
    } else {
        sendQueue.front().process(sended);
    }
}

void printStatus(sf::TcpSocket::Status status) {
    switch (status) {
    case (sf::TcpSocket::Status::Done): {
        std::cout << "Done" << std::endl;
        break;
    }
    case (sf::TcpSocket::Status::NotReady): {
        // std::cout << "NotReady" << std::endl;
        break;
    }
    case (sf::TcpSocket::Status::Partial): {
        std::cout << "Partial" << std::endl;
        break;
    }
    case (sf::TcpSocket::Status::Disconnected): {
        std::cout << "Disconnected" << std::endl;
        break;
    }
    case (sf::TcpSocket::Status::Error): {
        std::cout << "Error" << std::endl;
        break;
    }
    }
}

void PacketQueue::tryReceive() {
    Connection & connection = Connection::instance();
    if (receivingPacket.getProcessed() < 8) {
        size_t remained = 8 - receivingPacket.getProcessed();
        size_t received;
        sf::TcpSocket::Status res = 
        connection.receive(receivingPacket.getPtr(), remained, received);
        printStatus(res);
        if (remained == received) {
            if (receivingPacket.getJsonSize() == 0) {
                receiveQueue.push(std::move(receivingPacket));
                receivingPacket = Packet();
            } else {
                receivingPacket = Packet(receivingPacket.message.get());                
            }
        } else {
            receivingPacket.process(received);
        }
    } else {
        size_t remained = receivingPacket.remained();
        size_t received;
        sf::TcpSocket::Status res = 
        connection.receive(receivingPacket.getPtr(), remained, received);
        printStatus(res);        
        if (received == remained) {
            receiveQueue.push(std::move(receivingPacket));
            std::cout << receiveQueue.back().getJson().dump(4) << std::endl;
            std::cout << receiveQueue.back().getJson().dump(4).size() << std::endl;
            std::cout << receiveQueue.back().getJsonSize() << std::endl;
            std::cout.flush();
            receivingPacket = Packet();
        } else {
            receivingPacket.process(received);
        }
    }
}

void PacketQueue::update() {
    sendUpdate();
    tryReceive();
}

void PacketQueue::sendPacket(Packet packet) {
    int32_t code = *reinterpret_cast<int32_t *>(packet.message.get());
    sendQueue.push(std::move(packet));
    codesQueue.push(code);
}

bool PacketQueue::anyReceived() {
    return !receiveQueue.empty();
}

PacketQueue & PacketQueue::instance() {
    static PacketQueue singleton;
    return singleton;
}

std::pair<Packet, int32_t> PacketQueue::receivePacket() {
    Packet frontPacket = std::move(receiveQueue.front());
    receiveQueue.pop();
    int32_t code = codesQueue.front();
    codesQueue.pop();
    return std::make_pair(std::move(frontPacket), code);
}

void PacketQueue::waitSending() {
    while (!sendQueue.empty()) {
        trySend();
    }
}

void PacketQueue::wait() {
    waitSending();
    while (codesQueue.size() != receiveQueue.size()) {
        tryReceive();
    }
}
