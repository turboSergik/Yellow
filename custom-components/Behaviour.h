//
// Created by Олег Бобров on 16.11.2019.
//

#ifndef WG_BEHAVIOUR_H
#define WG_BEHAVIOUR_H


#include <SFML/System.hpp>
#include <SFML/Graphics/Transform.hpp>
#include "../json.hpp"
#include "../core/Transform.h"
#include "../core/Component.h"

class Behaviour : public Component {
private:
public:
    int idx;

    virtual void update(float deltaTime);
    virtual void applyLayer0(const nlohmann::json &json);
    virtual void applyLayer1(const nlohmann::json &json);
    virtual void applyLayer10(const nlohmann::json &json);
};


#endif //WG_BEHAVIOUR_H
