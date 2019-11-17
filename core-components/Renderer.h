//
// Created by Олег Бобров on 17.11.2019.
//

#ifndef WG_RENDERER_H
#define WG_RENDERER_H


#include <SFML/Graphics.hpp>
#include <list>
#include "../core/Component.h"

class Renderer : public Component, public sf::Drawable {
private:
    static std::list<Renderer*> renderers;
    std::list<Renderer*>::iterator iterator;
public:
    Renderer();
};


#endif //WG_RENDERER_H
