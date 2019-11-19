//
// Created by Олег Бобров on 15.11.2019.
//

#ifndef WG_CAMERA_H
#define WG_CAMERA_H


#include <SFML/Graphics/View.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include "../core/Component.h"

class Camera : public Component {
private:
    static Camera* mainCamera;
    float width; //horizontal half-size
    sf::RenderTarget *renderTarget;
    sf::View view;
public:
    Camera(sf::RenderTarget *renderTarget1);
    float getWidth();
    void setWidth(float width);
    void setWindowSize(float width, float height);
    void setWindowSize(const sf::Vector2f &size);
    void onWindowResized();
    sf::RenderStates getRenderState();
};


#endif //WG_CAMERA_H
