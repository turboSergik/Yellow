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

#include <queue>
#include <bitset>
#include <algorithm>

int f[300000];
std::vector<int> way[300000];

class El {

public:
    std::vector<int> way;
    std::bitset<2000> used;

    int value = 0;
    int spent = 0;
    int vertex = 0;
    int maxValue = 0;
    int startVertex = 0;

};

bool operator< (El left, El right) {

        if (left.value < right.value || ( left.value == right.value && left.spent > right.spent)) return true;
        return false;

}


bool isStorage(int vertex) {

    for (auto ver : Database::posts) {

        if (ver.second -> point -> idx == vertex && ver.second -> type == 3) return true;
    }
    return false;
}

bool isMarket(int vertex) {

    for (auto ver : Database::posts) {

        // std::cout << "In post;" << std::endl;
        // std::cout << "Idx=" << ver.second -> point -> idx << std::endl;

        if (ver.second -> point -> idx == vertex && ver.second -> type == 2) return true;
    }
    return false;
}


int marketProduct(int vertex) {

     for (auto ver : Database::posts)  if (ver.second -> point -> idx == vertex) {

        auto *townObj = static_cast<Market* >(ver.second);
        return townObj -> product;
    }
}


std::vector<int> showWay() {

    /*
    std::cout << "way=== ";
    for (auto ver : Database::posts) {
        if (ver.second -> type == 1) {
            for (int i = 0; i < way[ver.second -> point -> idx].size(); i++) {
                std::cout << way[ver.second -> point -> idx][i] - 332 << " ";
            }
            std::cout << std::endl;
        }
    }

    for (auto ver : Database::posts) {
        if (ver.second -> type == 1) {
            std::cout << "  cost= " << f[ver.second -> point -> idx] << std::endl;
        }
    }
    */

    for (auto ver : Database::posts) if (ver.second -> type == 1) return way[ver.second -> point -> idx];

}



void changeDist() {

    for (auto vertexNow : Database::points) {
        f[vertexNow.second -> idx] = -1e9;
        way[vertexNow.second -> idx].clear();
    }
}


int getTrainVertex(Train* train) {

    if (train -> position == 0) return train -> line -> points[0] -> idx;
    else return train -> line -> points[1] -> idx;
}


void main_strategy();
void refresh_map();
void calcTrainWay(Train*);
void trainIteration(Train*);


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

    Database::is_map_updated = true;

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
    GraphController::layer0 = json;
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
    Network::send(Action::LOGIN, {{"name", "Yellow2"}, {"game", "Yellow"}});
    Network::send(Action::MAP, {{"layer", 0}});
    Network::send(Action::MAP, {{"layer", 1}});
}

void GraphController::update() {

    for (int i = 0; i < 80; i++) {
        GraphController::applyForceMethodIteration();
    }

    //TODO: most of gameLogic

   main_strategy();



    sf::sleep(sf::milliseconds(100));

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
    } else {
        Network::send(Action::MAP, {{"layer", 1}});
    }
}

void main_strategy() {


    if (Database::is_map_updated == true) {

        Database::is_map_updated = false;

        std::cout << "=====================================" << std::endl;

        for (auto now : Database::posts) {


            // std::cout << "idx=" << now.first << " name= " << now.second -> name << std::endl;
            if (now.second -> type == 1) {
                // std::cout << "in market" << std::endl;
                // std::cout << "Product= " << now.second -> product << std::endl;

                /// class Post
                /// class Market::Post

                auto *kek = static_cast<Town* >(now.second);
                std::cout << "Product=" << kek -> product << " Population=" << kek -> population << std::endl;
            }


            /*
            else
            if (now.second -> type == 2) {

                auto *kek = static_cast<Market* >(now.second);
                std::cout << "Product=" << kek -> product << std::endl;

            }
            */
        }

        /// int train_count = 0;
        for (auto train_now : Database::trains) {

            // std::cout << "Move_type=" << train_now.second -> move_type << std::endl;

            if (train_now.second -> move_type == 0) {
                train_now.second -> move_type = 1;

                ///TODO: move_type of trains + destiny;
            }

            int train_pos = train_now.second -> position;
            if (train_pos == 0 || train_pos == train_now.second -> line -> length) {
                /// trainLogic(train_now.second);
                train_now.second -> move_type = 2;

                if (train_now.second -> need_way.size() == 0) {

                    // std::cout << "Calc train way" << std::endl;

                    calcTrainWay(train_now.second);
                }

                // std::cout << "Calc train iteration" << std::endl;
                trainIteration(train_now.second);

                train_now.second -> need_way.pop_back();

            }

        }

        json message;
        Network::send(Action::TURN, message);
        Network::send(Action::MAP, {{"layer", 1}});

    }
}


