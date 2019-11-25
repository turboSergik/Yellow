//
// Created by Олег Бобров on 15.11.2019.
//

#ifndef WG_POINT_H
#define WG_POINT_H

#include "Behaviour.h"

class Post;

class Point : public Behaviour {
public:
    Post* post = nullptr;

    explicit Point(int idx);
    void applyLayer10(const nlohmann::json &json) override;
    void applyLayer0(const nlohmann::json &json) override;
};


#endif //WG_POINT_H
