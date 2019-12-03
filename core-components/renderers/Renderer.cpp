//
// Created by Олег Бобров on 17.11.2019.
//

#include "Renderer.h"

std::list<Renderer*> Renderer::renderers;

void Renderer::draw(sf::RenderTarget &target, sf::RenderStates states) {
    for (auto renderer : renderers) {
        renderer->onDraw(target, states);
    }
}

void Renderer::start() {
    Renderer::renderers.push_back(this);
    iterator = std::prev(renderers.end());
}

void Renderer::onDestroy() {
    Renderer::renderers.erase(iterator);
}
