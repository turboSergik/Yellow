//
// Created by Олег Бобров on 17.11.2019.
//

#include "PrefabCreator.h"

GameObject *PrefabCreator::createPoint(int idx) {
    GameObject *obj = new GameObject();
    obj->addComponent<Point>(idx);
    return obj;
}

GameObject *PrefabCreator::createLine(int idx) {
    GameObject *obj = new GameObject();
    obj->addComponent<Line>(idx);
    return obj;
}

GameObject *PrefabCreator::createTrain(int idx) {
    GameObject *obj = new GameObject();
    obj->addComponent<Train>(idx);
    return obj;
}

GameObject *PrefabCreator::createTown(int idx) {
    GameObject *obj = new GameObject();
    obj->addComponent<Town>(idx);
    return obj;
}

GameObject *PrefabCreator::createMarket(int idx) {
    GameObject *obj = new GameObject();
    obj->addComponent<Market>(idx);
    return obj;
}

GameObject *PrefabCreator::createStorage(int idx) {
    GameObject *obj = new GameObject();
    obj->addComponent<Storage>(idx);
    return obj;
}

GameObject *PrefabCreator::createCamera() {
    GameObject *obj = new GameObject();
    obj->addComponent<Camera>();
    return obj;
}
