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
#include <unordered_map>
#include <set>

int f[300000];
bool blockedEdges[300000];
bool blockedVertex[300000];
int lineFict[300000];

std::unordered_map<int, std::set<int> > timeTableVertex;
std::unordered_map<int, std::set<int> > timeTableLine;


std::unordered_map<int, long long> fLine[50];
std::unordered_map<int, long long> fVertex[50];

// std::vector<int> globalWays[300000];

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


bool PlayerController::isTown(int vertex) {

    if (vertex == PlayerController::playerTown -> point -> idx) return true;
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
    return finalWay;
}


int getTrainVertex(Train* train) {
    if (train->position == 0) return train->line->points[0]->idx;
    else return train->line->points[1]->idx;
}


void clearEdges() {
    for (auto line : Database::lines) blockedEdges[line.second -> idx] = false;
    for (auto train : Database::trains) {
        if (train.second -> position != 0 &&
             train.second -> position != train.second -> line -> length) blockedEdges[train.second -> line -> idx] = true;
    }
}


void clearLinesAndVertex(int marketCount) {

    for (int i = 0; i < marketCount; i++) {
        fVertex[i].clear();
        fLine[i].clear();
    }
}


void checkOnBestWay(El &now, std::vector<int> &bestWay, int &bestCost) {

    if (now.value - now.spent > bestCost && now.way.size() > 2) {

        bestCost = now.value - now.spent;
        bestWay = now.way;
    }

    return;
}


void PlayerController::addTimeTable(std::vector<int> needWay) {

    int timeNow = 0;
    for (int i = 0; i < needWay.size(); i++) {

        if (isMarket(needWay[i])) std::cout << "Is market! ver=" << needWay[i] << std::endl;

        if (i == 0) {
            int l1 = timeTableVertex[needWay[i]].size();
            if (isTown(needWay[i]) == false) timeTableVertex[needWay[i]].insert(timeNow + PlayerController::tickNow);
            if (l1 == timeTableVertex[needWay[i]].size() && isTown(needWay[i]) == false) {
                std::cout << "ERROR0" << std::endl;
                std::cout << "ERROR0" << std::endl;
                std::cout << "ERROR0" << std::endl;
                std::cout << "ERROR0" << std::endl;
            }
            /// std::cout << "BLOCK " << needWay[i] << " time=" << timeNow + PlayerController::tickNow << std::endl;
        }
        else {

            for (auto line : Database::lines) {
                if (line.second -> points[0] -> idx == needWay[i - 1]
                 && line.second -> points[1] -> idx == needWay[i]) {

                            int len = line.second -> length;
                            for (int j = 1; j < len; j++) {

                                timeNow++;
                                int l1 = timeTableLine[line.second -> idx * 1000 + j].size();
                                timeTableLine[line.second -> idx * 1000 + j].insert(timeNow + PlayerController::tickNow);
                                if (l1 == timeTableLine[line.second -> idx * 1000 + j].size()) {
                                    std::cout << "ERROR1" << std::endl;
                                    std::cout << "ERROR1" << std::endl;
                                    std::cout << "ERROR1" << std::endl;
                                    std::cout << "ERROR1" << std::endl;
                                }
                            }
                            timeNow++;

                            int l1 = timeTableVertex[needWay[i]].size();
                            if (isTown(needWay[i]) == false) timeTableVertex[needWay[i]].insert(timeNow + PlayerController::tickNow);
                            if (l1 == timeTableVertex[needWay[i]].size() && isTown(needWay[i]) == false) {
                                std::cout << "ERROR12" << std::endl;
                                std::cout << "ERROR12" << std::endl;
                                std::cout << "ERROR12" << std::endl;
                                std::cout << "ERROR12" << std::endl;
                            }

                            /// std::cout << "BLOCK " << needWay[i] << " time=" << timeNow + PlayerController::tickNow << std::endl;

                            break;
                        }
                else if (line.second -> points[1] -> idx == needWay[i - 1]
                      && line.second -> points[0] -> idx == needWay[i]) {

                            int len = line.second -> length;
                            for (int j = len - 1; j >= 1; j--) {

                                timeNow++;
                                int l1 = timeTableLine[line.second -> idx * 1000 + j].size();
                                timeTableLine[line.second -> idx * 1000 + j].insert(timeNow + PlayerController::tickNow);
                                if (l1 == timeTableLine[line.second -> idx * 1000 + j].size()) {
                                    std::cout << "ERROR2" << std::endl;
                                    std::cout << "ERROR2" << std::endl;
                                    std::cout << "ERROR2" << std::endl;
                                    std::cout << "ERROR2" << std::endl;
                                }
                            }
                            timeNow++;

                            int l1 = timeTableVertex[needWay[i]].size();
                            if (isTown(needWay[i]) == false) timeTableVertex[needWay[i]].insert(timeNow + PlayerController::tickNow);
                            if (l1 == timeTableVertex[needWay[i]].size() && isTown(needWay[i]) == false) {
                                std::cout << "ERROR22" << std::endl;
                                std::cout << "ERROR22" << std::endl;
                                std::cout << "ERROR22" << std::endl;
                                std::cout << "ERROR22" << std::endl;
                            }

                            /// std::cout << "BLOCK " << needWay[i] << " time=" << timeNow + PlayerController::tickNow << std::endl;

                            break;
                        }
            }
        }
    }
}


