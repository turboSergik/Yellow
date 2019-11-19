//
// Created by Олег Бобров on 19.11.2019.
//

#include <fstream>
#include "GraphController.h"
#include "../static/Database.h"
#include "Post.h"
#include "../static/PrefabCreator.h"
#include "../static/graphdrawing.h"

void GraphController::applyLayer10(const nlohmann::json &json) {
    for (const auto & item : json["coordinates"]) {
        int idx = item["idx"];
        auto & point = Database::points[idx];
        if (!point) {
            point = PrefabCreator::createPoint(idx);
            point->transform->setParent(GraphController::transform);//TODO: change to Instantiate(original, parent)
        }
        point->applyLayer10(item);
    }
}

void GraphController::applyLayer1(const nlohmann::json &json) {
    for (const auto & item : json["posts"]) {
        int idx = item["idx"];
        auto & post = Database::posts[idx];
        PostType type = item["type"];
        switch (type) {
            case PostType::TOWN: {
                if (!post) {
                    post = PrefabCreator::createTown(idx);
                }
                post->applyLayer1(item);
                break;
            }
            case MARKET: {
                if (!post) {
                    post = PrefabCreator::createMarket(idx);
                }
                post->applyLayer1(item);
                break;
            }
            case STORAGE: {
                if (!post) {
                    post = PrefabCreator::createStorage(idx);
                }
                post->applyLayer1(item);
                break;
            }
        }
    }
    for (const auto & item : json["trains"]) {
        //TODO: init trains
    }
}

void GraphController::applyLayer0(const nlohmann::json &json) {
    for (const auto & item : json["points"]) {
        int idx = item["idx"];
        auto& point = Database::points[idx];
        if (!point) {
            point = PrefabCreator::createPoint(idx);
            point->transform->setParent(GraphController::transform);//TODO: change to Instantiate(original, parent)
        }
        point->applyLayer0(item);
    }
    for (const auto & item : json["lines"]) {
        int idx = item["idx"];
        auto & line = Database::lines[idx];
        if (!line) {
            line = PrefabCreator::createLine(idx);
            line->transform->setParent(GraphController::transform);//TODO: change to Instantiate(original, parent)
        }
        line->applyLayer0(item);
        //for graph
        auto& item_points = item["points"];
        graph[item_points[0]].push_back(item_points[1]);
        graph[item_points[1]].push_back(item_points[0]);
    }
}

void GraphController::generateCoordinates() {
    auto positions = GraphDrawing::forceMethod(graph);
    sf::Vector2f center = {0, 0};
    for (auto & pair : Database::points) {
        pair.second->transform->setLocalPosition(positions[pair.second->idx]);
        center += positions[pair.second->idx];
    }
    center /= static_cast<float>(Database::points.size());
    Camera::mainCamera->transform->setLocalPosition(center);
}

void GraphController::update() {
    if (!called) {
        //TODO: do all network stuff here
        called = true;
        nlohmann::json layer0, layer1;
        std::ifstream in;
        in.open("map_layer0.json");
        in >> layer0;
        in.close();
        in.open("map_layer1.json");
        in >> layer1;
        in.close();

        GraphController::applyLayer0(layer0);
        GraphController::applyLayer1(layer1);
        GraphController::generateCoordinates();
    }
}
