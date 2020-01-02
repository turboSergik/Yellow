//
// Created by Олег Бобров on 17.11.2019.
//

#include "CameraController.h"
#include "../static/Time.h"
#include "../static/Input.hpp"
#include "../core-components/Camera.h"
#include <iostream>

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
        float width = mainCamera->getWidth();
        width -= width * CameraController::scrollFactor * Input::getWheelScrollEvent().delta * Time::deltaTime;
        //std::cout << width << std::endl;
        if (width < CameraController::minSize) {
            width = CameraController::minSize;
        }
        if (width > CameraController::maxSize) {
            width = CameraController::maxSize;
        }
        mainCamera->setWidth(width);
    }
}
