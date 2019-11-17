//
// Created by Олег Бобров on 17.11.2019.
//

#include "Renderer.h"

//std::list<Renderer*> Renderer::renderers;

Renderer::Renderer() {
    Renderer::renderers.push_back(this);
    iterator = std::prev(renderers.end());
}
