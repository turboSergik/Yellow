//
// Created by Олег Бобров on 17.11.2019.
//

#include "CameraController.h"
#include "../static/Time.h"
#include "../static/Input.hpp"
#include "../core-components/Camera.h"
#include "../utility/Mathf.hpp"
#include <iostream>
#include <cmath>

void CameraController::start() {
    this->targetWidth = Camera::mainCamera->getWidth();
}

void CameraController::update() {
    Camera * mainCamera = Camera::mainCamera;
    Vector2 pos = this->transform->getLocalPosition();
    if (Input::isKey(up)) {
        pos += {0.f, mainCamera->getWidth()*moveFactor*Time::deltaTime};
        this->transform->setLocalPosition(pos);
    }
    if (Input::isKey(left)) {
        pos -= {mainCamera->getWidth()*moveFactor*Time::deltaTime,0.f };
        this->transform->setLocalPosition(pos);
    }
    if (Input::isKey(down)) {
        pos -= {0.f, mainCamera->getWidth()*moveFactor*Time::deltaTime};
        this->transform->setLocalPosition(pos);
    }
    if (Input::isKey(right)) {
        pos += {mainCamera->getWidth()*moveFactor*Time::deltaTime,0.f };
        this->transform->setLocalPosition(pos);
    }
    if (Input::getWheelScrolled()) {

        this->targetWidth -=
                this->targetWidth *
                this->scrollFactor *
                Input::getWheelScrollEvent().delta;
        this->targetWidth = Mathf::clamp(this->targetWidth, this->minSize, this->maxSize);
    }
    float width = mainCamera->getWidth();
    if (fabs(targetWidth - width) > 1e-5) {
        mainCamera->setWidth(
                Mathf::lerp(width, this->targetWidth, this->scrollSpeed*Time::deltaTime));
    }
}
