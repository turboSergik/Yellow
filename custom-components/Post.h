//
// Created by Олег Бобров on 15.11.2019.
//

#ifndef WG_POST_H
#define WG_POST_H


#include <string>
#include "Behaviour.h"

enum PostType
{
    DEFAULT = 0,
    TOWN = 1,
    MARKET = 2,
    STORAGE = 3,
};

class Post : public Behaviour {
public:
    Point* point;
    PostType type;
    std::string name;

    explicit Post(int idx);
    virtual void applyLayer1(const nlohmann::json &json);
};

class Town : public Post {
public:
    int armor;
    int armor_capacity;
    int level;
    int next_level_price;
    int population;
    int population_capacity;
    int product;
    int product_capacity;
    int train_cooldown;
    std::string player_idx;

    explicit Town(int idx);
    void applyLayer1(const nlohmann::json &json) override;
};

class Market : public Post {
public:
    int product;
    int product_capacity;
    int replenishment;

    explicit Market(int idx);
    void applyLayer1(const nlohmann::json &json) override;
};

class Storage : public Post {
public:
    int armor;
    int armor_capacity;
    int replenishment;

    explicit Storage(int idx);
    void applyLayer1(const nlohmann::json &json) override;
};


#endif //WG_POST_H
