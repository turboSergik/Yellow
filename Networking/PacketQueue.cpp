#include "PacketQueue.hpp"
#include "Connection.hpp"
#include <iostream>

bool PacketQueue::trySend() {
    Connection & connection = Connection::instance();
    size_t sended;
    size_t remained = sendQueue.front().remained();
    connection.send(sendQueue.front().getPtr(), remained, sended);
    if (sended == remained) {
        sendQueue.pop();
        return true;
    } else {
        sendQueue.front().process(sended);
        return false;
    }
}

bool PacketQueue::tryReceive() {
    Connection & connection = Connection::instance();
    if (receivingPacket.getProcessed() < 8) {
        size_t remained = 8 - receivingPacket.getProcessed();
        size_t received;
        connection.receive(receivingPacket.getPtr(), remained, received);
        if (remained == received) {
            if (receivingPacket.getJsonSize() == 0) {
                receiveQueue.push(std::move(receivingPacket));
                receivingPacket = Packet();
                return true;
            } else {
                receivingPacket = Packet(receivingPacket.message.get());
            }
        } else {
            receivingPacket.process(received);
        }
        return false;
    } else {
        size_t remained = receivingPacket.remained();
        size_t received;
        connection.receive(receivingPacket.getPtr(), remained, received);
        if (received == remained) {
            receiveQueue.push(std::move(receivingPacket));
            receivingPacket = Packet();
            return true;
        } else {
            receivingPacket.process(received);
            return false;
        }
    }
}

void PacketQueue::update() {
    if (sending) {
        // this means if not empty return !trySend()
        sending = sendQueue.empty() || !trySend();
    } else {
        sending = tryReceive();
    }
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

void PacketQueue::wait() {
    while (codesQueue.size() != receiveQueue.size()) {
        update();
    }
}

int PacketQueue::getSize() {
    return receiveQueue.size();
}
