//
// Created by Олег Бобров on 17.11.2019.
//

#ifndef WG_LINERENDERER_H
#define WG_LINERENDERER_H


#include "Renderer.h"
#include "../../linalg/Vector2.hpp"

class LineRenderer : public Renderer {
private:
    sf::Vertex vertices[2];
    sf::Color color;
public:
    const sf::Color & getColor() const;
    void setColor(const sf::Color &color);
    const sf::Vertex *getVertices() const;
    void setVertices(const Vector2 &v1, const Vector2 &v2);
    void onDraw(sf::RenderTarget &target, sf::RenderStates states) const override;
};


#endif //WG_LINERENDERER_H
