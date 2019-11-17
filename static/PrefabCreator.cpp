//
// Created by Олег Бобров on 17.11.2019.
//

#include "PrefabCreator.h"

Point *PrefabCreator::createPoint(int idx) {
    GameObject *obj = new GameObject();
    Point *point = obj->addComponent<Point>(idx);
    return point;
}

Line *PrefabCreator::createLine(int idx) {
    GameObject *obj = new GameObject();
    Line *line = obj->addComponent<Line>(idx);
    return line;
}

Train *PrefabCreator::createTrain(int idx) {
    GameObject *obj = new GameObject();
    Train *train = obj->addComponent<Train>(idx);
    return train;
}

Town *PrefabCreator::createTown(int idx) {
    GameObject *obj = new GameObject();
    Town *town = obj->addComponent<Town>(idx);
    return town;
}

Market *PrefabCreator::createMarket(int idx) {
    GameObject *obj = new GameObject();
    Market *market = obj->addComponent<Market>(idx);
    return market;
}

Storage *PrefabCreator::createStorage(int idx) {
    GameObject *obj = new GameObject();
    Storage *storage = obj->addComponent<Storage>(idx);
    return storage;
}

Camera *PrefabCreator::createCamera() {
    GameObject *obj = new GameObject();
    Camera *camera = obj->addComponent<Camera>();
    return camera;
}
