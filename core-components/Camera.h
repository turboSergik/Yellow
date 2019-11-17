//
// Created by Олег Бобров on 15.11.2019.
//

#ifndef WG_CAMERA_H
#define WG_CAMERA_H


#include <SFML/Graphics/View.hpp>
#include <SFML/Window.hpp>

class Camera : public sf::View, public Component {
private:
    static Camera* mainCamera;
    float width; //horizontal half-size
public:
    sf::Keyboard::Key up = sf::Keyboard::Up;
    sf::Keyboard::Key left = sf::Keyboard::Left;
    sf::Keyboard::Key down = sf::Keyboard::Down;
    sf::Keyboard::Key right = sf::Keyboard::Right;
    float maxSize = 10000;
    float minSize = 1000;
    float scrollFactor = 0.1f;
    float moveFactor = 0.1f;

    using sf::View::View;

    Camera(const sf::View &view);
    void update(float deltaTime);
    float getWidth();
    void setWidth(float width);
    void setSize(float width, float height);
    void setSize(const sf::Vector2f &size);
};


#endif //WG_CAMERA_H
