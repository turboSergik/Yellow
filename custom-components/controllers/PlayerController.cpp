//
// Created by Олег Бобров on 10.12.2019.
//

#include "PlayerController.hpp"
#include "../../static/Database.h"
#include "../../network/Network.hpp"
#include "../../static/Time.h"
#include "../../utility/random.hpp"
#include "../Game.hpp"

#include <queue>
#include <bitset>
#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <set>

#include <ctime>
#include <fstream>

using Random = effolkronium::random_static;

std::unordered_map<int, std::set<int> > timeTableVertex;
std::unordered_map<int, std::set<int> > timeTableLine;


std::unordered_map<int, long long> fLine[50];
std::unordered_map<int, long long> fVertex[50];
int countType1 = 4;

void PlayerController::start() {
    Network::onMapResponse1.addListener<PlayerController, &PlayerController::onMapLayer1>(this);
    Network::onGamesResponse.addListener<PlayerController, &PlayerController::onGames>(this);
}

void PlayerController::onDestroy() {
    Network::onMapResponse1.removeListener<PlayerController, &PlayerController::onMapLayer1>(this);
    Network::onGamesResponse.removeListener<PlayerController, &PlayerController::onGames>(this);
}

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
bool isStorage(Point* point) {

    if (point -> post == nullptr) return false;
    if (point -> post -> type == PostType::STORAGE) return true;
    return false;
}
//TODO: point has post field


bool isMarket(Point* point) {
    if (point -> post == nullptr) return false;
    if (point -> post -> type == PostType::MARKET) return true;
    return false;
}


bool PlayerController::isTown(int vertex) {

    if (vertex == Database::playerTown -> point -> idx) return true;
    return false;
}

//TODO: point has post field or I can store all markets
int marketProduct(Point* point) {

    auto *marketObj = static_cast<Market* >(point -> post);
    return marketObj->productFict;
}


int storageArmor(Point* point) {

    auto *storageObj = static_cast<Storage* >(point -> post);
    return storageObj->armorFict;

}


std::vector<int> showWay(std::vector<int> &finalWay) {
    return finalWay;
}


int getTrainVertex(Train* train) {
    if (train->position == 0) return train->line->points[0]->idx;
    else return train->line->points[1]->idx;
}


Point* getTrainPoint(Train* train) {
    if (train->position == 0) return train->line->points[0];
    else return train->line->points[1];
}


void clearLinesAndVertex(int marketCount) {

    for (int i = 0; i < marketCount; i++) {
        fVertex[i].clear();
        fLine[i].clear();
    }
}


