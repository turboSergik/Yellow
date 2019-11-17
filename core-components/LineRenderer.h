//
// Created by Олег Бобров on 17.11.2019.
//

#ifndef WG_LINERENDERER_H
#define WG_LINERENDERER_H


#include "Renderer.h"

class LineRenderer : public Renderer {
private:
    sf::Vertex vertices[2];
public:
    const sf::Vertex *getVertices() const;
    void setVertices(const sf::Vector2f &v1, const sf::Vector2f &v2);
protected:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

protected:

};


#endif //WG_LINERENDERER_H
