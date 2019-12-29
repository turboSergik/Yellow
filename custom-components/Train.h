//
// Created by Олег Бобров on 15.11.2019.
//

#ifndef WG_TRAIN_H
#define WG_TRAIN_H

#include "Behaviour.h"
#include "../linalg/Vector2.hpp"

enum GoodsType {
    Products = 1,
    Armor = 2
};

class Train : public Behaviour {
private:
    Vector2 lerp(const Vector2 & a, const Vector2 & b, float t);
public:
    explicit Train(int idx);

    int cooldown = 0;
    int fuel = 0;
    int fuel_capacity = 0;
    int fuel_consumption = 0;
    int goods = 0;
    int goods_capacity = 0;
    int level = 0;
    int next_level_price = 0;
    int position = 0;
    int speed = 0;
    std::string player_idx;
    Line *line = nullptr;
    //TODO: ask what this field store
    //"goods_type": null, WTF?

    int move_type = 0;
    int destiny = 0;
    std::vector<int> needWay;

    void applyLayer1(const nlohmann::json &json) override;
    void update();
};


#endif //WG_TRAIN_H