void PlayerController::trainUpgrade() {

    for (auto trainNow : Database::playerTrains) {
        if (trainNow -> level == 1) tryTrainUpdate(trainNow);
    }

    for (auto trainNow : Database::playerTrains) {
        if (trainNow -> level == 2) tryTrainUpdate(trainNow);
    }

    for (auto trainNow : Database::playerTrains) {
        if (trainNow -> level == 3) tryTrainUpdate(trainNow);
    }
    return;
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

        if (i == 0) {
            int l1 = timeTableVertex[needWay[i]].size();
            if (isTown(needWay[i]) == false) timeTableVertex[needWay[i]].insert(timeNow + Database::tick);
            if (l1 == timeTableVertex[needWay[i]].size() && isTown(needWay[i]) == false) {
                std::cout << "ERROR0" << std::endl;
                std::cout << "ERROR0" << std::endl;
                std::cout << "ERROR0" << std::endl;
                std::cout << "ERROR0" << std::endl;
            }
        }
        else {

            for (auto line : Database::lines) {
                if (line.second -> points[0] -> idx == needWay[i - 1]
                 && line.second -> points[1] -> idx == needWay[i]) {

                            int len = line.second -> length;
                            for (int j = 1; j < len; j++) {

                                timeNow++;
                                int l1 = timeTableLine[line.second -> idx * 1000 + j].size();
                                timeTableLine[line.second -> idx * 1000 + j].insert(timeNow + Database::tick);
                                if (l1 == timeTableLine[line.second -> idx * 1000 + j].size()) {
                                    std::cout << "ERROR1" << std::endl;
                                    std::cout << "ERROR1" << std::endl;
                                    std::cout << "ERROR1" << std::endl;
                                    std::cout << "ERROR1" << std::endl;
                                }
                            }
                            timeNow++;

                            int l1 = timeTableVertex[needWay[i]].size();
                            if (isTown(needWay[i]) == false) timeTableVertex[needWay[i]].insert(timeNow + Database::tick);
                            if (l1 == timeTableVertex[needWay[i]].size() && isTown(needWay[i]) == false) {
                                std::cout << "ERROR12" << std::endl;
                                std::cout << "ERROR12" << std::endl;
                                std::cout << "ERROR12" << std::endl;
                                std::cout << "ERROR12" << std::endl;
                            }

                            break;
                        }
                else if (line.second -> points[1] -> idx == needWay[i - 1]
                      && line.second -> points[0] -> idx == needWay[i]) {

                            int len = line.second -> length;
                            for (int j = len - 1; j >= 1; j--) {

                                timeNow++;
                                int l1 = timeTableLine[line.second -> idx * 1000 + j].size();
                                timeTableLine[line.second -> idx * 1000 + j].insert(timeNow + Database::tick);
                                if (l1 == timeTableLine[line.second -> idx * 1000 + j].size()) {
                                    std::cout << "ERROR2" << std::endl;
                                    std::cout << "ERROR2" << std::endl;
                                    std::cout << "ERROR2" << std::endl;
                                    std::cout << "ERROR2" << std::endl;
                                }
                            }
                            timeNow++;

                            int l1 = timeTableVertex[needWay[i]].size();
                            if (isTown(needWay[i]) == false) timeTableVertex[needWay[i]].insert(timeNow + Database::tick);
                            if (l1 == timeTableVertex[needWay[i]].size() && isTown(needWay[i]) == false) {
                                std::cout << "ERROR22" << std::endl;
                                std::cout << "ERROR22" << std::endl;
                                std::cout << "ERROR22" << std::endl;
                                std::cout << "ERROR22" << std::endl;
                            }

                            break;
                        }
            }
        }
    }
}


bool PlayerController::isFreeTimeTable(El now) {

    std::set<int> ::iterator t;

    if (now.line == nullptr) {

        t = timeTableVertex[now.vertex].lower_bound(now.spent + Database::tick);
        if (t != timeTableVertex[now.vertex].end() && *t == now.spent + Database::tick) return false;

        t = timeTableVertex[now.vertex].lower_bound(now.spent + Database::tick + 1);
        if (t != timeTableVertex[now.vertex].end() && *t == now.spent + Database::tick + 1) return false;
    }
    else {

        t = timeTableLine[now.line -> idx * 1000 + now.position].lower_bound(now.spent + Database::tick);
        if (t != timeTableLine[now.line -> idx * 1000 + now.position].end() && *t == now.spent + Database::tick) return false;

        t = timeTableLine[now.line -> idx * 1000 + now.position].lower_bound(now.spent + Database::tick + 1);
        if (t != timeTableLine[now.line -> idx * 1000 + now.position].end() && *t == now.spent + Database::tick + 1) return false;
    }

    return true;
}

void logTrain(Train* train) {


    if (train -> position == 0) std::cout << "Train idx=" << train -> idx << " lvl=" << train -> level << " point=" << train -> line -> points[0] -> idx << " move_type=" << train -> move_type;
    else if (train -> position == train -> line -> length) std::cout << "Train idx=" << train -> idx << " lvl=" << train -> level << " point=" << train -> line -> points[1] -> idx << " move_type=" << train -> move_type;
    else std::cout << "Train idx=" << train -> idx << " lvl=" << train -> level << " line=" << train -> line -> idx << " move_type=" << train -> move_type;
    std::cout << " size=" << train -> needWay.size();
    std::cout << std::endl;


}


void logTime(unsigned int time) {

    std::ofstream out;          // поток для записи
    out.open("test.txt", std::ios::out | std::ios::app); // окрываем файл для записи

    out << "Lost time= " << time << " mc" << std::endl;
    out.close();
}


void addFictProduct(std::vector<int> &way, int capacity){

    int now = 0;
    for (int i = way.size() - 1; i >= 0; i--) {

        for (auto post : Database::posts) if (post.second -> type == PostType::MARKET) {

            auto *marketObj = static_cast<Market* >(post.second);

            int add = capacity - now;
            marketObj -> productFict = std::max(0, marketObj -> productFict - std::min(marketObj -> product, add));

            now += std::min(add, marketObj -> product);
        }
    }
}


