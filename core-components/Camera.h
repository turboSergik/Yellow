//
// Created by Олег Бобров on 15.11.2019.
//

#ifndef WG_CAMERA_H
#define WG_CAMERA_H


#include <SFML/Graphics/View.hpp>
#include <SFML/Window.hpp>
#include "../core/Component.h"

class Camera : public sf::View, public Component {
private:
    static Camera* mainCamera;
    float width; //horizontal half-size
public:
    //TODO: make camera fully control view
    //TODO: split camera control and camera

    using sf::View::View;

    Camera(const sf::View &view);
    float getWidth();
    void setWidth(float width);
    void setSize(float width, float height);
    void setSize(const sf::Vector2f &size);
};


#endif //WG_CAMERA_H
