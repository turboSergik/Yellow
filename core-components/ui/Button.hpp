
#ifndef BUTTON_HPP
#define BUTTON_HPP
#include "../../utility/Event.hpp"
#include "../../core/Component.h"
#include "../../core-components/colliders/BoxCollider.hpp"
#include "../../core-components/renderers/RectangleRenderer.hpp"

class Button : public Component {
public:
    int buttonIdx;
    BoxCollider * buttonCollider;
    RectangleRenderer * renderer;
    Event<int> onClick;
    void update();
};

#endif
