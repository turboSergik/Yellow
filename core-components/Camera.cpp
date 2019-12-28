//
// Created by Олег Бобров on 15.11.2019.
//

#include "Camera.h"

Camera * Camera::mainCamera = nullptr;

Camera::Camera(sf::RenderTarget *renderTarget) {
    if (!Camera::mainCamera) {
        Camera::mainCamera = this; //not a singleton, just first (main) camera
    }
    Camera::renderTarget = renderTarget;
    Camera::view = renderTarget->getView();
    Camera::width = 1000;
    sf::Vector2u size = Camera::renderTarget->getSize();
    Camera::view.setSize(2 * Camera::width, 2 * Camera::width * size.y / size.x);
    Camera::view.setCenter(0, 0);
    Camera::renderTarget->setView(view);
}

void Camera::setWindowSize(float width, float height) {
    Camera::view.setSize(2 * Camera::width, 2 * Camera::width * height / width);
    Camera::renderTarget->setView(view);
}

void Camera::setWindowSize(const lng::Vector2 &size) {
    Camera::view.setSize(2 * Camera::width, 2 * Camera::width * size.y / size.x);
    Camera::renderTarget->setView(view);
}

void Camera::setWidth(float width) {
    Camera::width = width;
    sf::Vector2u size = Camera::renderTarget->getSize();
    Camera::view.setSize(2 * Camera::width, 2 * Camera::width * size.y / size.x);
    Camera::renderTarget->setView(view);
}

float Camera::getWidth() {
    return width;
}

void Camera::onWindowResized() {
    sf::Vector2u size = Camera::renderTarget->getSize();
    Camera::view.setSize(2 * Camera::width, 2 * Camera::width * size.y / size.x);
    Camera::renderTarget->setView(view);
}

sf::RenderStates Camera::getRenderState() {
    sf::RenderStates cameraState;
    cameraState.transform
        .scale({1, -1})//Trick to flip image)))
        .combine(Camera::transform->getWorldToLocalTransform());//all things draw in camera coordinate system
    return cameraState;
}
