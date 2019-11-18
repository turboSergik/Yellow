//
// Created by Олег Бобров on 17.11.2019.
//

#ifndef WG_PREFABCREATOR_H
#define WG_PREFABCREATOR_H

#include "../core/GameObject.h"
#include "../custom-components/Point.h"
#include "../custom-components/Post.h"
#include "../custom-components/Train.h"
#include "../custom-components/Line.h"
#include "../core-components/Camera.h"

class PrefabCreator {
public:
    static Camera *createCamera();
    static Point *createPoint(int idx, Transform *parent = nullptr);
    static Line *createLine(int idx, Transform *parent = nullptr);
    static Train *createTrain(int idx, Transform *parent = nullptr);
    static Town *createTown(int idx, Transform *parent = nullptr);
    static Market *createMarket(int idx, Transform *parent = nullptr);
    static Storage *createStorage(int idx, Transform *parent = nullptr);
};


#endif //WG_PREFABCREATOR_H
