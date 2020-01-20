//
// Created by Олег Бобров on 10.12.2019.
//

#include "PlayerController.hpp"
#include "../static/Database.h"
#include "../network/Network.hpp"
#include "../static/Time.h"
#include "../utility/random.hpp"

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
std::unordered_map<int, std::set<int> > timeTableLine[3];


std::unordered_map<int, long long> fLine[50];
std::unordered_map<int, long long> fVertex[50];
int countType1 = 0;
int bestCost2 = 0;


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

    if (vertex == PlayerController::playerTown -> point -> idx) return true;
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

    for (auto trainNow : PlayerController::playerTrains) {
        if (trainNow -> level == 1) tryTrainUpdate(trainNow);
    }

    for (auto trainNow : PlayerController::playerTrains) {
        if (trainNow -> level == 2) tryTrainUpdate(trainNow);
    }

    for (auto trainNow : PlayerController::playerTrains) {
        if (trainNow -> level == 3) tryTrainUpdate(trainNow);
    }
    return;
}


void checkOnBestWay(El &now, std::vector<int> &bestWay, int &bestCost) {

    if (now.value - now.spent > bestCost && now.way.size() > 2) {

        bestCost = now.value - now.spent;
        bestWay = now.way;
        bestCost2 = now.spent;
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

                            /*
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
                            */

                            timeTableLine[0][line.second -> idx * 1000].insert(timeNow + Database::tick - 1);
                            timeNow += len;

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

                            /*
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
                            */

                            timeTableLine[1][line.second -> idx * 1000].insert(timeNow + Database::tick - 1);
                            timeNow += len;

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

        int len = now.line -> length;

        if (now.vertex == now.line -> points[0] -> idx) {

            t = timeTableLine[0][now.line -> idx * 1000].lower_bound(now.spent + Database::tick);
            if (t != timeTableLine[0][now.line -> idx * 1000].end() && *t == now.spent + Database::tick) return false;


            t = timeTableLine[1][now.line -> idx * 1000].lower_bound(now.spent - len + Database::tick + 1);
            if (t != timeTableLine[1][now.line -> idx * 1000].end() && *t < now.spent + len + Database::tick)
            {
                std::cout << "tick=" << Database::tick << " ";
                std::cout << "line=" << now.line -> idx << " now spent=" << now.spent << " len=" << len;
                if (t != timeTableLine[1][now.line -> idx * 1000].end()) std::cout << " iterator=" << *t;
                std::cout << std::endl;

                return false;
            }

        }
        else {

            t = timeTableLine[1][now.line -> idx * 1000].lower_bound(now.spent + Database::tick);
            if (t != timeTableLine[1][now.line -> idx * 1000].end() && *t == now.spent + Database::tick) return false;


            t = timeTableLine[0][now.line -> idx * 1000].lower_bound(now.spent - len + Database::tick + 1);
            if (t != timeTableLine[0][now.line -> idx * 1000].end() && *t < now.spent + len + Database::tick)
            {

                std::cout << "tick=" << Database::tick << " ";
                std::cout << "line=" << now.line -> idx << " now spent=" << now.spent << " len=" << len;
                if (t != timeTableLine[0][now.line -> idx * 1000].end()) std::cout << " iterator=" << *t;
                std::cout << std::endl;

                return false;
            }
        }


    }

    return true;
}

