
#ifndef BUTTON_HPP
#define BUTTON_HPP
#include "../../utility/Event.hpp"
#include "../../core/Component.h"
#include "../../core-components/colliders/BoxCollider.hpp"
#include "../../core-components/renderers/RectangleRenderer.hpp"
// #include "../../core-components/renderers/TextRenderer.hpp"

class Button : public Component {
    static size_t ButtonCounter;
    size_t buttonIdx;

public:
    BoxCollider * buttonCollider;
    RectangleRenderer * renderer;
    Button();
    Event<size_t> onPress;
    void update();
};

#endif