void trainLogic(Train* train) {
    /*
    if (train -> need_way.size() == 0) {

        if (train -> move_type == 1) calcTrainWay(train);
        else if (train -> move_type == 2) calcTrainWayToUpdate();
    }
    else trainWayIteration(train);
    */
}


void calcTrainWay(Train* train) {

    // std::cout << "In way" << std::endl;


    std::priority_queue<El> q;
    El start;

    changeDist();

    for (auto pointNow : Database::posts) {

        /// if it is market
        if (pointNow.second -> type == 2) {

            auto *marketObj = static_cast<Market* >(pointNow.second);

            start.vertex = marketObj -> point -> idx;
            start.maxValue = train -> goods_capacity;
            start.value = std::min(marketObj -> product, start.maxValue);
            // start.way.push_back(start.vertex);
            start.startVertex = start.vertex;

            q.push(start);

            // std::cout << "q.push " << marketObj -> point -> idx - 332 << std::endl;
        }

    }
    // std::cout << "Queue size=" << q.size() << std::endl;
    while(!q.empty()) {


        El to, now = q.top();
        q.pop();

        // std::cout << "In queue; ver=" << now.vertex - 332 << " value=" <<  now.value << " f=" << f[now.vertex] << std::endl;

        if (isStorage(now.vertex) == true) continue;

        if (isMarket(now.vertex) == true && now.used[now.vertex] == 0){

            // std::cout << "Is market; value += " << marketProduct(now.vertex) << std::endl;

            now.used[now.vertex] = 1;
            now.value += marketProduct(now.vertex);



            now.value = std::min(now.maxValue, now.value);


        }
        if (f[now.vertex] >= now.value - now.spent) continue;
        f[now.vertex] = now.value - now.spent;

        way[now.vertex].clear();
        way[now.vertex].push_back(now.startVertex);
        for (int i = 0; i < now.way.size(); i++) way[now.vertex].push_back(now.way[i]);
        // way[now.vertex] = now.way;

        // std::cout << "+In queue; ver=" << now.vertex - 332 << " value=" <<  now.value << std::endl;

        for (auto line : Database::lines) {

            if (line.second -> points[0] -> idx == now.vertex) {

                to = now;
                to.vertex = line.second -> points[1] -> idx;
                to.spent += line.second -> length + 1;
                to.way.push_back(to.vertex);
                // std::cout << "Push to next " << to.vertex - 332 << " new value=: " << to.value << std::endl;

                q.push(to);
            }
            if (line.second -> points[1] -> idx == now.vertex) {

                to = now;
                to.vertex = line.second -> points[0] -> idx;
                to.spent += line.second -> length + 1;
                to.way.push_back(to.vertex);
                // std::cout << "Push to next " << to.vertex - 332 << " new value=: " << to.value << std::endl;

                q.push(to);
            }
        }
    }

    // std::cout << "=============================================" << std::endl;
    std::cout << "=============================================" << std::endl;
    std::cout << "=============================================" << std::endl;

    train -> need_way = showWay();
    if (train -> need_way.size() == 0) return;

    changeDist();

    El startShortestWay;
    startShortestWay.vertex = train -> need_way[0];

    q.push(startShortestWay);


    while(!q.empty()) {


        El to, now = q.top();
        q.pop();

        // std::cout << "In queue; ver=" << now.vertex - 332 << " value=" <<  now.value << " f=" << f[now.vertex] << std::endl;

        if (f[now.vertex] >= now.value - now.spent) continue;
        f[now.vertex] = now.value - now.spent;

        way[now.vertex] = now.way;

        // std::cout << "+In queue; ver=" << now.vertex - 332 << " value=" <<  now.value << std::endl;

        for (auto line : Database::lines) {

            if (line.second -> points[0] -> idx == now.vertex) {

                to = now;
                to.vertex = line.second -> points[1] -> idx;
                to.spent += line.second -> length + 1;
                to.way.push_back(to.vertex);
                // std::cout << "Push to next " << to.vertex - 332 << " new value=: " << to.value << std::endl;

                q.push(to);
            }
            if (line.second -> points[1] -> idx == now.vertex) {

                to = now;
                to.vertex = line.second -> points[0] -> idx;
                to.spent += line.second -> length + 1;
                to.way.push_back(to.vertex);
                // std::cout << "Push to next " << to.vertex - 332 << " new value=: " << to.value << std::endl;

                q.push(to);
            }
        }
    }

    std::vector<int> shortestWayToCity = showWay();

    reverse(train -> need_way.begin(), train -> need_way.end());
    for (auto i : shortestWayToCity) {
        train -> need_way.push_back(i);
    }


    train -> need_way.pop_back();


    std::cout << "Need way: " << std::endl;
    for (auto i : train -> need_way) {
        std::cout << i - 332 << " ";
    }
    std::cout << std::endl;



}


