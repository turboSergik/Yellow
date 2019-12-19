//
// Created by Олег Бобров on 10.12.2019.
//

#include "PlayerController.hpp"
#include "../static/Database.h"
#include "../network/Network.hpp"
#include "../static/Time.h"

#include <queue>
#include <bitset>
#include <algorithm>
#include <iostream>

int f[300000];
std::vector<int> way[300000];

class El {
//TODO: remove this structure or make private
public:
    std::vector<int> way;
    std::bitset<2000> used;

    int value = 0;
    int spent = 0;
    int vertex = 0;
    int maxValue = 0;
    int startVertex = 0;
};

//TODO: make friend or comparator function
bool operator< (El left, El right) {
    if (left.value < right.value || (left.value == right.value && left.spent > right.spent)) return true;
    return false;
}

//TODO: point has post field
bool isStorage(int vertex) {
    for (auto ver : Database::posts) {

        if (ver.second->point->idx == vertex && ver.second->type == 3) return true;
    }
    return false;
}
//TODO: point has post field
bool isMarket(int vertex) {
    for (auto ver : Database::posts) {
        //TODO: use enum to compare
        if (ver.second->point->idx == vertex && ver.second->type == 2) return true;
    }
    return false;
}
//TODO: point has post field or I can store all markets
int marketProduct(int vertex) {
    for (auto ver : Database::posts)  if (ver.second->point->idx == vertex) {
            auto *townObj = static_cast<Market* >(ver.second);
            return townObj->product;
        }
}


std::vector<int> showWay() {
    for (auto ver : Database::posts) if (ver.second->type == 1) return way[ver.second->point->idx];
}



void changeDist() {
    //TODO: store fields in point class
    for (auto vertexNow : Database::points) {
        f[vertexNow.second->idx] = -1e9;
        way[vertexNow.second->idx].clear();
    }
}


int getTrainVertex(Train* train) {
    if (train->position == 0) return train->line->points[0]->idx;
    else return train->line->points[1]->idx;
}

//TODO: move to class methods
void calcTrainWay(Train*);
void trainIteration(Train*);

void PlayerController::update() {
    PlayerController::timeFromLastTurn += Time::deltaTime;
    if (PlayerController::isMapUpdated && PlayerController::timeFromLastTurn > PlayerController::waitingTime) {
        //TODO: most of gameLogic
        strategyIteration();
        PlayerController::timeFromLastTurn = 0;
    }
}

void PlayerController::strategyIteration() {
    std::cout << "=====================================" << std::endl;
    std::cout << "Product=" << playerTown->product << " Population=" << playerTown->population << std::endl;

    //TODO: iterate over PlayerController::playerTrains
    for (auto train_now : Database::trains) {
        if (train_now.second->move_type == 0) {
            train_now.second->move_type = 1;
            //TODO: move_type of trains + destiny;
        }
        int train_pos = train_now.second->position;
        if (train_pos == 0 || train_pos == train_now.second->line->length) {
            train_now.second->move_type = 2;
            if (train_now.second->need_way.size() == 0) {
                calcTrainWay(train_now.second);
            }
            trainIteration(train_now.second);
            train_now.second->need_way.pop_back();
        }

    }
    PlayerController::isMapUpdated = false;
    Network::send(Action::TURN);
    Network::send(Action::MAP, {{"layer", 1}});
}

