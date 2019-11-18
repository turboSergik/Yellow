//
// Created by Олег Бобров on 15.11.2019.
//

#include "Database.h"
#include "graphdrawing.h"
#include "PrefabCreator.h"

std::unordered_map<int, std::list<int>> Database::graph;
std::unordered_map<int, Point*> Database::points;
std::unordered_map<int, Line*> Database::lines;
std::unordered_map<int, Post*> Database::posts;
std::unordered_map<int, Train*> Database::trains;

void Database::applyLayer10(const nlohmann::json &json) {
    for (const auto & item : json["coordinates"]) {
        int idx = item["idx"];
        auto & point = Database::points[idx];
        if (!point) {
            point = PrefabCreator::createPoint(idx);
        }
        point->applyLayer10(item);
    }
}

void Database::applyLayer1(const nlohmann::json &json) {
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

    }
}

void Database::applyLayer0(const nlohmann::json &json) {
    for (const auto & item : json["points"]) {
        int idx = item["idx"];
        auto& point = Database::points[idx];
        if (!point) {
            point = PrefabCreator::createPoint(idx);
        }
        point->applyLayer0(item);
    }
    for (const auto & item : json["lines"]) {
        int idx = item["idx"];
        auto & line = lines[idx];
        if (!line) {
            line = PrefabCreator::createLine(idx);
        }
        line->applyLayer0(item);
        //for graph
        auto& item_points = item["points"];
        graph[item_points[0]].push_back(item_points[1]);
        graph[item_points[1]].push_back(item_points[0]);
    }
}

void Database::generateCoordinates() {
    auto positions = GraphDrawing::forceMethod(graph);
    for (auto & pair : points) {
        pair.second->transform->setLocalPosition(positions[pair.second->idx]);
    }
}


