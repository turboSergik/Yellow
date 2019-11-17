//
// Created by Олег Бобров on 15.11.2019.
//

#ifndef WG_TRAIN_H
#define WG_TRAIN_H


#include "Line.h"

class Train : public sf::Sprite, public Behaviour {
public:
    explicit Train(int idx);
};


#endif //WG_TRAIN_H