void addFictArmor(std::vector<int> &way, int capacity){

    int now = 0;
    for (int i = way.size() - 1; i >= 0; i--) {

        for (auto post : Database::posts) if (post.second -> type == PostType::STORAGE) {

            auto *storageObj = static_cast<Storage* >(post.second);

            int add = capacity - now;
            storageObj -> armorFict = std::max(0, storageObj -> armorFict - std::min(storageObj -> armor, add));

            now += std::min(add, storageObj -> armor);
        }
    }
}


void trainIteration(Train*);
void tryTrainUpdate(Train*);

void PlayerController::update() {
    int _count = 0;
    for (auto train : Database::playerTrains) {
        if (_count < countType1) train -> move_type = 1;
        else train -> move_type = 2;

        _count += 1;
    }

    PlayerController::timeFromLastTurn += Time::deltaTime;
    if (Database::isMapUpdated
    && PlayerController::timeFromLastTurn > PlayerController::waitingTime) {
        //TODO: most of gameLogic

        std::cout << "TICK=" << Database::tick << std::endl;
        for (auto post : Database::posts) {

            if (post.second -> type == PostType::MARKET) {

                auto *marketObj = static_cast<Market* >(post.second);
                marketObj -> productFict += marketObj -> replenishment;


                if (Database::tick % 30 == 2) marketObj -> productFict = marketObj -> product;
                marketObj -> productFict = std::min(marketObj -> productFict, marketObj -> product_capacity);

            }

            if (post.second -> type == PostType::STORAGE) {

                auto *storageObj = static_cast<Storage* >(post.second);
                storageObj -> armorFict += storageObj -> replenishment;

                if (Database::tick % 30 == 2) storageObj -> armor = storageObj -> armor;
                storageObj -> armorFict = std::min(storageObj -> armorFict, storageObj -> armor_capacity);

                // std::cout << "Id=" << storageObj -> point -> idx << " product=" << storageObj -> armorFict << " max=" << storageObj -> armor_capacity  << std::endl;
            }
        }
        strategyIteration();
        PlayerController::timeFromLastTurn = 0;

    }

}

void PlayerController::strategyIteration() {

    std::cout << "=====================================" << std::endl;
    std::cout << "Product=" << Database::playerTown->product <<  " Armor=" << Database::playerTown->armor  <<  " Population=" << Database::playerTown->population << std::endl;


    //TODO: iterate over Database::playerTrains
    for (auto train_now : Database::playerTrains) {

        tryTrainUpdate(train_now);
        if (train_now -> cooldown != 0) {

            std::cout << "Train idx=" << train_now -> idx << " lvl=" << train_now -> level << " IN COOL DOWN! cooldown=" << train_now -> cooldown << std::endl;
            train_now -> needWay.clear();
            continue;
        }

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

                    if (train_now->level <= 2 || Database::playerTown->level <= 2 || Database::playerTown -> armor <= 50) train_now->needWay = PlayerController::trainWayToStorage(train_now).first;
                    else train_now->needWay = PlayerController::trainWayToProducts(train_now).first;
                }
            }
        }
    }

    /// first for trains, which go to Town
    for (auto train_now : Database::playerTrains) {
        if (train_now -> needWay.size() != 0 && train_now -> needWay.back() == Database::playerTown -> point -> idx) trainIteration(train_now);
    }
    /// then for other
    for (auto train_now : Database::playerTrains) {
        if (train_now -> needWay.size() != 0 && train_now -> needWay.back() != Database::playerTown -> point -> idx) trainIteration(train_now);
    }

    Database::isMapUpdated = false;
    Network::send(Action::TURN);
    Network::send(Action::MAP, {{"layer", 1}});
}


