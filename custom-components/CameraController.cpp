//
// Created by Олег Бобров on 17.11.2019.
//

#include "CameraController.h"
#include "../static/Time.h"
#include "../static/Input.hpp"
#include "../core-components/Camera.h"
#include <iostream>
#include <cmath>

void CameraController::start() {
    this->targetWidth = Camera::mainCamera->getWidth();
}

void CameraController::update() {
    Camera * mainCamera = Camera::mainCamera;
    Vector2 pos = CameraController::transform->getLocalPosition();
    if (Input::isKey(up)) {
        pos += {0.f, mainCamera->getWidth()*moveFactor*Time::deltaTime};
        CameraController::transform->setLocalPosition(pos);
    }
    if (Input::isKey(left)) {
        pos -= {mainCamera->getWidth()*moveFactor*Time::deltaTime,0.f };
        CameraController::transform->setLocalPosition(pos);
    }
    if (Input::isKey(down)) {
        pos -= {0.f, mainCamera->getWidth()*moveFactor*Time::deltaTime};
        CameraController::transform->setLocalPosition(pos);
    }
    if (Input::isKey(right)) {
        pos += {mainCamera->getWidth()*moveFactor*Time::deltaTime,0.f };
        CameraController::transform->setLocalPosition(pos);
    }
    if (Input::getWheelScrolled()) {

        this->targetWidth -= this->targetWidth * CameraController::scrollFactor * Input::getWheelScrollEvent().delta;
        //std::cout << width << std::endl;
        if (this->targetWidth < CameraController::minSize) {
            this->targetWidth = CameraController::minSize;
        }
        if (this->targetWidth > CameraController::maxSize) {
            this->targetWidth = CameraController::maxSize;
        }
    }
    float width = mainCamera->getWidth();
    if (fabs(targetWidth - width) > 1e-5) {
        mainCamera->setWidth(width + (targetWidth - width)*this->scrollSpeed*Time::deltaTime);
    }
}
