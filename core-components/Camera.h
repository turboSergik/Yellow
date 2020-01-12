//
// Created by Олег Бобров on 15.11.2019.
//

#ifndef WG_CAMERA_H
#define WG_CAMERA_H


#include <SFML/Graphics/View.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include "../core/Component.h"
#include "../linalg/Vector2.hpp"

class Camera : public Component {
private:
    float width; //horizontal half-size
    sf::RenderTarget *renderTarget;
    sf::View view;
public:
    static Camera* mainCamera;
    Camera(sf::RenderTarget *renderTarget);
    float getWidth();
    void setWidth(float width);
    void setWindowSize(float width, float height);
    void setWindowSize(const Vector2 &size);
    void onWindowResized();
    Vector2 screenToWorldPoint(const Vector2 & point);
    Vector2 worldToScreenPoint(const Vector2 & point);
    sf::RenderStates getRenderState();
};


#endif //WG_CAMERA_H