std::pair<std::vector<int>, int> PlayerController::trainWayToProducts(Train* train) {

    std::vector<int> finalWay;
    int bestCost = -1e9;

    std::priority_queue<El> q;
    El start;

    int fixNumber = 1e9;
    int marketCount = Database::markets.size();

    int summary1 = 0, summary2 = 0;

    std::vector<int> needMarkets;
    for (int i = 0; i < Database::markets.size(); i++) {
        needMarkets.push_back(Database::markets[i] -> point -> idx);
    }
    sort(needMarkets.begin(), needMarkets.end());

    std::cout << "size:" << needMarkets.size() << std::endl;

    unsigned int start_time =  clock(); // начальное время

    int step = 0;
    do{

        random_shuffle(needMarkets.begin(), needMarkets.end());

        step += 1;
        if (step == 10) break;

        clearLinesAndVertex(marketCount);

        start.vertex = getTrainVertex(train);
        start.point = getTrainPoint(train);

        start.marketPos = 0;
        start.maxValue = train -> goods_capacity;

        q.push(start);

        int _count = 0;
        int kol2 = 0;
        int kol3 = 0;

        while(!q.empty()) {

            El to, now;

            now = q.top();
            q.pop();

            /*
            if (now.line != nullptr) {

                if (fLine[now.marketPos][now.line -> idx * 1000 + now.position] >= now.value - now.spent + fixNumber) continue;
                fLine[now.marketPos][now.line -> idx * 1000 + now.position] = now.value - now.spent + fixNumber;
                kol3 += 1;

                if (now.position == 0) {
                    now.vertex = now.line -> points[0] -> idx;
                    now.point = now.line -> points[0];

                    now.line = nullptr;
                }

                else if (now.position == now.line -> length) {
                    now.vertex = now.line -> points[1] -> idx;
                    now.point = now.line -> points[1];

                    now.line = nullptr;
                }
                else {

                    if (!isFreeTimeTable(now)) continue;

                    now.spent += 1;
                    now.position += 1;
                    q.push(now);

                    now.position -= 2;
                    q.push(now);

                    _count += 1;

                    continue;
                }
            }
            */


            if (!isFreeTimeTable(now)) continue;
            if (isStorage(now.point) && now.value == 0) continue;

            if (fVertex[now.marketPos][now.vertex] >= now.value - now.spent + fixNumber) continue;
            fVertex[now.marketPos][now.vertex] = now.value - now.spent + fixNumber;

            kol2 += 1;
            _count += 1;

            now.way.push_back(now.vertex);

            if (isTown(now.vertex)) checkOnBestWay(now, finalWay, bestCost);

            if (now.marketPos != marketCount && now.vertex == needMarkets[now.marketPos]) {
                now.marketPos += 1;
                now.value += marketProduct(now.point);
                now.value = std::min(now.value, now.maxValue);
            }

            // std::cout << "Try do something; idx=" << now.vertex << " is Town=" << isTown(now.vertex) << " marketPos=" << now.marketPos << " product=" << now.value << " spent=" << now.spent << std::endl;
            for (auto line : now.point -> adjacent) {

                if (line.second -> points[0] -> idx == now.vertex) {

                    /*
                    to = now;
                    to.line = line.second;
                    to.position = 1;
                    to.spent += 1;
                    */

                    to = now;
                    to.line = line.second;
                    for (int j = 1; j < line.second -> length; j++) {

                        to.spent += 1;
                        to.position = j;

                        if (!isFreeTimeTable(to)) goto point1;
                    }
                    to.spent += 1;
                    to.line = nullptr;

                    to.vertex = line.second -> points[1] -> idx;
                    to.point = line.second -> points[1];

                    q.push(to);
                }
                else if (line.second -> points[1] -> idx == now.vertex) {

                    /*
                    to = now;
                    to.line = line.second;
                    to.position = line.second -> length - 1;
                    to.spent += 1;
                    */

                    to = now;
                    to.line = line.second;
                    for (int j = line.second -> length - 1; j >= 1; j--) {

                        to.spent += 1;
                        to.position = j;

                        if (!isFreeTimeTable(to)) goto point1;
                    }
                    to.spent += 1;
                    to.line = nullptr;

                    to.vertex = line.second -> points[0] -> idx;
                    to.point = line.second -> points[0];

                    q.push(to);
                }

                point1:;
            }
        }

        // std::cout << "====== COUNT=" << _count << " kol3=" << kol3 << " kol2=" << kol2 << std::endl;
        // return {};

    } while (1);


    addTimeTable(finalWay);

    reverse(finalWay.begin(), finalWay.end());
    if (finalWay.size()) finalWay.pop_back();

    addFictProduct(finalWay, train -> goods_capacity);

    unsigned int end_time = clock(); // конечное время
    unsigned int search_time = end_time - start_time; // искомое время
    logTime(search_time);

    return {finalWay, summary1 + summary2};
}