void trainIteration(Train* train) {

    json message;
    message["train_idx"] = train -> idx;

    /*
    std::cout << std::endl;
    std::cout << "In iteration; idx=" << train -> idx << " train pos= " << train -> position << std::endl;
    std::cout << "vertex1= " << train -> line -> points[0] -> idx << " vertex2= " << train -> line -> points[1] -> idx << " way back=" << train -> need_way.back() << std::endl;

    std::cout << "Line idx=" << train -> line -> idx << std::endl;
    */

    /// EXAMPLE Network::send(Action::TURN, message);

    if (train -> position == 0 &&
            train -> line -> points[1] -> idx == train -> need_way.back()) {

        /// go speed = 1

        // std::cout << "Try set speed= 1" << std::endl;

        message["speed"] = 1;
        message["line_idx"] = train -> line -> idx;
        Network::send(Action::MOVE, message);

        return;
    }
    if (train -> position != 0 &&
            train -> line -> points[0] -> idx == train -> need_way.back()) {

        /// go speed = -1
        // std::cout << "Try set speed= -1" << std::endl;

        message["speed"] = -1;
        message["line_idx"] = train -> line -> idx;
        Network::send(Action::MOVE, message);

        return;
    }


    for (auto line : Database::lines) {

        if (line.second -> points[0] -> idx == getTrainVertex(train) &&
                line.second -> points[1] -> idx == train -> need_way.back()) {

                    /// change line to this;

                    // std::cout << "Try change line= " << line.second -> idx << std::endl;

                    message["speed"] = 1;
                    message["line_idx"] = line.second -> idx;

                    // std::cout << "Message=" << message << std::endl;

                    Network::send(Action::MOVE, message);

                    return;
                }

        if (line.second -> points[1] -> idx == getTrainVertex(train) &&
                line.second -> points[0] -> idx == train -> need_way.back()) {

                    /// change line to this;
                   //  std::cout << "Try change line= " << line.second -> idx << std::endl;

                    message["speed"] = -1;
                    message["line_idx"] = line.second -> idx;

                    // std::cout << "Message=" << message << std::endl;

                    Network::send(Action::MOVE, message);

                    return;
                }
    }



}

