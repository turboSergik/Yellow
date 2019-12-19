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
    Point* point = nullptr;
    PostType type = PostType::DEFAULT;
    std::string name;

    explicit Post(int idx);
    virtual void applyLayer1(const nlohmann::json &json);
};

class Town : public Post {
public:
    int armor = 0;
    int armor_capacity = 0;
    int level = 0;
    int next_level_price = 0;
    int population = 0;
    int population_capacity = 0;
    int product = 0;
    int product_capacity = 0;
    int train_cooldown = 0;
    std::string player_idx;

    explicit Town(int idx);
    void applyLayer1(const nlohmann::json &json) override;
};

class Market : public Post {
public:
    int product = 0;
    int product_capacity = 0;
    int replenishment = 0;

    explicit Market(int idx);
    void applyLayer1(const nlohmann::json &json) override;
};

class Storage : public Post {
public:
    int armor = 0;
    int armor_capacity = 0;
    int replenishment = 0;

    explicit Storage(int idx);
    void applyLayer1(const nlohmann::json &json) override;
};


#endif //WG_POST_H
