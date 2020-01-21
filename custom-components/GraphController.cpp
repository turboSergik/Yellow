//
// Created by Олег Бобров on 19.11.2019.
//

#include <fstream>
#include <iostream>
#include "GraphController.h"
#include "../static/Database.h"
#include "Post.h"
#include "../static/Prefabs.h"
#include "../network/Connection.hpp"
#include "../network/PacketQueue.hpp"
#include "../network/Packet.hpp"
#include "../network/Network.hpp"
#include "../utility/random.hpp"
#include "../core-components/renderers/CircleRenderer.h"
#include "CameraController.h"
#include "../static/Input.hpp"
#include "../core-components/colliders/Collider.hpp"

using Random = effolkronium::random_static;

void GraphController::start() {
    Network::onLoginResponse.addListener<GraphController, &GraphController::onLogin>(this);
    Network::onMapResponse0.addListener<GraphController, &GraphController::onMapLayer0>(this);
    Network::onMapResponse1.addListener<GraphController, &GraphController::onMapLayer1>(this);
    Network::onMapResponse10.addListener<GraphController, &GraphController::onMapLayer10>(this);
    Network::onPlayerResponse.addListener<GraphController, &GraphController::onPlayer>(this);
    Network::onGamesResponse.addListener<GraphController, &GraphController::onGames>(this);

    Network::connect("wgforge-srv.wargaming.net", 443);
    Network::send(Action::LOGIN, {
            {"name", PlayerConfig::playerName},
            {"game", PlayerConfig::hostName},
            {"num_players", PlayerConfig::numPlayers},
            {"num_turns", 2000}});
    Network::send(Action::MAP, {{"layer", 0}});
    Network::send(Action::MAP, {{"layer", 1}});
    //Network::send(Action::MAP, {{"layer", 10}});
    Network::send(Action::GAMES);
}

void GraphController::onDestroy() {
    Network::onLoginResponse.removeListener<GraphController, &GraphController::onLogin>(this);
    Network::onMapResponse0.removeListener<GraphController, &GraphController::onMapLayer0>(this);
    Network::onMapResponse1.removeListener<GraphController, &GraphController::onMapLayer1>(this);
    Network::onMapResponse10.removeListener<GraphController, &GraphController::onMapLayer10>(this);
    Network::onPlayerResponse.removeListener<GraphController, &GraphController::onPlayer>(this);
    Network::onGamesResponse.removeListener<GraphController, &GraphController::onGames>(this);
}

void GraphController::applyLayer10(const nlohmann::json &json) {
    if (json.contains("coordinates")) {
        for (const auto &item : json["coordinates"]) {
            int idx = item.value("idx", -1);
            if (idx != -1) {
                auto &point = Database::points[idx];
                if (!point) {
                    point = Prefabs::point(idx);
                    point->gameObject->instantiate(this->transform);
                }
                point->applyLayer10(item);
            }
        }
    }
}

void GraphController::applyLayer0(const nlohmann::json &json) {
    this->layer0 = json;
    if (json.contains("points")) {
        //this->graphSize = ForceMethodConfig::springLength*json["points"].size();
        this->graphSize = 1000;
        Camera::mainCamera->setWidth(this->graphSize);
        Camera::mainCamera->gameObject->getComponent<CameraController>()->targetWidth = this->graphSize;
        for (const auto & item : json["points"]) {
            int idx = item.value("idx", -1);
            if (idx != -1) {
                auto &point = Database::points[idx];
                if (!point) {
                    point = Prefabs::point(idx);
                    point->gameObject->instantiate(this->transform);
                }
                point->applyLayer0(item);
                point->transform->setPosition({Random::get(-this->graphSize, this->graphSize),
                                               Random::get(-this->graphSize, this->graphSize)});
            }
        }
    }
    if (json.contains("lines")) {
        for (const auto &item : json["lines"]) {
            int idx = item.value("idx", -1);
            if (idx != -1) {
                auto &line = Database::lines[idx];
                if (!line) {
                    line = Prefabs::line(idx);
                    line->gameObject->instantiate(this->transform);
                }
                line->applyLayer0(item);
            }
        }
    }
}

