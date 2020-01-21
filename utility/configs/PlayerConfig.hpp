#ifndef PLAYERCONFIG_HPP
#define PLAYERCONFIG_HPP

#include <string>

class PlayerConfig
{
public:
    PlayerConfig();
    static std::string playerName;
    static std::string hostName;
    static size_t numPlayers;
    // trick to init config from file
    static PlayerConfig playerConfigInit;
};

#endif // PLAYERCONFIG_HPP
