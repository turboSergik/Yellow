//
// Created by Олег Бобров on 15.11.2019.
//

#include "Post.h"
#include "../static/Database.h"

void Post::applyLayer1(const nlohmann::json &json) {
    Post::name = json.value("name", "");
    if (json.contains("point_idx")) {
        const auto & point_idx = json["point_idx"];
        if (point_idx != nullptr) {
            Post::point = Database::points[point_idx];
            if (Post::point) {
                Post::transform->setParent(Post::point->transform);
                Post::point->post = this;
            }
        } else {
            //TODO: alert that point_idx = null (exception or smth else)
        }
    }
}

Post::Post(int idx) : Behaviour(idx) {}


Town::Town(int idx) : Post(idx) {
    Post::type = PostType::TOWN;
}

void Town::applyLayer1(const nlohmann::json &json) {
    Post::applyLayer1(json);
    Town::armor = json.value("armor", Town::armor);
    Town::armor_capacity = json.value("armor_capacity", Town::armor_capacity);
    Town::level = json.value("level", Town::level);
    /// Town::next_level_price = json.value("next_level_price", Town::next_level_price);
    Town::next_level_price = 1488;

    Town::population = json.value("population", Town::population);
    Town::population_capacity = json.value("population_capacity", Town::population_capacity);
    Town::product = json.value("product", Town::product);
    Town::product_capacity = json.value("product_capacity", Town::product_capacity);
    Town::train_cooldown = json.value("train_cooldown", Town::train_cooldown);
    Town::player_idx = json.value("player_idx", Town::player_idx);
}

Market::Market(int idx): Post(idx) {
    Post::type = PostType::MARKET;
}

void Market::applyLayer1(const nlohmann::json &json) {
    Post::applyLayer1(json);
    Market::product = json.value("product", Market::product);
    Market::product_capacity = json.value("product_capacity", Market::product_capacity);
    Market::replenishment = json.value("replenishment", Market::replenishment);
}

Storage::Storage(int idx) : Post(idx) {
    Post::type = PostType::STORAGE;
}

void Storage::applyLayer1(const nlohmann::json &json) {
    Post::applyLayer1(json);
    Storage::armor = json.value("armor", Storage::armor);
    Storage::armor_capacity = json.value("armor_capacity", Storage::armor_capacity);
    Storage::replenishment = json.value("replenishment", Storage::replenishment);
}