std::pair<std::vector<int>, int> PlayerController::trainWayToStorage(Train* train) {

    std::vector<int> finalWay;
    int bestCost = -1e9;

    std::priority_queue<El> q;
    El start;

    int fixNumber = 1e9;
    int storagesCount = Database::storages.size();

    int summary1 = 0, summary2 = 0;

    std::vector<int> needStorages;
    for (int i = 0; i < Database::storages.size(); i++) {
        needStorages.push_back(Database::storages[i] -> point -> idx);
    }
    sort(needStorages.begin(), needStorages.end());

    int step = 0;
    do{

        step++;
        if (step == 10) break;

        random_shuffle(needStorages.begin(), needStorages.end());

        clearLinesAndVertex(storagesCount);

        start.vertex = getTrainVertex(train);
        start.point = getTrainPoint(train);

        start.marketPos = 0;
        start.maxValue = train -> goods_capacity;

        q.push(start);

        while(!q.empty()) {

            El to, now;

            now = q.top();
            q.pop();

            /*
            if (now.line != nullptr) {

                if (fLine[now.marketPos][now.line -> idx * 1000 + now.position] >= now.value - now.spent + fixNumber) continue;
                fLine[now.marketPos][now.line -> idx * 1000 + now.position] = now.value - now.spent + fixNumber;

                if (now.position == 0) {
                    now.vertex = now.line -> points[0] -> idx;
                    now.point = now.line -> points[0];

                    now.line = nullptr;
                }

                else if (now.position == now.line -> length) {
                    now.vertex = now.line -> points[1] -> idx;
                    now.point = now.line -> points[1];

                    now.line = nullptr;
                }
                else {

                    if (!isFreeTimeTable(now)) continue;

                    now.spent += 1;
                    now.position += 1;
                    q.push(now);

                    now.position -= 2;
                    q.push(now);

                    continue;
                }
            }
            */

            if (!isFreeTimeTable(now)) continue;
            if (isMarket(now.point) && now.value == 0) continue;

            if (fVertex[now.marketPos][now.vertex] >= now.value - now.spent + fixNumber) continue;
            fVertex[now.marketPos][now.vertex] = now.value - now.spent + fixNumber;


            now.way.push_back(now.vertex);
            if (isTown(now.vertex)) checkOnBestWay(now, finalWay, bestCost);

            if (now.marketPos != storagesCount && now.vertex == needStorages[now.marketPos]) {
                now.marketPos += 1;
                now.value += storageArmor(now.point);
                now.value = std::min(now.value, now.maxValue);
            }

            for (auto line : now.point -> adjacent) {

                if (line.second -> points[0] -> idx == now.vertex) {

                    /*
                    to = now;
                    to.line = line.second;
                    to.position = 1;
                    to.spent += 1;
                    */

                    to = now;
                    to.line = line.second;
                    for (int j = 1; j < line.second -> length; j++) {

                        to.spent += 1;
                        to.position = j;

                        if (!isFreeTimeTable(to)) goto point1;
                    }
                    to.spent += 1;
                    to.line = nullptr;

                    to.vertex = line.second -> points[1] -> idx;
                    to.point = line.second -> points[1];

                    q.push(to);
                }
                else if (line.second -> points[1] -> idx == now.vertex) {

                    /*
                    to = now;
                    to.line = line.second;
                    to.position = line.second -> length - 1;
                    to.spent += 1;
                    */

                    to = now;
                    to.line = line.second;
                    for (int j = line.second -> length - 1; j >= 1; j--) {

                        to.spent += 1;
                        to.position = j;

                        if (!isFreeTimeTable(to)) goto point1;
                    }
                    to.spent += 1;
                    to.line = nullptr;

                    to.vertex = line.second -> points[0] -> idx;
                    to.point = line.second -> points[0];

                    q.push(to);
                }

                point1:;
            }
        }

    } while (1);

    addTimeTable(finalWay);

    reverse(finalWay.begin(), finalWay.end());
    if (finalWay.size()) finalWay.pop_back();

    addFictArmor(finalWay, train -> goods_capacity);

    return {finalWay, summary1 + summary2};
}


