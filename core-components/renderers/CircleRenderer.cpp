//
// Created by Олег Бобров on 17.11.2019.
//

#include "CircleRenderer.h"

void CircleRenderer::onDraw(sf::RenderTarget &target, sf::RenderStates states) const {
    states.transform *= transform->getLocalToWorldTransform();
    target.draw(circle, states);
}