void calcTrainWay(Train* train) {
    std::priority_queue<El> q;
    El start;
    changeDist();
    for (auto pointNow : Database::posts) {
        if (pointNow.second->type == 2) {
            auto *marketObj = static_cast<Market*>(pointNow.second);
            start.vertex = marketObj->point->idx;
            start.maxValue = train->goods_capacity;
            start.value = std::min(marketObj->product, start.maxValue);
            start.startVertex = start.vertex;
            q.push(start);
        }
    }
    while(!q.empty()) {
        El to, now = q.top();
        q.pop();
        if (isStorage(now.vertex) == true) continue;

        if (isMarket(now.vertex) == true && now.used[now.vertex] == 0) {
            now.used[now.vertex] = 1;
            now.value += marketProduct(now.vertex);
            now.value = std::min(now.maxValue, now.value);
        }
        if (f[now.vertex] >= now.value - now.spent) continue;
        f[now.vertex] = now.value - now.spent;

        way[now.vertex].clear();
        way[now.vertex].push_back(now.startVertex);
        for (int i = 0; i < now.way.size(); i++) way[now.vertex].push_back(now.way[i]);
        for (auto line : Database::lines) {
            if (line.second->points[0]->idx == now.vertex) {
                to = now;
                to.vertex = line.second->points[1]->idx;
                to.spent += line.second->length + 1;
                to.way.push_back(to.vertex);
                // std::cout << "Push to next " << to.vertex - 332 << " new value=: " << to.value << std::endl;
                q.push(to);
            }
            if (line.second->points[1]->idx == now.vertex) {
                to = now;
                to.vertex = line.second->points[0]->idx;
                to.spent += line.second->length + 1;
                to.way.push_back(to.vertex);
                // std::cout << "Push to next " << to.vertex - 332 << " new value=: " << to.value << std::endl;
                q.push(to);
            }
        }
    }
    std::cout << "=============================================" << std::endl;
    std::cout << "=============================================" << std::endl;

    train->need_way = showWay();
    if (train->need_way.size() == 0) return;

    changeDist();

    El startShortestWay;
    startShortestWay.vertex = train->need_way[0];

    q.push(startShortestWay);

    while(!q.empty()) {
        El to, now = q.top();
        q.pop();

        if (f[now.vertex] >= now.value - now.spent) continue;
        f[now.vertex] = now.value - now.spent;

        way[now.vertex] = now.way;
        for (auto line : Database::lines) {

            if (line.second->points[0]->idx == now.vertex) {

                to = now;
                to.vertex = line.second->points[1]->idx;
                to.spent += line.second->length + 1;
                to.way.push_back(to.vertex);

                q.push(to);
            }
            if (line.second->points[1]->idx == now.vertex) {

                to = now;
                to.vertex = line.second->points[0]->idx;
                to.spent += line.second->length + 1;
                to.way.push_back(to.vertex);

                q.push(to);
            }
        }
    }

    std::vector<int> shortestWayToCity = showWay();
    reverse(train->need_way.begin(), train->need_way.end());
    for (auto i : shortestWayToCity) {
        train->need_way.push_back(i);
    }
    train->need_way.pop_back();
    std::cout << "Need way: " << std::endl;
    for (auto i : train->need_way) {
        std::cout << i - 332 << " ";
    }
    std::cout << std::endl;
}


void trainIteration(Train* train) {
    json message;
    message["train_idx"] = train->idx;

    if (train->position == 0 &&
        train->line->points[1]->idx == train->need_way.back()) {

        message["speed"] = 1;
        message["line_idx"] = train->line->idx;
        Network::send(Action::MOVE, message);

        return;
    }
    if (train->position != 0 &&
        train->line->points[0]->idx == train->need_way.back()) {
        message["speed"] = -1;
        message["line_idx"] = train->line->idx;
        Network::send(Action::MOVE, message);

        return;
    }
    //TODO: store way as array of lines
    for (auto line : Database::lines) {
        if (line.second->points[0]->idx == getTrainVertex(train) &&
            line.second->points[1]->idx == train->need_way.back()) {
            message["speed"] = 1;
            message["line_idx"] = line.second->idx;
            Network::send(Action::MOVE, message);

            return;
        }

        if (line.second->points[1]->idx == getTrainVertex(train) &&
            line.second->points[0]->idx == train->need_way.back()) {
            message["speed"] = -1;
            message["line_idx"] = line.second->idx;
            Network::send(Action::MOVE, message);

            return;
        }
    }
}