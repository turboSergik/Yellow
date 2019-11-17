//
// Created by Олег Бобров on 17.11.2019.
//

#ifndef WG_CIRCLERENDERER_H
#define WG_CIRCLERENDERER_H


#include "Renderer.h"

class CircleRenderer : public Renderer {
public:
    sf::CircleShape circle; //TODO: hide this shit and adapt methods (like line renderer)

    void onDraw(sf::RenderTarget &target, sf::RenderStates states) const override;
};


#endif //WG_CIRCLERENDERER_H
