//
// Created by Олег Бобров on 17.11.2019.
//

#ifndef WG_PREFABCREATOR_H
#define WG_PREFABCREATOR_H


#include "../core/GameObject.h"

class PrefabCreator {
public:
    static GameObject *createCamera();
    static GameObject *createPoint(int idx);
    static GameObject *createLine(int idx);
    static GameObject *createTrain(int idx);
    static GameObject *createTown(int idx);
    static GameObject *createMarket(int idx);
    static GameObject *createStorage(int idx);
};


#endif //WG_PREFABCREATOR_H
