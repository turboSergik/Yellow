//
// Created by Олег Бобров on 15.11.2019.
//

#ifndef WG_TRAIN_H
#define WG_TRAIN_H

#include "Behaviour.h"


class Train : public Behaviour {
public:
    explicit Train(int idx);

    int cooldown = 0;
    //int fuel = 0;
    //int fuel_capacity = 0;
    //int fuel_consumption = 1;
    int goods = 0;
    int goods_capacity = 40;
    //TODO: ask what this field store
    //"goods_type": null, WTF?
    int level = 1;
    int next_level_price = 40;
    //"player_idx": "a33dc107-04ab-4039-9578-1dccd00867d1",
    int position = 0;
    int speed = 0;
    Line *line = nullptr;

    void applyLayer1(const nlohmann::json &json) override;
    void update();
};


#endif //WG_TRAIN_H
