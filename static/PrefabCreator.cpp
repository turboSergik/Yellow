//
// Created by Олег Бобров on 17.11.2019.
//

#include "PrefabCreator.h"
#include "../core-components/renderers/CircleRenderer.h"
#include "../core-components/renderers/LineRenderer.h"
#include "../custom-components/CameraController.h"


//TODO: add constants to easy manipulate default values of shapes
Point *PrefabCreator::createPoint(int idx) {
    GameObject *obj = new GameObject();
    CircleRenderer *circleRenderer = obj->addComponent<CircleRenderer>();
    circleRenderer->circle.setRadius(40);
    circleRenderer->circle.setOrigin(40, 40);
    Point *point = obj->addComponent<Point>(idx);
    return point;
}

Line *PrefabCreator::createLine(int idx) {
    GameObject *obj = new GameObject();
    obj->addComponent<LineRenderer>();
    Line *line = obj->addComponent<Line>(idx);
    return line;
}

Train *PrefabCreator::createTrain(int idx) {
    GameObject *obj = new GameObject();
    CircleRenderer *circleRenderer = obj->addComponent<CircleRenderer>();
    circleRenderer->circle.setRadius(30);
    circleRenderer->circle.setOrigin(30, 30);
    circleRenderer->circle.setFillColor(sf::Color::Green);
    circleRenderer->circle.setPointCount(3);
    Train *train = obj->addComponent<Train>(idx);
    return train;
}

Town *PrefabCreator::createTown(int idx) {
    GameObject *obj = new GameObject();
    CircleRenderer *circleRenderer = obj->addComponent<CircleRenderer>();
    circleRenderer->circle.setRadius(30);
    circleRenderer->circle.setOrigin(30, 30);
    circleRenderer->circle.setFillColor(sf::Color::Black);
    Town *town = obj->addComponent<Town>(idx);
    return town;
}

Market *PrefabCreator::createMarket(int idx) {
    GameObject *obj = new GameObject();
    CircleRenderer *circleRenderer = obj->addComponent<CircleRenderer>();
    circleRenderer->circle.setRadius(30);
    circleRenderer->circle.setOrigin(30, 30);
    circleRenderer->circle.setFillColor(sf::Color::Red);
    Market *market = obj->addComponent<Market>(idx);
    return market;
}

Storage *PrefabCreator::createStorage(int idx) {
    GameObject *obj = new GameObject();
    CircleRenderer *circleRenderer = obj->addComponent<CircleRenderer>();
    circleRenderer->circle.setRadius(30);
    circleRenderer->circle.setOrigin(30, 30);
    circleRenderer->circle.setFillColor(sf::Color::Blue);
    Storage *storage = obj->addComponent<Storage>(idx);
    return storage;
}

Camera *PrefabCreator::createCamera(sf::RenderTarget *renderTarget) {
    GameObject *obj = new GameObject();
    Camera *camera = obj->addComponent<Camera>(renderTarget);
    obj->addComponent<CameraController>();
    return camera;
}

GraphController *PrefabCreator::createRoot() {
    GameObject *obj = new GameObject();
    GraphController *graphController = obj->addComponent<GraphController>();
    return graphController;
}
