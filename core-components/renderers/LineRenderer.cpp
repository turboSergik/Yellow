//
// Created by Олег Бобров on 17.11.2019.
//

#include "LineRenderer.h"

const sf::Vertex *LineRenderer::getVertices() const {
    return vertices;
}

void LineRenderer::onDraw(sf::RenderTarget &target, sf::RenderStates states) const {
    states.transform *= transform->getLocalToWorldTransform();
    target.draw(vertices, 2, sf::Lines, states);
}

void LineRenderer::setVertices(const lng::Vector2 &v1, const lng::Vector2 &v2) {
    vertices[0].position = v1;
    vertices[1].position = v2;
}
