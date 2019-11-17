//
// Created by Олег Бобров on 17.11.2019.
//

#ifndef WG_SPRITERENDERER_H
#define WG_SPRITERENDERER_H


#include "Renderer.h"

class SpriteRenderer : public Renderer {
public:
    sf::Sprite sprite; //TODO: hide this shit and adapt methods (like line renderer)
    void onDraw(sf::RenderTarget &target, sf::RenderStates states) const override;
};


#endif //WG_SPRITERENDERER_H