void GraphController::applyLayer1(const nlohmann::json &json) {
    //TODO: find difference between jsons
    this->layer1 = json;
    if (json.contains("posts")) {
        Database::markets.clear();
        Database::storages.clear();
        for (const auto &item : json["posts"]) {
            int idx = item.value("idx", -1);
            if (idx != -1) {
                auto & post = Database::posts[idx];
                PostType type = item.value("type", PostType::DEFAULT);
                switch (type) {
                    case PostType::TOWN: {
                        if (!post) {
                            post = Prefabs::town(idx);
                            post->gameObject->instantiate();
                        }
                        post->applyLayer1(item);
                        break;
                    }
                    case MARKET: {
                        if (!post) {
                            post = Prefabs::market(idx);
                            post->gameObject->instantiate();
                        }
                        Database::markets.push_back(static_cast<Market*>(post));
                        post->applyLayer1(item);
                        break;
                    }
                    case STORAGE: {
                        if (!post) {
                            post = Prefabs::storage(idx);
                            post->gameObject->instantiate();
                        }
                        Database::storages.push_back(static_cast<Storage*>(post));
                        post->applyLayer1(item);
                        break;
                    }
                    default: {
                        break;
                    }
                }
            }
        }
    }
    if (json.contains("trains")) {
        for (const auto & item : json["trains"]) {
            int idx = item.value("idx", -1);
            if (idx == -1) {
                continue;
            }
            auto & train = Database::trains[idx];
            if (!train) {
                train = Prefabs::train(idx);
                train->gameObject->instantiate();
            }
            train->applyLayer1(item);
        }
    }
}

void GraphController::applyPlayerInfo(const nlohmann::json &json) {
    this->playerInfo = json;
    this->playerController = gameObject->addComponent<PlayerController>();
    this->playerController->playerPoint = Database::points[playerInfo["home"]["idx"]];
    this->playerController->playerTown = static_cast<Town*>(Database::posts[playerInfo["home"]["post_idx"]]);
    this->playerController->playerTown->gameObject->getComponent<CircleRenderer>()->circle.setFillColor(InterfaceConfig::playerTownColor);
    if (this->playerInfo.contains("trains")) {
        this->playerController->playerTrains.clear();
        for (const auto & item : this->playerInfo["trains"]) {
            int idx = item.value("idx", -1);
            if (idx == -1) {
                continue;
            }
            auto & train = Database::trains[idx];
            if (train) {
                this->playerController->playerTrains.push_back(train);
                train->gameObject->getComponent<CircleRenderer>()->circle.setFillColor(InterfaceConfig::playerTrainColor);
            }
        }
    }
}

bool GraphController::isGameRunning(const nlohmann::json & json) {
    if (json.contains("games")) {
        for (auto game : json["games"]) {
            if (game["name"] == PlayerConfig::hostName && game["state"] == GameState::RUN) {
                return true;
            }
        }
    }
    return false;
}

void GraphController::onLogin(const nlohmann::json & json) {
    this->playerInfo = json;
}

void GraphController::onMapLayer0(const nlohmann::json & json) {
    if (this->layer0 != json) {
        this->applyLayer0(json);
    } else {
        Network::send(Action::MAP, {{"layer", 0}});
    }
}

void GraphController::onMapLayer1(const nlohmann::json & json) {
    if (this->layer1 != json) {
        this->applyLayer1(json);
        Database::isMapUpdated = true;
        // Database::tick++;
    } else {
        Network::send(Action::MAP, {{"layer", 1}});
    }
}

void GraphController::onMapLayer10(const nlohmann::json & json) {
    //std::cout << "Layer 10 exists" << std::endl;
    this->applyLayer10(json);
    std::cout << json.dump(4) << std::endl;
}

void GraphController::onPlayer(const nlohmann::json & json) {
    this->applyPlayerInfo(json);
}

void GraphController::onGames(const nlohmann::json & json) {
    //std::cout << json.dump(4) << std::endl;
    if (isGameRunning(json)) {
        //Network::send(Action::PLAYER);
        this->applyPlayerInfo(playerInfo);
    } else {
        Network::send(Action::GAMES);
    }
}
