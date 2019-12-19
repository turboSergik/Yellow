#include "Connection.hpp"
#include "PacketQueue.hpp"
#include "Packet.hpp"

Connection Connection::connection;

Connection & Connection::instance() {
    return connection;
}

void Connection::disconnect() {
    PacketQueue::instance().sendPacket(Packet(Action::LOGOUT, json{}));
    // may be check some error codes
    // now it seems that server dont send responce to
    // logout action
    connection.close();
}

Connection::~Connection() {
    disconnect();
}
