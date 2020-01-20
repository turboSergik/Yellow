#include "PlayerConfig.hpp"
#include <fstream>
#include "json.hpp"

std::string PlayerConfig::playerName;
std::string PlayerConfig::hostName;
size_t PlayerConfig::numPlayers;

PlayerConfig::PlayerConfig() {
    std::ifstream configFile("playerConfig.json");
    nlohmann::json playerConfig;
    configFile >> playerConfig;
    PlayerConfig::hostName = playerConfig["hostName"];
    PlayerConfig::numPlayers = playerConfig["numPlayers"];
    PlayerConfig::playerName = playerConfig["playerName"];    
}

PlayerConfig PlayerConfig::playerConfigInit;