bool PlayerController::isFreeTimeTable(El now) {

    std::set<int> ::iterator t;

    if (now.line == nullptr) {

        t = timeTableVertex[now.vertex].lower_bound(now.spent + PlayerController::tickNow);

        // if (t == timeTableVertex[now.vertex].end()) return true;
        if (t != timeTableVertex[now.vertex].end() && *t == now.spent + PlayerController::tickNow) return false;


        t = timeTableVertex[now.vertex].lower_bound(now.spent + PlayerController::tickNow + 1);
        // if (*t != now.spent + PlayerController::tickNow + 1) return true;
        if (t != timeTableVertex[now.vertex].end() && *t == now.spent + PlayerController::tickNow + 1) return false;

    }
    else {

        t = timeTableLine[now.line -> idx * 1000 + now.position].lower_bound(now.spent + PlayerController::tickNow);

        // if (t == timeTableLine[now.line -> idx * 1000 + now.position].end()) return true;
        if (t != timeTableLine[now.line -> idx * 1000 + now.position].end() && *t == now.spent + PlayerController::tickNow) return false;


        t = timeTableLine[now.line -> idx * 1000 + now.position].lower_bound(now.spent + PlayerController::tickNow + 1);
        // if (t == timeTableLine[now.line -> idx * 1000 + now.position].end()) return true;
        if (t != timeTableLine[now.line -> idx * 1000 + now.position].end() && *t == now.spent + PlayerController::tickNow + 1) return false;


    }



    return true;
}

void PlayerController::clearVertex() {
    for (auto point : Database::points) blockedVertex[point.second -> idx] = false;
    for (auto train : Database::trains) {
        if (train.second -> position != 0 &&
             train.second -> position != train.second -> line -> length) {
                 if (train.second -> speed == 1) blockedVertex[train.second -> line -> points[1] -> idx] = true;
                 else blockedVertex[train.second -> line -> points[0] -> idx] = true;
             }

        if (train.second -> position == 0) blockedEdges[train.second -> line -> points[0] -> idx] = true;
        if (train.second -> position == train.second -> line -> length) blockedEdges[train.second -> line -> points[1] -> idx] = true;
    }

    if (playerTown != nullptr) blockedEdges[PlayerController::playerTown -> point -> idx] = false;
}


void logTrain(Train* train) {

    /// std::cout << "Train idx=" << train_now -> idx << " lvl=" << train_now -> level << " move type=" << train_now -> move_type << " point=" << train_now -> line -> points[0] -> idx << std::endl;



    if (train -> position == 0) std::cout << "Train idx=" << train -> idx << " lvl=" << train -> level << " point=" << train -> line -> points[0] -> idx << std::endl;
    else if (train -> position == train -> line -> length) std::cout << "Train idx=" << train -> idx << " lvl=" << train -> level << " point=" << train -> line -> points[1] -> idx << std::endl;
    else std::cout << "Train idx=" << train -> idx << " lvl=" << train -> level << " line=" << train -> line -> idx << std::endl;

}


