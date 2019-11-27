#include "Connection.hpp"
#include "PacketQueue.hpp"
#include "Packet.hpp"

Connection Connection::connection;
const sf::IpAddress Connection::serverAddress("wgforge-srv.wargaming.net");
const uint16_t Connection::serverPort = 443;

void Connection::login() {
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
