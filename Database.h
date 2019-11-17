//
// Created by Олег Бобров on 15.11.2019.
//

#ifndef WG_DATABASE_H
#define WG_DATABASE_H

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <memory>
#include "custom-components/Point.h"
#include "custom-components/Line.h"
#include "custom-components/Post.h"
#include "custom-components/Train.h"
#include "json.hpp"

class Database {
private:
    Database();
    std::unordered_map<int, std::list<int>> graph;
    std::unordered_map<int, std::unique_ptr<Point>> points;
    std::unordered_map<int, std::unique_ptr<Line>> lines;
    std::unordered_map<int, std::unique_ptr<Post>> posts;
    std::unordered_map<int, std::unique_ptr<Train>> trains;
public:
    static Database & getInstance();

    Database(const Database &database) = delete;
    Database& operator=(Database &database) = delete;

    void applyLayer0(const nlohmann::json& json);
    void applyLayer1(const nlohmann::json& json);
    void applyLayer10(const nlohmann::json& json);
    void generateCoordinates();
};


#endif //WG_DATABASE_H
