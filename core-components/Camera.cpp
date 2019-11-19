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
    setWidth(10);
}

void Camera::setWindowSize(float width, float height) {
    Camera::view.setSize(2 * Camera::width, 2 * Camera::width * height / width);
    Camera::renderTarget->setView(view);
}

void Camera::setWindowSize(const sf::Vector2f &size) {
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
