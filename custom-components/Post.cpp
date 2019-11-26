//
// Created by Олег Бобров on 15.11.2019.
//

#include "Post.h"
#include "../static/Database.h"

void Post::applyLayer1(const nlohmann::json &json) {
    json.tryGetValue("name", Post::name);
    nlohmann::json point_idx;
    json.tryGetValue("point_idx", point_idx);
    if (point_idx != nullptr) {
        Post::point = Database::points[point_idx];
        Post::transform->setParent(Post::point->transform);//TODO: change to Instantiate(original, parent)
    } else {
        //TODO: alert that point_idx = null (exception or smth else)
    }
}

Post::Post(int idx) : Behaviour(idx) {}


Town::Town(int idx) : Post(idx) {
    Post::type = PostType::TOWN;
}

void Town::applyLayer1(const nlohmann::json &json) {
    Post::applyLayer1(json);
    json.tryGetValue("armor", Town::armor);
    json.tryGetValue("armor_capacity", Town::armor_capacity);
    json.tryGetValue("level", Town::level);
    json.tryGetValue("next_level_price", Town::next_level_price);
    json.tryGetValue("population", Town::population);
    json.tryGetValue("population_capacity", Town::population_capacity);
    json.tryGetValue("product", Town::product);
    json.tryGetValue("product_capacity", Town::product_capacity);
    json.tryGetValue("train_cooldown", Town::train_cooldown);
    json.tryGetValue("player_idx", Town::player_idx);
}

Market::Market(int idx): Post(idx) {
    Post::type = PostType::MARKET;
}

void Market::applyLayer1(const nlohmann::json &json) {
    Post::applyLayer1(json);
    json.tryGetValue("product", Market::product);
    json.tryGetValue("product_capacity", Market::product_capacity);
    json.tryGetValue("replenishment", Market::replenishment);
}

Storage::Storage(int idx) : Post(idx) {
    Post::type = PostType::STORAGE;
}

void Storage::applyLayer1(const nlohmann::json &json) {
    Post::applyLayer1(json);
    json.tryGetValue("armor", Storage::armor);
    json.tryGetValue("armor_capacity", Storage::armor_capacity);
    json.tryGetValue("replenishment", Storage::replenishment);
}