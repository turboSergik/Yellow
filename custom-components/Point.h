//
// Created by Олег Бобров on 15.11.2019.
//

#ifndef WG_POINT_H
#define WG_POINT_H

#include "Behaviour.h"
#include "../core-components/RigidBody.hpp"

class Post;

class Point : public Behaviour {
private:
    float frictionK = 0.05f;
    float dampK = 0.15f;
public:
    RigidBody * rigidBody = nullptr;
    Post * post = nullptr;
    std::list<std::pair<Point*, Line*>> adjacent;

    explicit Point(int idx);
    void applyLayer10(const nlohmann::json &json) override;
    void applyLayer0(const nlohmann::json &json) override;

    void start();
    void update();
};


#endif //WG_POINT_H