void logTrain(Train* train) {


    if (train -> position == 0) std::cout << "Train idx=" << train -> idx << " lvl=" << train -> level << " point=" << train -> line -> points[0] -> idx << " move_type=" << train -> move_type;
    else if (train -> position == train -> line -> length) std::cout << "Train idx=" << train -> idx << " lvl=" << train -> level << " point=" << train -> line -> points[1] -> idx << " move_type=" << train -> move_type;
    else std::cout << "Train idx=" << train -> idx << " lvl=" << train -> level << " line=" << train -> line -> idx << " move_type=" << train -> move_type;
    std::cout << " size=" << train -> needWay.size();
    std::cout << " capacity=" << train -> goods_capacity;
    std::cout << " position=" << train -> position;
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
    for (auto train : PlayerController::playerTrains) {
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
    std::cout << "Product=" << playerTown->product <<  " Armor=" << playerTown->armor  <<  " Population=" << playerTown->population << " Level=" << playerTown -> level << std::endl;


    std::cout << "Storages: " << std::endl;
    for (auto stor : Database::storages) {
        std::cout << "Capacity=" << stor -> armor_capacity << " repl=" << stor -> replenishment << std::endl;
    }
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "Markets: " << std::endl;
    for (auto stor : Database::markets) {
        std::cout << "Capacity=" << stor -> product_capacity << " repl=" << stor -> replenishment << std::endl;
    }
    std::cout << std::endl;


    //TODO: iterate over PlayerController::playerTrains
    for (auto train_now : PlayerController::playerTrains) {

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

            if (train_now->needWay.size() == 0 && (train_now -> position == 0 || train_now -> position == train_now -> line -> length)) {

                if (train_now -> move_type == 1) train_now -> needWay = PlayerController::trainWayToProducts(train_now).first;
                else if (train_now -> move_type == 2) {

                    if (train_now->level <= 2 || playerTown->level <= 2 || playerTown -> armor <= 50) train_now->needWay = PlayerController::trainWayToStorage(train_now).first;
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

            kol2++;

            El to, now;

            now = q.top();
            q.pop();

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

    unsigned int start_time =  clock(); // начальное время


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
        if (step == 100) break;

        random_shuffle(needStorages.begin(), needStorages.end());
        /// while(needStorages.size() > 3) needStorages.pop_back();

        clearLinesAndVertex(storagesCount);

        start.vertex = getTrainVertex(train);
        start.point = getTrainPoint(train);

        start.marketPos = 0;
        start.maxValue = train -> goods_capacity;

        int kol2 = 0;
        int kol3 = 0;
        int kol4 = 0;

        q.push(start);

        while(!q.empty()) {


            El to, now;

            now = q.top();
            q.pop();

            if (!isFreeTimeTable(now)) continue;
            if (isMarket(now.point) && now.value == 0) continue;

            kol2++;

            if (fVertex[now.marketPos][now.vertex] >= now.value - now.spent + fixNumber) continue;
            fVertex[now.marketPos][now.vertex] = now.value - now.spent + fixNumber;

            kol3++;

            now.way.push_back(now.vertex);
            if (isTown(now.vertex)) checkOnBestWay(now, finalWay, bestCost);

            if (now.marketPos != storagesCount && now.vertex == needStorages[now.marketPos]) {
                now.marketPos += 1;
                now.value += storageArmor(now.point);
                now.value = std::min(now.value, now.maxValue);
            }

            for (auto line : now.point -> adjacent) {

                if (line.second -> points[0] -> idx == now.vertex) {

                    to = now;
                    to.line = line.second;

                    if (!isFreeTimeTable(to)) goto point1;

                    to.spent += to.line -> length;
                    to.line = nullptr;

                    kol4++;

                    to.vertex = line.second -> points[1] -> idx;
                    to.point = line.second -> points[1];

                    //if (fVertex[to.marketPos][to.vertex] >= to.value - to.spent + fixNumber) continue;

                    q.push(to);
                }
                else if (line.second -> points[1] -> idx == now.vertex) {

                    to = now;
                    to.line = line.second;

                    if (!isFreeTimeTable(to)) goto point1;

                    to.spent += to.line -> length;
                    to.line = nullptr;

                    kol4++;

                    to.vertex = line.second -> points[0] -> idx;
                    to.point = line.second -> points[0];

                    //if (fVertex[to.marketPos][to.vertex] >= to.value - to.spent + fixNumber) continue;

                    q.push(to);
                }

                point1:;
            }
        }

        // sstd::cout << "kol2=" << kol2 << " kol3=" << kol3 << " kol4=" << kol4 << std::endl;

    } while (1);

    addTimeTable(finalWay);

    reverse(finalWay.begin(), finalWay.end());
    if (finalWay.size()) finalWay.pop_back();

    addFictArmor(finalWay, train -> goods_capacity);

    unsigned int end_time = clock(); // конечное время
    unsigned int search_time = end_time - start_time; // искомое время
    logTime(search_time);

    std::cout << "BSET WAY COST=" << bestCost2 << std::endl;

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
    && PlayerController::playerTown -> point -> idx != mainTrain -> line -> points[0] -> idx) return;

    if (mainTrain -> position == mainTrain -> line -> length
    && PlayerController::playerTown -> point -> idx != mainTrain -> line -> points[1] -> idx) return;

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


static const char * s_list[] = {
    "1. Код говно.",
    "2. Это надо переписать.",
    "3. Ты чем вообще думаешь?",
    "4. Так никто не пишет.",
    "5. Ебанёт.",
    "6. И как это поддерживать?",
    "7. Пиздец!",
    "8. Пиздец[2]!",
    "9. Мы же полгода назад устно договаривались не делать так.",
    "10. Я это ревьюить больше не буду, ищи кого-нибудь ещё.",
    "11. Ты этот говнокод из своего днк взял?",
    "12. Обезьяну посади - сделает лучше",
    "13. Ты вообще чтоли животное?",
    "14. Господь дал нам интеллект, но ты, я смотрю, от подарков отказываешься",
    "15. Ещё раз так сделаешь - я тебя найду",
    "16. Маме твоей вечером покажу это, если не исправишь",
    "17. Неудивительно, что от тебя отец ушёл",
    "18. Если бы я просто на клавиатуру сел- было бы также, как у тебя",
    "19. Если ты считаешь, что это ок, то как ты вообще дверь открываешь?",
    "20. Буду просить, чтобы тебя повысили, так как тебе нужны деньги на лечение",
    "21. Да я о твоей ориентации давно догадался, можешь расслабиться",
    "22. Из KFC сбежал какой-то недожаренный петух и пытается это вкомитить",
    "23. Из-за тебя я всегда работу найду, спасибо",
    "24. А сначала казался нормальным человеком",
    "25. Твои пр- это первые автобаны в России",
    "26. В следующий раз буду смотреть это до обеда",
    "27. У меня от этого говна зрение на единицу село",
    "28. Твои пр- это артхаус",
    "29. Давай ты больше не будешь шутить в пулл реквестах?",
    "30. У твоих родителей есть живые дети?",
    "31. Если бы мы сидели в комнате с в комнате с Гитлером и Сталиным,у меня было бы два патрона,то я бы за такое выстрелил в тебя дважды",
    "32. Что общего между художественным фильмом -two girls one cup- и этим ПР? И там,и тут жрут говно",
    "33. Жаль отменили смертную казнь",
    "34. Тебе вообще людей не жалко?",
    "35. Когда читаю твой код, то постоянно чихаю, потому что у меня аллергия на чушб",
    "36. Я, конечно, не гинеколог, но это пизда",
    "37. Ты вообще в курсе, что это все видят?",
    "38. Ты не говорил, что с цирковым прошлым",
    "39. После этого пр не забудь руки помыть",
    "40. И давно это с тобой?",
    "41. Уважаемый, еб вашу мать, вам там нормально?",
    "42. Извините, а адекватность выйдет?",
    "43. Ну, ты хотя бы пытался!",
    "44. Лучше бы собаку завели",
    "45. Думать - не твоя сильная сторона",
    "46. Ты молодец(нет)",
    "47. Когда бог раздавал мозги, то ты, похоже, стоял между утконосом и голубем",
    "48. Когда я говорю, что это говнокод даже голуби головой кивают",
    "49. Твои пулл реквесты - это комедия, а последний и вовсе триллер",
    "50. Кошечка говорит мяу, коровка говорит му, я говорю саша ты ебанулся такое писать?",
    "51. H-водород, С-углерод, O- иди переписывай, урод",
    "52. Ты не узнавал, этот стиль и подход - заразный?",
    "53. Если бы у меня не было высшего образования, я бы не понял ни строчки, молодец!",
    "54. Ты на 1xbet за машин против человечества поставил?",
    "55. Ты доказательство того, что человечество обречено",
    "56. Открыл сейчас этот твой код и в городе ввели чрезвычайные ситуации техногенного характера",
    "57. Жаль, что у тебя кончился испытательный срок!",
    "58. Я бы мог сейчас с тобой согласиться, но тогда мы оба будем не правы",
    "59. Ты, я смотрю, и с дипломом, программер хуевый, иди на завод делать рубанок новы",
    "60. У нас в проекте была проблема с производительностью, но я смотрю у нас есть кое-что хуже",

};
