//
// Created by Олег Бобров on 17.11.2019.
//

#include "PrefabCreator.h"
#include "../core-components/renderers/CircleRenderer.h"
#include "../core-components/renderers/LineRenderer.h"

Point *PrefabCreator::createPoint(int idx, Transform *parent) {
    GameObject *obj = new GameObject();
    obj->transform->setParent(parent);
    CircleRenderer *circleRenderer = obj->addComponent<CircleRenderer>();
    circleRenderer->circle.setRadius(20);
    circleRenderer->circle.setOrigin(20, 20);
    Point *point = obj->addComponent<Point>(idx);
    return point;
}

Line *PrefabCreator::createLine(int idx, Transform *parent) {
    GameObject *obj = new GameObject();
    obj->transform->setParent(parent);
    obj->addComponent<LineRenderer>();
    Line *line = obj->addComponent<Line>(idx);
    return line;
}

Train *PrefabCreator::createTrain(int idx, Transform *parent) {
    GameObject *obj = new GameObject();
    Train *train = obj->addComponent<Train>(idx);
    obj->transform->setParent(parent);
    return train;
}

Town *PrefabCreator::createTown(int idx, Transform *parent) {
    GameObject *obj = new GameObject();
    Town *town = obj->addComponent<Town>(idx);
    obj->transform->setParent(parent);
    return town;
}

Market *PrefabCreator::createMarket(int idx, Transform *parent) {
    GameObject *obj = new GameObject();
    Market *market = obj->addComponent<Market>(idx);
    obj->transform->setParent(parent);
    return market;
}

Storage *PrefabCreator::createStorage(int idx, Transform *parent) {
    GameObject *obj = new GameObject();
    Storage *storage = obj->addComponent<Storage>(idx);
    obj->transform->setParent(parent);
    return storage;
}

Camera *PrefabCreator::createCamera() {
    GameObject *obj = new GameObject();
    Camera *camera = obj->addComponent<Camera>();
    return camera;
}
