//
// Created by Олег Бобров on 15.11.2019.
//

#ifndef WG_POST_H
#define WG_POST_H


#include <string>
#include "Point.h"

enum PostType
{
    TOWN = 1,
    MARKET = 2,
    STORAGE = 3
};

class Post : public sf::Sprite, public Behaviour {
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
    //player_idx "a33dc107-04ab-4039-9578-1dccd00867d1" //TODO: what type use?
    int population;
    int population_capacity;
    int product;
    int product_capacity;
    int train_cooldown;

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
