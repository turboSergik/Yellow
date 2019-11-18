//
// Created by Олег Бобров on 17.11.2019.
//

#include "SpriteRenderer.h"

void SpriteRenderer::onDraw(sf::RenderTarget &target, sf::RenderStates states) const {
    states.transform *= transform->getLocalToWorldTransform();
    target.draw(sprite, states);
}