void addLineFict(std::vector<int>& way){

    for (int i = 1; i < way.size(); i++) {
        int a = way[i];
        int b = way[i - 1];

        for (auto line : Database::lines) {
            if (line.second -> points[0] -> idx == a && line.second -> points[1] -> idx == b) lineFict[line.second -> idx] += 2;
            if (line.second -> points[1] -> idx == a && line.second -> points[0] -> idx == b) lineFict[line.second -> idx] += 2;
        }
    }

}

void trainIteration(Train*);
void tryTrainUpdate(Train*);

void PlayerController::update() {

    int _count = 0;
    for (auto train : PlayerController::playerTrains) {
        if (_count < 4) train -> move_type = 1;
        else  train -> move_type = 3;

        _count += 1;
    }

    clearEdges();
    clearVertex();
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


    //TODO: iterate over PlayerController::playerTrains
    for (auto train_now : PlayerController::playerTrains) {

        tryTrainUpdate(train_now);
        if (train_now -> cooldown != 0) {

            std::cout << "Train idx=" << train_now -> idx << " lvl=" << train_now -> level << " IN COOL DOWN! cooldown=" << train_now -> cooldown << std::endl;
            train_now -> needWay.clear();
            continue;
        }

        /// std::cout << "Train idx=" << train_now -> idx << " lvl=" << train_now -> level << " move type=" << train_now -> move_type << " point=" << train_now -> line -> points[0] -> idx << std::endl;
        logTrain(train_now);

        if (train_now->move_type == 0) {
            train_now->move_type = 1;
            //TODO: move_type of trains + destiny;
        }
        int train_pos = train_now->position;
        if (train_pos == 0 || train_pos == train_now->line->length) {

            if (train_now->needWay.size() == 0) {

                if (train_now -> move_type == 1) train_now -> needWay = PlayerController::trainWayToProducts(train_now).first;
                else if (train_now -> move_type == 2) {

                    if (train_now->level <= 2 || playerTown->level <= 2) train_now->needWay = PlayerController::trainWayToStorage(train_now).first;
                    else train_now->needWay = PlayerController::trainWayToProducts(train_now).first;
                }
            }
        }
    }

    /// first for trains, which go to Town
    for (auto train_now : PlayerController::playerTrains) {
        if (train_now -> needWay.size() != 0 && train_now -> needWay.back() == PlayerController::playerTown -> point -> idx) trainIteration(train_now);
    }
    /// then for other
    for (auto train_now : PlayerController::playerTrains) {
        if (train_now -> needWay.size() != 0 && train_now -> needWay.back() != PlayerController::playerTown -> point -> idx) trainIteration(train_now);
    }

    PlayerController::isMapUpdated = false;
    Network::send(Action::TURN);
    Network::send(Action::MAP, {{"layer", 1}});
}


