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
public:
    const sf::Vertex *getVertices() const;
    void setVertices(const Vector2 &v1, const Vector2 &v2);
    //TODO: add color change ability
    void onDraw(sf::RenderTarget &target, sf::RenderStates states) const override;

protected:

};


#endif //WG_LINERENDERER_H
