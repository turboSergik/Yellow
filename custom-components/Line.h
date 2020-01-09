//
// Created by Олег Бобров on 15.11.2019.
//

#ifndef WG_LINE_H
#define WG_LINE_H


#include "Behaviour.h"
#include "../core-components/renderers/LineRenderer.h"
#include "../utility/Config.hpp"

class Line : public Behaviour {
private:
    float worldLength = 0.f;
    LineRenderer *lineRenderer = nullptr;
public:
    int length = -1;
    Point* points[2] = {nullptr, nullptr};

    explicit Line(int idx);

    Vector2 toWorldGlobalPosition(int position);
    Vector2 toWorldLocalPosition(int position);
    void start();
    void fixedUpdate();
    void applyLayer0(const nlohmann::json &json) override;
};

#endif //WG_LINE_H