void trainIteration(Train* train) {
    json message;

    message["train_idx"] = train->idx;

    if (train->position == 0 &&
        train->line->points[1]->idx == train->needWay.back()) {

        message["speed"] = 1;
        message["line_idx"] = train->line->idx;
        Network::send(Action::MOVE, message);

        train->needWay.pop_back();
        return;
    }


    if (train->position == train -> line -> length &&
        train->line->points[0]->idx == train->needWay.back()) {

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

            message["speed"] = 1;
            message["line_idx"] = line.second->idx;
            Network::send(Action::MOVE, message);

            train->needWay.pop_back();
            return;
        }

        if (line.second->points[1]->idx == getTrainVertex(train) &&
            line.second->points[0]->idx == train->needWay.back()) {

            message["speed"] = -1;
            message["line_idx"] = line.second->idx;
            Network::send(Action::MOVE, message);


            train->needWay.pop_back();
            return;
        }
    }
}


void PlayerController::tryTrainUpdate(Train* mainTrain) {

    if (mainTrain -> position != 0
    && mainTrain -> position != mainTrain -> line -> length) return;

    if (mainTrain -> position == 0
    && Database::playerTown -> point -> idx != mainTrain -> line -> points[0] -> idx) return;

    if (mainTrain -> position == mainTrain -> line -> length
    && Database::playerTown -> point -> idx != mainTrain -> line -> points[1] -> idx) return;

    if (Database::playerTown->armor >= 70 && mainTrain->level == 1) {

        json message;

        std::vector<int> a{};
        std::vector<int> b{mainTrain->idx};

        message["posts"] = a;
        message["trains"] = b;

        mainTrain->level += 1;
        Database::playerTown->armor -= 40;

        std::cout << "UPGRADE TO LEVEL 2, MESSAGE=" << message << std::endl;
        Network::send(Action::UPGRADE, message);
    }

    if (Database::playerTown->armor >= 110 && mainTrain->level == 2) {

        json message;

        std::vector<int> a{};
        std::vector<int> b{mainTrain->idx};

        message["posts"] = a;
        message["trains"] = b;

        mainTrain->level += 1;
        Database::playerTown->armor -= 80;

        std::cout << "UPGRADE TO LEVEL 3, MESSAGE=" << message << std::endl;

        Network::send(Action::UPGRADE, message);
    }

    if (mainTrain->level == 3) {
        if (Database::playerTown->armor >= 130 && Database::playerTown->level == 1) {
            json message;

            std::vector<int> a{Database::playerTown->idx};
            std::vector<int> b{};

            message["posts"] = a;
            message["trains"] = b;

            std::cout << "UPGRADE TOWN TO LEVEL 2, MESSAGE=" << message << std::endl;

            Database::playerTown->level += 1;
            Database::playerTown->armor -= 100;

            Network::send(Action::UPGRADE, message);
        }
        if (Database::playerTown->armor >= 230 && Database::playerTown->level == 2) {
            json message;

            std::vector<int> a{Database::playerTown->idx};
            std::vector<int> b{};

            message["posts"] = a;
            message["trains"] = b;

            std::cout << "UPGRADE TOWN TO LEVEL 3, MESSAGE=" << message << std::endl;

            Database::playerTown->level += 1;
            Database::playerTown->armor -= 200;
            Network::send(Action::UPGRADE, message);
        }
    }

}

void PlayerController::onMapLayer1(const nlohmann::json &json) {
    if (Game::current->trySetLayer1(json)) {
        Game::current->applyLayer1(json);
    } else {
        Network::send(Action::MAP, {{"layer", 1}});
    }
}

void PlayerController::onGames(const nlohmann::json &json) {
    if (isGameFinished(json)) {
        Game::current->setState(GameState::FINISHED);
    }
}

bool PlayerController::isGameFinished(const nlohmann::json &json) {
    if (json.contains("games")) {
        for (auto game : json["games"]) {
            if (game["name"] == PlayerConfig::hostName && game["state"] == GameState::FINISHED) {
                return true;
            }
        }
    }
    return false;
}
