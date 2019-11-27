#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/IpAddress.hpp>
#include <cstdint>

class Connection : protected sf::TcpSocket {
    Connection() {}
    Connection(const Connection &) = delete;
    Connection(Connection &&) = delete;
    Connection & operator=(const Connection &) = delete;
    Connection & operator=(Connection &&) = delete;
protected:
    static Connection connection;
    
    // assuming that you first call login, than
    // use instance as much as you want, than call
    // logout
    
    friend class PacketQueue;
public:
    static void login();
    static Connection & instance();
    static void logout();
    ~Connection();
};

#endif // CONNECTION_HPP
