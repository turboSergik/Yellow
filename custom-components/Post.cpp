//
// Created by Олег Бобров on 15.11.2019.
//

#include "Post.h"
#include "../static/Database.h"

void Post::applyLayer1(const nlohmann::json &json) {
    Post::name = json["name"];
    Post::point = Database::points[json["point_idx"]];
    Post::transform->setParent(point->transform);//TODO: change to Instantiate(original, parent)
}

Post::Post(int idx) : Behaviour(idx) {}


Town::Town(int idx) : Post(idx) {
    Post::type = PostType::TOWN;
}

void Town::applyLayer1(const nlohmann::json &json) {
    Post::applyLayer1(json);
    Town::armor = json["armor"];
    Town::armor_capacity = json["armor_capacity"];
    Town::level = json["level"];
    Town::next_level_price = json["next_level_price"];
    //this->player_idx = item["player_idx"];
    Town::population = json["population"];
    Town::population_capacity = json["population_capacity"];
    Town::product = json["product"];
    Town::product_capacity = json["product_capacity"];
    Town::train_cooldown = json["train_cooldown"];
}

Market::Market(int idx): Post(idx) {
    Post::type = PostType::MARKET;
}

void Market::applyLayer1(const nlohmann::json &json) {
    Post::applyLayer1(json);
    Market::product = json["product"];
    Market::product_capacity = json["product_capacity"];
    Market::replenishment = json["replenishment"];
}

Storage::Storage(int idx) : Post(idx) {
    Post::type = PostType::STORAGE;
}

void Storage::applyLayer1(const nlohmann::json &json) {
    Post::applyLayer1(json);
    Storage::armor = json["armor"];
    Storage::armor_capacity = json["armor_capacity"];
    Storage::replenishment = json["replenishment"];
}