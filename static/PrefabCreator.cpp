//
// Created by Олег Бобров on 17.11.2019.
//

#include "PrefabCreator.h"
#include "../custom-components/Point.h"
#include "../custom-components/Post.h"
#include "../custom-components/Train.h"
#include "../custom-components/Line.h"
#include "../base-components/Camera.h"

GameObject *PrefabCreator::createPoint(int idx) {
    GameObject *obj = new GameObject();
    obj->addComponent<Point>();
    return obj;
}

GameObject *PrefabCreator::createLine(int idx) {
    GameObject *obj = new GameObject();
    obj->addComponent<Line>();
    return obj;
}

GameObject *PrefabCreator::createTrain(int idx) {
    GameObject *obj = new GameObject();
    obj->addComponent<Train>();
    return obj;
}

GameObject *PrefabCreator::createTown(int idx) {
    GameObject *obj = new GameObject();
    obj->addComponent<Town>();
    return obj;
}

GameObject *PrefabCreator::createMarket(int idx) {
    GameObject *obj = new GameObject();
    obj->addComponent<Market>();
    return obj;
}

GameObject *PrefabCreator::createStorage(int idx) {
    GameObject *obj = new GameObject();
    obj->addComponent<Storage>();
    return obj;
}

GameObject *PrefabCreator::createCamera() {
    GameObject *obj = new GameObject();
    obj->addComponent<Camera>();
    return obj;
}