std::pair<std::vector<int>, int> PlayerController::trainWayToProducts(Train* train) {

    std::vector<int> finalWay;
    int bestCost = -1e9;

    std::priority_queue<El> q;
    El start;

    int fixNumber = 1e9;
    int marketCount = PlayerController::markets.size();


    int summary1 = 0, summary2 = 0;


    std::vector<int> needMarkets;
    for (int i = 0; i < PlayerController::markets.size(); i++) {
        needMarkets.push_back(PlayerController::markets[i] -> point -> idx);
    }
    sort(needMarkets.begin(), needMarkets.end());

    do{
        /// needMarket - need perm;

        clearLinesAndVertex(marketCount);

        // start.way.push_back(start.vertex);
        start.vertex = getTrainVertex(train);
        start.marketPos = 0;
        start.maxValue = train -> goods_capacity;

        q.push(start);

        while(!q.empty()) {

            El to, now;

            now = q.top();
            q.pop();



            if (now.line != nullptr) {

                if (fLine[now.marketPos][now.line -> idx * 1000 + now.position] >= now.value - now.spent + fixNumber) continue;
                fLine[now.marketPos][now.line -> idx * 1000 + now.position] = now.value - now.spent + fixNumber;

                if (now.position == 0) {
                    now.vertex = now.line -> points[0] -> idx;
                    now.line = nullptr;
                }

                else if (now.position == now.line -> length) {
                    now.vertex = now.line -> points[1] -> idx;
                    now.line = nullptr;
                }
                else {

                    /// раписание. Добавить
                    if (!isFreeTimeTable(now)) continue;

                    now.spent += 1;
                    now.position += 1;
                    q.push(now);

                    now.position -= 2;
                    q.push(now);

                    continue;
                }
            }

            /// расписание. Добавить
            if (!isFreeTimeTable(now)) continue;
            if (isStorage(now.vertex) && now.value == 0) continue;

            if (fVertex[now.marketPos][now.vertex] >= now.value - now.spent + fixNumber) continue;
            fVertex[now.marketPos][now.vertex] = now.value - now.spent + fixNumber;


            /*
            if (now.marketPos != 0) {

                std::cout << std::endl;
                if (isTown(now.vertex)) std::cout << "Its town! size=" << now.way.size() << " ";
                std::cout << " vertex=" << now.vertex << " value=" << now.value << " spent=" << now.spent << " marketPos=" << now.marketPos << std::endl;

                std::cout << "Best now=" << finalWay.size() << std::endl;
            }
            */

            now.way.push_back(now.vertex);
            if (isTown(now.vertex)) checkOnBestWay(now, finalWay, bestCost);

            if (now.marketPos != marketCount && now.vertex == needMarkets[now.marketPos]) {
                now.marketPos += 1;
                now.value += marketProduct(now.vertex);
                now.value = std::min(now.value, now.maxValue);
            }

            for (auto line : Database::lines) {

                if (line.second -> points[0] -> idx == now.vertex) {

                    to = now;
                    to.line = line.second;
                    to.position = 1;
                    to.spent += 1;

                    q.push(to);
                }
                else if (line.second -> points[1] -> idx == now.vertex) {

                    to = now;
                    to.line = line.second;
                    to.position = line.second -> length - 1;
                    to.spent += 1;

                    q.push(to);
                }
            }

            /// +++ add - действие если мы в городе
            /// add - заполнение раписания
            /// add - проверка расписания
            /// +++ add - переменная, что отвечает за текущий ход для расписания.
            /// +++ add - обнуление служебных переменных fLine fVeertex.



        }

    } while (std::next_permutation(needMarkets.begin(), needMarkets.end()));


    std::cout << "END! idx=" << train -> idx << std::endl;
    std::cout << "BEST WAY===============" << std::endl;
    for (int i = 0; i < finalWay.size(); i++) std::cout << finalWay[i] << " ";
    std::cout << std::endl;

    addTimeTable(finalWay);

    reverse(finalWay.begin(), finalWay.end());
    if (finalWay.size()) finalWay.pop_back();


    return {finalWay, summary1 + summary2};
}




std::pair<std::vector<int>, int> PlayerController::trainWayToStorage(Train* train) {

    std::vector<int> finalWay;
    std::priority_queue<El> q;
    El start;

    int summary1 = 0;
    int summary2 = 0;
    std::vector<int> resultWay;

    // El::clearGraph();

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
                // to.spent += line.second->length + lineFict[line.second -> idx];
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

    /// std::cout << "=============================================" << std::endl;
    /// ===========================================================================

    // El::clearGraph();
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
                /// to.spent += line.second->length + lineFict[line.second -> idx];
                to.spent += line.second->length;
                to.way.push_back(to.vertex);

                q.push(to);
            }
        }
    }

    std::vector<int> shortestWayToCity = showWay(finalWay);

    resultWay += shortestWayToCity;
    reverse(resultWay.begin(), resultWay.end());
    resultWay.pop_back();

    /*
    std::cout << "Need way: " << std::endl;
    for (auto i : resultWay) {
        std::cout << i - 332 << " ";
    }
    std::cout << std::endl;
    std::cout << "add fict " << std::endl;
    */

    addLineFict(resultWay);
    return {resultWay, summary1 + summary2};
}


