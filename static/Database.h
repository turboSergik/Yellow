//
// Created by Олег Бобров on 15.11.2019.
//

#ifndef WG_DATABASE_H
#define WG_DATABASE_H

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <list>
#include "../custom-components/Point.h"
#include "../custom-components/Line.h"
#include "../custom-components/Post.h"
#include "../custom-components/Train.h"

class Database {

public:
    //if someone need singleton, he can do it himself
    static std::unordered_map<int, Point*> points;
    static std::unordered_map<int, Line*> lines;
    static std::unordered_map<int, Post*> posts;
    static std::unordered_map<int, Train*> trains;
};


#endif //WG_DATABASE_H
