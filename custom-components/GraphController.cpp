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
#include "../utility/ForceMethodConfig.hpp"

using Random = effolkronium::random_static;

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

void GraphController::applyLayer1(const nlohmann::json &json) {
    //TODO: find difference between jsons
    playerController->markets.clear();
    playerController->storages.clear();
    this->layer1 = json;
    if (json.contains("posts")) {
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
                        playerController->markets.push_back(static_cast<Market*>(post));
                        post->applyLayer1(item);
                        break;
                    }
                    case STORAGE: {
                        if (!post) {
                            post = Prefabs::storage(idx);
                            post->gameObject->instantiate();
                        }
                        playerController->storages.push_back(static_cast<Storage*>(post));
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
    playerController->playerTown = static_cast<Town*>(Database::posts[playerInfo["home"]["post_idx"]]);
    if (playerInfo.contains("trains")) {
        playerController->playerTrains.clear();
        for (const auto & item : playerInfo["trains"]) {
            int idx = item.value("idx", -1);
            if (idx == -1) {
                continue;
            }
            auto & train = Database::trains[idx];
            if (train) {
                playerController->playerTrains.push_back(train);
            }
        }
    }
}

void GraphController::applyLayer0(const nlohmann::json &json) {
    this->layer0 = json;
    if (json.contains("points")) {
        this->graphSize = 2*ForceMethodConfig::springLength*sqrtf(json["points"].size());
        Camera::mainCamera->setWidth(this->graphSize);
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
    this->playerController->playerPoint = Database::points[playerInfo["home"]["idx"]];
}

void GraphController::start() {
    this->playerController = gameObject->addComponent<PlayerController>();
    Network::onLoginResponse.addListener<GraphController, &GraphController::onLogin>(this);
    Network::onMapResponse0.addListener<GraphController, &GraphController::onMapLayer0>(this);
    Network::onMapResponse1.addListener<GraphController, &GraphController::onMapLayer1>(this);
    Network::onMapResponse10.addListener<GraphController, &GraphController::onMapLayer10>(this);

    Network::connect("wgforge-srv.wargaming.net", 443);
    Network::send(Action::LOGIN, {{"name", "Yellow2"}, {"game", "Yellow"}});
    Network::send(Action::MAP, {{"layer", 0}});
    Network::send(Action::MAP, {{"layer", 1}});
    //Network::send(Action::MAP, {{"layer", 10}});

    //Test mode
//    std::ifstream fin("graphs/graph3.json");
//    fin >> this->layer0;
//    fin.close();
//    applyLayer0(this->layer0);
}

void GraphController::update() {
    for (auto it1 = Database::points.begin(); it1 != Database::points.end(); it1++) {
        const auto & point1 = it1->second;
        for (auto it2 = std::next(it1); it2 != Database::points.end(); it2++) {
            const auto &point2 = it2->second;
            lng::Vector2 direction = point2->transform->getPosition() - point1->transform->getPosition();
            float k = direction.magnitude() != 0.f ? ForceMethodConfig::charge / direction.magnitude() : 0.f;
            //float k = direction.magnitude() != 0.f ? ForceMethodConfig::charge / direction.sqrMagnitude()) : 0.f;
            point2->rigidBody->addForce(direction.normalized() * k);
            point1->rigidBody->addForce(-direction.normalized() * k);
        }
    }
}

void GraphController::onDestroy() {
    Network::onLoginResponse.removeListener<GraphController, &GraphController::onLogin>(this);
    Network::onMapResponse0.removeListener<GraphController, &GraphController::onMapLayer0>(this);
    Network::onMapResponse1.removeListener<GraphController, &GraphController::onMapLayer1>(this);
    Network::onMapResponse10.removeListener<GraphController, &GraphController::onMapLayer10>(this);
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
        this->playerController->isMapUpdated = true;
    } else {
        Network::send(Action::MAP, {{"layer", 1}});
    }
}

void GraphController::onMapLayer10(const nlohmann::json & json) {
    std::cout << "Layer 10 exists" << std::endl;
}