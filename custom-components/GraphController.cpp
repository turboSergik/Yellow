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
#include "../static/Time.h"

void GraphController::applyLayer10(const nlohmann::json &json) {
    if (json.contains("coordinates")) {
        for (const auto &item : json["coordinates"]) {
            int idx = item.value("idx", -1);
            if (idx != -1) {
                auto &point = Database::points[idx];
                if (!point) {
                    point = Prefabs::point(idx);
                    point->gameObject->instantiate(GraphController::transform);
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
    GraphController::layer1 = json;
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
    GraphController::layer0 = json;
    if (json.contains("points")) {
        for (const auto & item : json["points"]) {
            int idx = item.value("idx", -1);
            if (idx != -1) {
                auto & point = Database::points[idx];
                if (!point) {
                    point = Prefabs::point(idx);
                    point->gameObject->instantiate(GraphController::transform);
                }
                point->applyLayer0(item);
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
                    line->gameObject->instantiate(GraphController::transform);
                }
                line->applyLayer0(item);
                //for graph
                if (item.contains("points")) {
                    const auto &item_points = item["points"];
                    graph[item_points[0]].push_back(item_points[1]);
                    graph[item_points[1]].push_back(item_points[0]);
                }
            }
        }
    }
    playerController->playerPoint = Database::points[playerInfo["home"]["idx"]];
}

void GraphController::applyForceMethod() {
    auto positions = graphVisualizer.forceMethod();
    sf::Vector2f center = {0, 0};
    for (auto & pair : Database::points) {
        pair.second->transform->setLocalPosition(positions[pair.second->idx]);
        center += positions[pair.second->idx];
    }
    center /= static_cast<float>(Database::points.size());
    GraphController::transform->setPosition(-center);
}

void GraphController::applyForceMethodIteration() {
    auto positions = graphVisualizer.forceMethodIteration();
    sf::Vector2f center = {0, 0};
    for (auto & pair : Database::points) {
        pair.second->transform->setLocalPosition(positions[pair.second->idx]);
        center += positions[pair.second->idx];
    }
    center /= static_cast<float>(Database::points.size());
    GraphController::transform->setPosition(-center);
}

void GraphController::start() {
    GraphController::playerController = gameObject->addComponent<PlayerController>();
    Network::onLoginResponse.addListener<GraphController, &GraphController::onLogin>(this);
    Network::onMap0Response.addListener<GraphController, &GraphController::onMapLayer0>(this);
    Network::onMap1Response.addListener<GraphController, &GraphController::onMapLayer1>(this);

    Network::connect("wgforge-srv.wargaming.net", 443);
    Network::send(Action::LOGIN, {{"name", "Yellow2"}, {"game", "Yellow"}});
    Network::send(Action::MAP, {{"layer", 0}});
    Network::send(Action::MAP, {{"layer", 1}});
}

void GraphController::update() {
    for (int i = 0; i < 10; i++) {
        GraphController::applyForceMethodIteration();
    }
}

void GraphController::onDestroy() {
    Network::onLoginResponse.removeListener<GraphController, &GraphController::onLogin>(this);
    Network::onMap0Response.removeListener<GraphController, &GraphController::onMapLayer0>(this);
    Network::onMap1Response.removeListener<GraphController, &GraphController::onMapLayer1>(this);
}

void GraphController::onLogin(const nlohmann::json & json) {
    //TODO: handle when login received
    playerInfo = json;
}

void GraphController::onMapLayer0(const nlohmann::json & json) {
    //TODO: handle when layer0 received
    if (GraphController::layer0 != json) {
        GraphController::applyLayer0(json);
        GraphController::graphVisualizer.setGraph(graph);
    } else {
        Network::send(Action::MAP, {{"layer", 0}});
    }
}

void GraphController::onMapLayer1(const nlohmann::json & json) {
    //TODO: handle when layer1 received
    if (GraphController::layer1 != json) {
        GraphController::applyLayer1(json);
        playerController->isMapUpdated = true;
    } else {
        Network::send(Action::MAP, {{"layer", 1}});
    }
}