//
// Created by Олег Бобров on 17.11.2019.
//

#include "LineRenderer.h"

const sf::Vertex *LineRenderer::getVertices() const {
    return vertices;
}

void LineRenderer::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    states.transform *= transform->getLocalToWorldTransform();
    target.draw(vertices, 2, sf::Lines, states);
}

void LineRenderer::setVertices(const sf::Vector2f &v1, const sf::Vector2f &v2) {
    vertices[0].position = v1;
    vertices[1].position = v2;
}
