//
// Created by Олег Бобров on 15.11.2019.
//

#ifndef WG_LINE_H
#define WG_LINE_H


#include "Behaviour.h"

class Line : public Behaviour {
public:
    int length;
    Point* points[2];

    explicit Line(int idx);
    void applyLayer0(const nlohmann::json &json) override;
};

#endif //WG_LINE_H
