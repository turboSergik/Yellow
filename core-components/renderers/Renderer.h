//
// Created by Олег Бобров on 17.11.2019.
//

#ifndef WG_RENDERER_H
#define WG_RENDERER_H


#include <SFML/Graphics.hpp>
#include <list>
#include "../../core/Component.h"

class Renderer : public Component {
private:
    static std::list<Renderer*> renderers;
    std::list<Renderer*>::iterator iterator;
public:
    static void draw(sf::RenderTarget &target, sf::RenderStates states);
    Renderer();

    virtual void onDraw(sf::RenderTarget &target, sf::RenderStates states) const = 0;
};


#endif //WG_RENDERER_H