void trainIteration(Train* train) {
    json message;

    if (train -> idx == 1) {

        std::cout << "In train need way idx=1; " << std::endl;
        for (int i = 0; i < train -> needWay.size(); i++) {
            std::cout << train -> needWay[i] << " ";
        }
        std::cout << std::endl;
    }

    if (train -> idx == 3) {

        std::cout << "In train need way idx=2; " << std::endl;
        for (int i = 0; i < train -> needWay.size(); i++) {
            std::cout << train -> needWay[i] << " ";
        }
        std::cout << std::endl;
    }


    message["train_idx"] = train->idx;

    if (train->position == 0 &&
        train->line->points[1]->idx == train->needWay.back()) {

        /*
        if (blockedEdges[train->line->idx] == true) return;
        if (blockedVertex[train->needWay.back()]) return;

        blockedEdges[train->line->idx] = true;
        blockedVertex[train->needWay.back()] = true;
        */

        message["speed"] = 1;
        message["line_idx"] = train->line->idx;
        Network::send(Action::MOVE, message);

        train->needWay.pop_back();
        return;
    }


    if (train->position == train -> line -> length &&
        train->line->points[0]->idx == train->needWay.back()) {

        /*
        if (blockedEdges[train->line->idx] == true) return;
        if (blockedVertex[train->needWay.back()]) return;

        blockedEdges[train->line->idx] = true;
        blockedVertex[train->needWay.back()] = true;
        */

        message["speed"] = -1;
        message["line_idx"] = train->line->idx;
        Network::send(Action::MOVE, message);

        train->needWay.pop_back();
        return;
    }

    if (train -> position != 0 && train -> position != train -> line -> length) return;

    //TODO: store way as array of lines
    for (auto line : Database::lines) {
        if (line.second->points[0]->idx == getTrainVertex(train) &&
            line.second->points[1]->idx == train->needWay.back()) {

            /*
            if (blockedEdges[line.second->idx] == true) return;
            if (blockedVertex[train->needWay.back()]) return;

            blockedEdges[line.second->idx] = true;
            blockedVertex[train->needWay.back()] = true;
            */

            message["speed"] = 1;
            message["line_idx"] = line.second->idx;
            Network::send(Action::MOVE, message);

            train->needWay.pop_back();
            return;
        }

        if (line.second->points[1]->idx == getTrainVertex(train) &&
            line.second->points[0]->idx == train->needWay.back()) {

            /*
            if (blockedEdges[line.second->idx] == true) return;
            if (blockedVertex[train->needWay.back()]) return;

            blockedEdges[line.second->idx] = true;
            blockedVertex[train->needWay.back()] = true;
            */

            message["speed"] = -1;
            message["line_idx"] = line.second->idx;
            Network::send(Action::MOVE, message);


            train->needWay.pop_back();
            return;
        }
    }
}


void PlayerController::tryTrainUpdate(Train* mainTrain) {

    if (playerTown->armor >= 70 && mainTrain->level == 1) {

        json message;

        std::vector<int> a{};
        std::vector<int> b{mainTrain->idx};

        message["posts"] = a;
        message["trains"] = b;

        mainTrain->level += 1;
        playerTown->armor -= 40;

        std::cout << "UPGRADE TO LEVEL 2, MESSAGE=" << message << std::endl;
        Network::send(Action::UPGRADE, message);
    }

    if (playerTown->armor >= 110 && mainTrain->level == 2) {

        json message;

        std::vector<int> a{};
        std::vector<int> b{mainTrain->idx};

        message["posts"] = a;
        message["trains"] = b;

        mainTrain->level += 1;
        playerTown->armor -= 80;

        std::cout << "UPGRADE TO LEVEL 3, MESSAGE=" << message << std::endl;

        Network::send(Action::UPGRADE, message);
    }

    if (mainTrain->level == 3) {
        if (playerTown->armor >= 130 && playerTown->level == 1) {
            json message;

            std::vector<int> a{playerTown->idx};
            std::vector<int> b{};

            message["posts"] = a;
            message["trains"] = b;

            std::cout << "UPGRADE TOWN TO LEVEL 2, MESSAGE=" << message << std::endl;

            playerTown->level += 1;
            playerTown->armor -= 100;

            Network::send(Action::UPGRADE, message);
        }
        if (playerTown->armor >= 230 && playerTown->level == 2) {
            json message;

            std::vector<int> a{playerTown->idx};
            std::vector<int> b{};

            message["posts"] = a;
            message["trains"] = b;

            std::cout << "UPGRADE TOWN TO LEVEL 3, MESSAGE=" << message << std::endl;

            playerTown->level += 1;
            playerTown->armor -= 200;
            Network::send(Action::UPGRADE, message);
        }
    }

}
