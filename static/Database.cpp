//
// Created by Олег Бобров on 15.11.2019.
//

#include "Database.h"

bool Database::isMapUpdated = false;
int Database::tick = 0;
std::unordered_map<int, Point*> Database::points;
std::unordered_map<int, Line*> Database::lines;
std::unordered_map<int, Post*> Database::posts;
std::unordered_map<int, Train*> Database::trains;

std::vector<Market*> Database::markets;
std::vector<Storage*> Database::storages;

