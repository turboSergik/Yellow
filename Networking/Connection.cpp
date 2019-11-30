#include "Connection.hpp"
#include "PacketQueue.hpp"
#include "Packet.hpp"

Connection Connection::connection;

void Connection::login() {
    sf::IpAddress serverAddress("wgforge-srv.wargaming.net");
    uint16_t serverPort = 443;
    connection.setBlocking(false);
    connection.connect(serverAddress, serverPort);
    // here we can send game request, login request
    // map request and process them
}

Connection & Connection::instance() {
    return connection;
}

void Connection::logout() {
    PacketQueue::instance().sendPacket(Packet(Action::LOGOUT, json{}));
    // may be check some error codes
    // now it seems that server dont send responce to
    // logout action
    connection.close();
}

Connection::~Connection() {
    logout();
}
