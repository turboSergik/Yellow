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
                        post->applyLayer1(item);
                        break;
                    }
                    case STORAGE: {
                        if (!post) {
                            post = Prefabs::storage(idx);
                            post->gameObject->instantiate();
                        }
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
        for (const auto &item : json["trains"]) {
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

void GraphController::applyLayer0(const nlohmann::json &json) {
    if (json.contains("points")) {
        for (const auto &item : json["points"]) {
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
    //Camera::mainCamera->transform->setLocalPosition(center);
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
    //Camera::mainCamera->transform->setLocalPosition(center);
}

void checkResult(const Packet & received) {
    if (received.getFlag() != Result::OKEY) {
        std::cout << "something went wrong. Returned code: " << 
                     received.getFlag() << std::endl;
        std::cout << received.getJson();
        exit(1);
    }
}

void GraphController::start() {
    Network::onLoginResponse.addListener<GraphController, &GraphController::onLogin>(this);
    Network::onMap0Response.addListener<GraphController, &GraphController::onMapLayer0>(this);
    Network::onMap1Response.addListener<GraphController, &GraphController::onMapLayer1>(this);

    Network::connect("wgforge-srv.wargaming.net", 443);
    Network::send(Action::LOGIN, {{"name", "Yellow"}});
    Network::send(Action::MAP, {{"layer", 0}});
    Network::send(Action::MAP, {{"layer", 1}});
}

void GraphController::update() {
    // TODO normal timer for iterations
    for (int i = 0; i < 40; i++) {
        GraphController::applyForceMethodIteration();
    }
    //TODO: most of gameLogic
}

void GraphController::onDestroy() {
    Network::onLoginResponse.removeListener<GraphController, &GraphController::onLogin>(this);
    Network::onMap0Response.removeListener<GraphController, &GraphController::onMapLayer0>(this);
    Network::onMap1Response.removeListener<GraphController, &GraphController::onMapLayer1>(this);
}

void GraphController::onLogin(const nlohmann::json & json) {
    //TODO: handle when login received
}

void GraphController::onMapLayer0(const nlohmann::json & json) {
    //TODO: handle when layer0 received
}

void GraphController::onMapLayer1(const nlohmann::json & json) {
    //TODO: handle when layer1 received
}

