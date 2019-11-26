//
// Created by Олег Бобров on 17.11.2019.
//

#include "Renderer.h"

std::list<Renderer*> Renderer::renderers;

Renderer::Renderer() {
    Renderer::renderers.push_back(this);
    iterator = std::prev(renderers.end());
}

Renderer::~Renderer() {
    Renderer::renderers.erase(iterator);
}

void Renderer::draw(sf::RenderTarget &target, sf::RenderStates states) {
    for (auto renderer : renderers) {
        renderer->onDraw(target, states);
    }
}
