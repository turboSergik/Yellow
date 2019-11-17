//
// Created by Олег Бобров on 15.11.2019.
//

#include <list>
#include "Database.h"
#include "static/graphdrawing.h"

void Database::applyLayer10(const nlohmann::json &json) {
    for (const auto& item : json["coordinates"]) {
        int idx = item["idx"];
        auto & point = points[idx];
        if (!point) {
            point.reset(new Point(idx));
        }
        point->applyLayer10(item);
    }
}

void Database::applyLayer1(const nlohmann::json &json) {
    for (const auto& item : json["posts"]) {
        int idx = item["idx"];
        auto& post = posts[idx];
        PostType type = item["type"];
        switch (type) {
            case PostType::TOWN: {
                if (!post) {
                    post.reset(new Town(idx));
                }
                post->applyLayer1(item);
                post->point = points[item["point_idx"]].get();
                break;
            }
            case MARKET: {
                if (!post) {
                    post.reset(new Market(idx));
                }
                post->applyLayer1(item);
                break;
            }
            case STORAGE: {
                if (!post) {
                    post.reset(new Storage(idx));
                }
                post->applyLayer1(item);
                break;
            }
        }
    }
    for (const auto& item : json["trains"]) {

    }
}

void Database::applyLayer0(const nlohmann::json &json) {
    for (const auto& item : json["points"]) {
        int idx = item["idx"];
        auto& point = points[idx];
        if (!point) {
            point.reset(new Point(idx));
        }
        auto& post_idx = item["post_idx"];
        point->post = post_idx == nullptr ? nullptr : posts[post_idx].get();
    }
    for (const auto& item : json["lines"]) {
        int idx = item["idx"];
        auto& line = lines[idx];
        if (!line) {
            line.reset(new Line(idx));
            line->idx = idx;
        }
        line->applyLayer0(item);
        auto& item_points = item["points"];
        line->points[0] = points[item_points[0]].get();
        line->points[1] = points[item_points[1]].get();
        //for graph
        graph[item_points[0]].push_back(item_points[1]);
        graph[item_points[1]].push_back(item_points[0]);
    }
}

void Database::generateCoordinates() {
    auto coordinates = GraphDrawing::forceMethod(graph);
    sf::Vector2f center = {0, 0};
    for (auto& pair : points) {
        const auto& position = coordinates[pair.second->idx];
        pair.second->setPosition(position);
        center += position;
    }
}

Database & Database::getInstance() {
    static Database instance;
    return instance;
}

Database::Database() {

}

