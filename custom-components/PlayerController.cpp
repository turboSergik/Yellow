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
// std::vector<int> globalWays[300000];

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


    static void clearGraph() {
    /// +TODO: store fields in point class
        for (auto vertexNow : Database::points) {
            f[vertexNow.second->idx] = -1e9;
            //globalWays[vertexNow.second->idx].clear();
        }
}
};

//TODO: make friend or comparator function
bool operator< (El left, El right) {
    if (left.value < right.value || (left.value == right.value && left.spent > right.spent)) return true;
    return false;
}


std::vector<int> operator+= (std::vector<int> &left, std::vector<int> &right) {
    for (int i = 0; i < right.size(); i++) left.push_back(right[i]);
    return left;
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

bool isTown(int vertex) {
    for (auto ver : Database::posts) {
        //TODO: use enum to compare
        if (ver.second->point->idx == vertex && ver.second->type == 1) return true;
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


int storageProduct(int vertex) {
    for (auto ver : Database::posts)  if (ver.second->point->idx == vertex) {
            auto *townObj = static_cast<Storage* >(ver.second);
            return townObj->armor;
        }
}


std::vector<int> showWay(std::vector<int> &finalWay) {
   // std::cout << "Way: ";
   // for (auto ver : finalWay) std::cout << ver << " ";
   // std::cout << std::endl;
    return finalWay;
}

int getTrainVertex(Train* train) {
    if (train->position == 0) return train->line->points[0]->idx;
    else return train->line->points[1]->idx;
}

//TODO: move to class methods
/// void calcTrainWay(Train*);
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
    std::cout << "Product=" << playerTown->product <<  " Armor=" << playerTown->armor  <<  " Population=" << playerTown->population << std::endl;

    Train* mainTrain = playerTrains[0];
    std::cout << "Train goods=" << mainTrain->goods << " level=" << mainTrain->level << " City_level=" << playerTown->level << std::endl;

    if (playerTown->armor >= 40 && mainTrain->level == 1) {

        json message;

        std::vector<int> a{};
        std::vector<int> b{mainTrain->idx};

        message["posts"] = a;
        message["trains"] = b;

        mainTrain->level += 1;
        std::cout << "UPGRADE TO LEVEL 2, MESSAGE=" << message << std::endl;
        Network::send(Action::UPGRADE, message);
    }

    if (playerTown->armor >= 80 && mainTrain->level == 2) {

        json message;

        std::vector<int> a{};
        std::vector<int> b{mainTrain->idx};

        message["posts"] = a;
        message["trains"] = b;

        mainTrain->level += 1;
        std::cout << "UPGRADE TO LEVEL 3, MESSAGE=" << message << std::endl;

        Network::send(Action::UPGRADE, message);
    }

    if (mainTrain->level == 3) {
        if (playerTown->armor >= 100 && playerTown->level == 1) {
            json message;

            std::vector<int> a{playerTown->idx};
            std::vector<int> b{};

            message["posts"] = a;
            message["trains"] = b;

            std::cout << "UPGRADE TOWN TO LEVEL 2, MESSAGE=" << message << std::endl;

            playerTown->level += 1;
            Network::send(Action::UPGRADE, message);
        }
        if (playerTown->armor >= 200 && playerTown->level == 2) {
            json message;

            std::vector<int> a{playerTown->idx};
            std::vector<int> b{};

            message["posts"] = a;
            message["trains"] = b;

            std::cout << "UPGRADE TOWN TO LEVEL 3, MESSAGE=" << message << std::endl;

            playerTown->level += 1;
            Network::send(Action::UPGRADE, message);
        }
    }



    //TODO: iterate over PlayerController::playerTrains
    for (auto train_now : PlayerController::playerTrains) {
        if (train_now->move_type == 0) {
            train_now->move_type = 1;
            //TODO: move_type of trains + destiny;
        }
        int train_pos = train_now->position;
        if (train_pos == 0 || train_pos == train_now->line->length) {

            if (train_now->need_way.size() == 0) {

                if (train_now->level <= 2 || playerTown->level <= 2) train_now->need_way = PlayerController::trainWayToStorage(train_now).first;
                else train_now->need_way = PlayerController::trainWayToProducts(train_now).first;
            }
            trainIteration(train_now);
            train_now->need_way.pop_back();
        }

    }
    PlayerController::isMapUpdated = false;
    Network::send(Action::TURN);
    Network::send(Action::MAP, {{"layer", 1}});
}

std::pair<std::vector<int>, int> PlayerController::trainWayToProducts(Train* train) {

    std::vector<int> finalWay;
    std::priority_queue<El> q;
    El start;

    int summary1 = 0;
    int summary2 = 0;
    std::vector<int> resultWay;

    El::clearGraph();

    for (auto pointNow : PlayerController::markets) {
        auto *marketObj = static_cast<Market*>(pointNow);
        start.vertex = marketObj->point->idx;
        start.maxValue = train->goods_capacity;
        start.value = std::min(marketObj->product, start.maxValue);
        start.startVertex = start.vertex;
        q.push(start);
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

        /*
        globalWays[now.vertex].clear();
        globalWays[now.vertex].push_back(now.startVertex);
        for (int i = 0; i < now.way.size(); i++) globalWays[now.vertex].push_back(now.way[i]);
        */

        /// if (isTown(now.vertex) == true){
        if (now.vertex == PlayerController::playerTown->point->idx){


            finalWay.clear();
            finalWay.push_back(now.startVertex);
            finalWay += now.way;

            summary1 = now.spent;
        }

        for (auto line : Database::lines) {
            if (line.second->points[0]->idx == now.vertex) {
                to = now;
                to.vertex = line.second->points[1]->idx;
                to.spent += line.second->length;
                to.way.push_back(to.vertex);
                q.push(to);
            }
            if (line.second->points[1]->idx == now.vertex) {
                to = now;
                to.vertex = line.second->points[0]->idx;
                to.spent += line.second->length;
                to.way.push_back(to.vertex);
                q.push(to);
            }
        }
    }
    std::cout << "=============================================" << std::endl;
    std::cout << "=============================================" << std::endl;

    resultWay = showWay(finalWay);
    reverse(resultWay.begin(), resultWay.end());

    if (resultWay.size() == 0){
        std::cout << "ERROR ON SHORTEST WAY TO MARKET" << std::endl;
        return {};
    }

    El::clearGraph();
    finalWay.clear();

    El startShortestWay;
    startShortestWay.vertex = resultWay.back();

    q.push(startShortestWay);


    while(!q.empty()) {
        El to, now = q.top();
        q.pop();

        if (f[now.vertex] >= now.value - now.spent) continue;
        f[now.vertex] = now.value - now.spent;

        if (now.vertex == PlayerController::playerTown->point->idx){
            finalWay = now.way;

            summary2 = now.spent;
        }

        for (auto line : Database::lines) {

            if (line.second->points[0]->idx == now.vertex) {

                to = now;
                to.vertex = line.second->points[1]->idx;
                to.spent += line.second->length;
                to.way.push_back(to.vertex);

                q.push(to);
            }
            if (line.second->points[1]->idx == now.vertex) {

                to = now;
                to.vertex = line.second->points[0]->idx;
                to.spent += line.second->length ;
                to.way.push_back(to.vertex);

                q.push(to);
            }
        }
    }

    std::vector<int> shortestWayToCity = showWay(finalWay);

    resultWay += shortestWayToCity;
    reverse(resultWay.begin(), resultWay.end());
    resultWay.pop_back();


    std::cout << "Need way: " << std::endl;
    for (auto i : resultWay) {
        std::cout << i - 332 << " ";
    }
    std::cout << std::endl;
    /// std::cout << "Summary=" << summary1 + summary2 << std::endl;

    return {resultWay, summary1 + summary2};
}




std::pair<std::vector<int>, int> PlayerController::trainWayToStorage(Train* train) {

    std::vector<int> finalWay;
    std::priority_queue<El> q;
    El start;

    int summary1 = 0;
    int summary2 = 0;
    std::vector<int> resultWay;

    El::clearGraph();

    for (auto pointNow : PlayerController::storages) {
        auto *storageObj = static_cast<Storage*>(pointNow);
        start.vertex = storageObj->point->idx;
        start.maxValue = train->goods_capacity;
        start.value = std::min(storageObj->armor, start.maxValue);
        start.startVertex = start.vertex;
        q.push(start);
    }

    while(!q.empty()) {
        El to, now = q.top();
        q.pop();

        if (isMarket(now.vertex) == true) {
            continue;
        }

        if (isStorage(now.vertex) == true && now.used[now.vertex] == 0) {
            now.used[now.vertex] = 1;
            now.value += storageProduct(now.vertex);
            now.value = std::min(now.maxValue, now.value);
        }
        if (f[now.vertex] >= now.value - now.spent) continue;
        f[now.vertex] = now.value - now.spent;

        if (now.vertex == PlayerController::playerTown->point->idx){

            finalWay.clear();
            finalWay.push_back(now.startVertex);
            finalWay += now.way;

            summary1 = now.spent;
        }

        for (auto line : Database::lines) {
            if (line.second->points[0]->idx == now.vertex) {
                to = now;
                to.vertex = line.second->points[1]->idx;
                to.spent += line.second->length;
                to.way.push_back(to.vertex);
                q.push(to);
            }
            if (line.second->points[1]->idx == now.vertex) {
                to = now;
                to.vertex = line.second->points[0]->idx;
                to.spent += line.second->length;
                to.way.push_back(to.vertex);
                q.push(to);
            }
        }
    }

    resultWay = showWay(finalWay);
    reverse(resultWay.begin(), resultWay.end());

    if (resultWay.size() == 0){
        std::cout << "ERROR ON SHORTEST WAY TO STORAGE" << std::endl;
        return {};
    }

    std::cout << "=============================================" << std::endl;
    std::cout << "=============================================" << std::endl;

    /// ===========================================================================

    El::clearGraph();
    finalWay.clear();

    El startShortestWay;
    startShortestWay.vertex = resultWay.back();

    q.push(startShortestWay);

    while(!q.empty()) {
        El to, now = q.top();
        q.pop();

        if (f[now.vertex] >= now.value - now.spent) continue;
        f[now.vertex] = now.value - now.spent;

        if (now.vertex == PlayerController::playerTown->point->idx){
            finalWay = now.way;

            summary2 = now.spent;
        }

        for (auto line : Database::lines) {

            if (line.second->points[0]->idx == now.vertex) {

                to = now;
                to.vertex = line.second->points[1]->idx;
                to.spent += line.second->length;
                to.way.push_back(to.vertex);

                q.push(to);
            }
            if (line.second->points[1]->idx == now.vertex) {

                to = now;
                to.vertex = line.second->points[0]->idx;
                to.spent += line.second->length ;
                to.way.push_back(to.vertex);

                q.push(to);
            }
        }
    }

    std::vector<int> shortestWayToCity = showWay(finalWay);

    resultWay += shortestWayToCity;
    reverse(resultWay.begin(), resultWay.end());
    resultWay.pop_back();

    std::cout << "Need way: " << std::endl;
    for (auto i : resultWay) {
        std::cout << i - 332 << " ";
    }
    std::cout << std::endl;

    return {resultWay, summary1 + summary2};
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
