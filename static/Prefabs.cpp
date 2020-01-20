//
// Created by Олег Бобров on 17.11.2019.
//

#include "Prefabs.h"
#include "../core-components/renderers/CircleRenderer.h"
#include "../core-components/renderers/LineRenderer.h"
#include "../core-components/renderers/RectangleRenderer.hpp"
#include "../custom-components/CameraController.h"
#include "../core-components/RigidBody.hpp"
#include "../core-components/colliders/CircleCollider.hpp"
#include "../core-components/colliders/BoxCollider.hpp"
#include "../core-components/ui/Button.hpp"
#include "../custom-components/ForceMethodPhysics/PointPhysics.hpp"
#include "../custom-components/ForceMethodPhysics/LinePhysics.hpp"
#include "../custom-components/ForceMethodPhysics/Draggable.hpp"


//TODO: add constants to easy manipulate default values of shapes
Point *Prefabs::point(int idx) {
    GameObject *obj = new GameObject();
    obj->name = "Point";
    CircleRenderer *circleRenderer = obj->addComponent<CircleRenderer>();
    circleRenderer->circle.setRadius(InterfaceConfig::largeR);
    circleRenderer->circle.setOrigin(InterfaceConfig::largeR, InterfaceConfig::largeR);
    circleRenderer->circle.setFillColor(InterfaceConfig::graphColor);
    circleRenderer->priotity = 1;
    Point *point = obj->addComponent<Point>(idx);
    RigidBody * rigidBody = obj->addComponent<RigidBody>();
    point->rigidBody = rigidBody;
    rigidBody->mass = 1.0f;
    rigidBody->drag = 0.0f;
    CircleCollider * circleCollider = obj->addComponent<CircleCollider>();
    circleCollider->setRadius(InterfaceConfig::largeR);
    obj->addComponent<PointPhysics>();
    return point;
}

Line *Prefabs::line(int idx) {
    GameObject * obj = new GameObject();
    obj->name = "Line";
    LineRenderer * lineRenderer =  obj->addComponent<LineRenderer>();
    lineRenderer->setColor(InterfaceConfig::graphColor);
    lineRenderer->priotity = 1;
    Line *line = obj->addComponent<Line>(idx);
    obj->addComponent<LinePhysics>();
    return line;
}

Train *Prefabs::train(int idx) {
    GameObject *obj = new GameObject();
    obj->name = "Train";
    CircleRenderer *circleRenderer = obj->addComponent<CircleRenderer>();
    circleRenderer->circle.setRadius(InterfaceConfig::smallR);
    circleRenderer->circle.setOrigin(InterfaceConfig::smallR, InterfaceConfig::smallR);
    circleRenderer->circle.setFillColor(InterfaceConfig::trainColor);
    circleRenderer->circle.setPointCount(3);
    circleRenderer->circle.setRotation(90);
    circleRenderer->priotity = -1;
    Train *train = obj->addComponent<Train>(idx);
    return train;
}

Town *Prefabs::town(int idx) {
    GameObject *obj = new GameObject();
    obj->name = "Town";
    CircleRenderer *circleRenderer = obj->addComponent<CircleRenderer>();
    circleRenderer->circle.setRadius(InterfaceConfig::smallR);
    circleRenderer->circle.setOrigin(InterfaceConfig::smallR, InterfaceConfig::smallR);
    circleRenderer->circle.setFillColor(InterfaceConfig::townColor);
    Town *town = obj->addComponent<Town>(idx);
    return town;
}

Market *Prefabs::market(int idx) {
    GameObject *obj = new GameObject();
    obj->name = "Market";
    CircleRenderer *circleRenderer = obj->addComponent<CircleRenderer>();
    circleRenderer->circle.setRadius(InterfaceConfig::smallR);
    circleRenderer->circle.setOrigin(InterfaceConfig::smallR, InterfaceConfig::smallR);
    circleRenderer->circle.setFillColor(InterfaceConfig::marketColor);
    Market *market = obj->addComponent<Market>(idx);
    return market;
}

Storage *Prefabs::storage(int idx) {
    GameObject *obj = new GameObject();
    obj->name = "Storage";
    CircleRenderer *circleRenderer = obj->addComponent<CircleRenderer>();
    circleRenderer->circle.setRadius(InterfaceConfig::smallR);
    circleRenderer->circle.setOrigin(InterfaceConfig::smallR, InterfaceConfig::smallR);
    circleRenderer->circle.setFillColor(InterfaceConfig::storageColor);
    Storage *storage = obj->addComponent<Storage>(idx);
    return storage;
}

Camera *Prefabs::camera(sf::RenderTarget *renderTarget) {
    GameObject *obj = new GameObject();
    obj->name = "Camera";
    Camera *camera = obj->addComponent<Camera>(renderTarget);
    obj->addComponent<CameraController>();
    return camera;
}

GraphController *Prefabs::graphRoot() {
    GameObject *obj = new GameObject();
    obj->name = "Root";
    GraphController *graphController = obj->addComponent<GraphController>();
    obj->addComponent<Draggable>();
    return graphController;
}

Button * Prefabs::button(float width, float height) {
    GameObject * obj = new GameObject();
    obj->name = "Button";
    Button * button = obj->addComponent<Button>();
    RectangleRenderer * renderer = obj->addComponent<RectangleRenderer>();
    BoxCollider * collider = obj->addComponent<BoxCollider>();
    renderer->rectangle.setSize({width, height});
    renderer->rectangle.setFillColor(InterfaceConfig::buttonColor);
    renderer->rectangle.setOrigin(width / 2, height / 2);
    collider->setWidthHeight(width, height);
    button->buttonCollider = collider;
    button->renderer = renderer;
    return button;
}
