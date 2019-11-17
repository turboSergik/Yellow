//
// Created by Олег Бобров on 15.11.2019.
//

#include "Post.h"

void Post::applyLayer1(const nlohmann::json &json) {
    this->name = json["name"];
}

Post::Post(int idx) {
    this->idx = idx;
}

Town::Town(int idx) : Post(idx) {
    this->type = PostType::TOWN;
}

void Town::applyLayer1(const nlohmann::json &json) {
    Post::applyLayer1(json);
    this->armor = json["armor"];
    this->armor_capacity = json["armor_capacity"];
    this->level = json["level"];
    this->next_level_price = json["next_level_price"];
    //this->player_idx = item["player_idx"];
    this->population = json["population"];
    this->population_capacity = json["population_capacity"];
    this->product = json["product"];
    this->product_capacity = json["product_capacity"];
    this->train_cooldown = json["train_cooldown"];
}

Market::Market(int idx): Post(idx) {
    this->type = PostType::MARKET;
}

void Market::applyLayer1(const nlohmann::json &json) {
    Post::applyLayer1(json);
    this->product = json["product"];
    this->product_capacity = json["product_capacity"];
    this->replenishment = json["replenishment"];
}

Storage::Storage(int idx) : Post(idx) {
    this->type = PostType::STORAGE;
}

void Storage::applyLayer1(const nlohmann::json &json) {
    Post::applyLayer1(json);
    this->armor = json["armor"];
    this->armor_capacity = json["armor_capacity"];
    this->replenishment = json["replenishment"];
}