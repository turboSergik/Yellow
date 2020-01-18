//
// Created by Олег Бобров on 17.11.2019.
//

#include "Prefabs.h"
#include "../core-components/renderers/CircleRenderer.h"
#include "../core-components/renderers/LineRenderer.h"
#include "../custom-components/CameraController.h"
#include "../core-components/RigidBody.hpp"
#include "../core-components/colliders/CircleCollider.hpp"


//TODO: add constants to easy manipulate default values of shapes
Point *Prefabs::point(int idx) {
    GameObject *obj = new GameObject();
    CircleRenderer *circleRenderer = obj->addComponent<CircleRenderer>();
    circleRenderer->circle.setRadius(InterfaceConfig::largeR);
    circleRenderer->circle.setOrigin(InterfaceConfig::largeR, InterfaceConfig::largeR);
    circleRenderer->circle.setFillColor(InterfaceConfig::graphColor);
    Point *point = obj->addComponent<Point>(idx);
    RigidBody * rigidBody = obj->addComponent<RigidBody>();
    rigidBody->mass = 1.0f;
    rigidBody->drag = 0.0f;
    CircleCollider * circleCollider = obj->addComponent<CircleCollider>();
    circleCollider->setRadius(InterfaceConfig::largeR);
    return point;
}

Line *Prefabs::line(int idx) {
    GameObject * obj = new GameObject();
    LineRenderer * lineRenderer =  obj->addComponent<LineRenderer>();
    lineRenderer->setColor(InterfaceConfig::graphColor);
    Line *line = obj->addComponent<Line>(idx);
    return line;
}

Train *Prefabs::train(int idx) {
    GameObject *obj = new GameObject();
    CircleRenderer *circleRenderer = obj->addComponent<CircleRenderer>();
    circleRenderer->circle.setRadius(InterfaceConfig::smallR);
    circleRenderer->circle.setOrigin(InterfaceConfig::smallR, InterfaceConfig::smallR);
    circleRenderer->circle.setFillColor(InterfaceConfig::trainColor);
    circleRenderer->circle.setPointCount(3);
    circleRenderer->circle.setRotation(90);
    Train *train = obj->addComponent<Train>(idx);
    return train;
}

Town *Prefabs::town(int idx) {
    GameObject *obj = new GameObject();
    CircleRenderer *circleRenderer = obj->addComponent<CircleRenderer>();
    circleRenderer->circle.setRadius(InterfaceConfig::smallR);
    circleRenderer->circle.setOrigin(InterfaceConfig::smallR, InterfaceConfig::smallR);
    circleRenderer->circle.setFillColor(InterfaceConfig::townColor);
    Town *town = obj->addComponent<Town>(idx);
    return town;
}

Market *Prefabs::market(int idx) {
    GameObject *obj = new GameObject();
    CircleRenderer *circleRenderer = obj->addComponent<CircleRenderer>();
    circleRenderer->circle.setRadius(InterfaceConfig::smallR);
    circleRenderer->circle.setOrigin(InterfaceConfig::smallR, InterfaceConfig::smallR);
    circleRenderer->circle.setFillColor(InterfaceConfig::marketColor);
    Market *market = obj->addComponent<Market>(idx);
    return market;
}

Storage *Prefabs::storage(int idx) {
    GameObject *obj = new GameObject();
    CircleRenderer *circleRenderer = obj->addComponent<CircleRenderer>();
    circleRenderer->circle.setRadius(InterfaceConfig::smallR);
    circleRenderer->circle.setOrigin(InterfaceConfig::smallR, InterfaceConfig::smallR);
    circleRenderer->circle.setFillColor(InterfaceConfig::storageColor);
    Storage *storage = obj->addComponent<Storage>(idx);
    return storage;
}

Camera *Prefabs::camera(sf::RenderTarget *renderTarget) {
    GameObject *obj = new GameObject();
    Camera *camera = obj->addComponent<Camera>(renderTarget);
    obj->addComponent<CameraController>();
    return camera;
}

GraphController *Prefabs::graphRoot() {
    GameObject *obj = new GameObject();
    GraphController *graphController = obj->addComponent<GraphController>();
    return graphController;
}
