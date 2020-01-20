//
// Created by Олег Бобров on 20.01.2020.
//

#ifndef WG_MENUCONTROLLER_HPP
#define WG_MENUCONTROLLER_HPP


#include "../../core/GameObject.h"
#include "../Game.hpp"
#include "../../utility/json.hpp"
#include <iostream>

class MenuController : public Component {
public:
    void start();
    void onDestroy();

    void onGames(const nlohmann::json & json);
};


#endif //WG_MENUCONTROLLER_HPP
