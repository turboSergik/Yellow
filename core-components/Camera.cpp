//
// Created by Олег Бобров on 15.11.2019.
//

#include "Camera.h"

Camera * Camera::mainCamera = nullptr;

Camera::Camera(const sf::View &view) : sf::View(view) {
    if (!Camera::mainCamera) {
        Camera::mainCamera = this; //not a singleton, just first (main) camera
    }
    setWidth(10);
}

void Camera::setSize(float width, float height) {
    sf::View::setSize(2 * this->width, 2 * this->width * height / width);
}

void Camera::setWidth(float width) {
    this->width = width;
    setSize(sf::View::getSize());
}

void Camera::setSize(const sf::Vector2f &size) {
    sf::View::setSize(2 * this->width, 2 * this->width * size.y / size.x);
}

float Camera::getWidth() {
    return width;
}
