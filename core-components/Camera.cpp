//
// Created by Олег Бобров on 15.11.2019.
//

#include "Camera.h"
#include "../linalg/Vector2.hpp"

Camera * Camera::mainCamera = nullptr;

Camera::Camera(sf::RenderTarget *renderTarget) {
    if (!Camera::mainCamera) {
        Camera::mainCamera = this; //not a singleton, just first (main) camera
    }
    this->renderTarget = renderTarget;
    this->view = renderTarget->getView();
    this->width = 1000;
    sf::Vector2u size = this->renderTarget->getSize();
    this->view.setSize(2 * this->width, 2 * this->width * size.y / size.x);
    this->view.setCenter(0, 0);
    this->renderTarget->setView(view);
}

void Camera::setWindowSize(float width, float height) {
    Camera::view.setSize(2 * Camera::width, 2 * Camera::width * height / width);
    Camera::renderTarget->setView(view);
}

void Camera::setWindowSize(const Vector2 &size) {
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

//TODO optimize this
Vector2 Camera::screenToWorldPoint(const Vector2 & point) {
    sf::Transform m = this->transform->getLocalToWorldTransform();
    m.scale({1, -1});
    return  m.transformPoint(renderTarget->mapPixelToCoords(sf::Vector2i(point)));
}

//TODO refactor this
Vector2 Camera::worldToScreenPoint(const Vector2 & point) {
    sf::Transform m;
    m.scale({1, -1}).combine(Camera::transform->getWorldToLocalTransform());
    return sf::Vector2f(renderTarget->mapCoordsToPixel(m.transformPoint(point)));
}
