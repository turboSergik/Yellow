//
// Created by Олег Бобров on 15.11.2019.
//

#include "Line.h"

void Line::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    states.transform *= getTransform();
    sf::Vertex vertices[] = {
            sf::Vertex(points[0]->getPosition()),
            sf::Vertex(points[1]->getPosition())
    };
    target.draw(vertices, 2, sf::Lines, states);
}

Line::Line(int idx) {
    this->idx = idx;
}

void Line::applyLayer0(const nlohmann::json &json) {
    this->length = json["length"];
}
