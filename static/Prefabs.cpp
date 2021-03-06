//
// Created by Олег Бобров on 17.11.2019.
//

#include "Prefabs.h"
#include "../core-components/renderers/CircleRenderer.h"
#include "../core-components/renderers/LineRenderer.h"
#include "../custom-components/CameraController.h"


//TODO: add constants to easy manipulate default values of shapes
Point *Prefabs::point(int idx) {
    GameObject *obj = new GameObject();
    CircleRenderer *circleRenderer = obj->addComponent<CircleRenderer>();
    circleRenderer->circle.setRadius(40);
    circleRenderer->circle.setOrigin(40, 40);
    Point *point = obj->addComponent<Point>(idx);
    return point;
}

Line *Prefabs::line(int idx) {
    GameObject *obj = new GameObject();
    obj->addComponent<LineRenderer>();
    Line *line = obj->addComponent<Line>(idx);
    return line;
}

Train *Prefabs::train(int idx) {
    GameObject *obj = new GameObject();
    CircleRenderer *circleRenderer = obj->addComponent<CircleRenderer>();
    circleRenderer->circle.setRadius(30);
    circleRenderer->circle.setOrigin(30, 30);
    circleRenderer->circle.setFillColor(sf::Color::Green);
    circleRenderer->circle.setPointCount(3);
    circleRenderer->circle.setRotation(90);
    Train *train = obj->addComponent<Train>(idx);
    return train;
}

Town *Prefabs::town(int idx) {
    GameObject *obj = new GameObject();
    CircleRenderer *circleRenderer = obj->addComponent<CircleRenderer>();
    circleRenderer->circle.setRadius(30);
    circleRenderer->circle.setOrigin(30, 30);
    circleRenderer->circle.setFillColor(sf::Color::Black);
    Town *town = obj->addComponent<Town>(idx);
    return town;
}

Market *Prefabs::market(int idx) {
    GameObject *obj = new GameObject();
    CircleRenderer *circleRenderer = obj->addComponent<CircleRenderer>();
    circleRenderer->circle.setRadius(30);
    circleRenderer->circle.setOrigin(30, 30);
    circleRenderer->circle.setFillColor(sf::Color::Red);
    Market *market = obj->addComponent<Market>(idx);
    return market;
}

Storage *Prefabs::storage(int idx) {
    GameObject *obj = new GameObject();
    CircleRenderer *circleRenderer = obj->addComponent<CircleRenderer>();
    circleRenderer->circle.setRadius(30);
    circleRenderer->circle.setOrigin(30, 30);
    circleRenderer->circle.setFillColor(sf::Color::Blue);
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
