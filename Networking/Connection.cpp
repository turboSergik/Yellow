#include "Connection.hpp"

Connection Connection::connection;
const sf::IpAddress Connection::serverAddress("wgforge-srv.wargaming.net");
const uint16_t Connection::serverPort = 443;

void Connection::login() {
    connection.connect(serverAddress, serverPort);
    // here we can send game request, login request
    // map request and process them
}

Connection & Connection::instance() {
    return connection;
}

void Connection::logout() {
    connection.close();
    // here we can send logout request
}
