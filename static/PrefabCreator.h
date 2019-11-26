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
#include "../custom-components/GraphController.h"

class PrefabCreator {
public:
    static Camera *createCamera(sf::RenderTarget *renderTarget);
    static Point *createPoint(int idx);
    static Line *createLine(int idx);
    static Train *createTrain(int idx);
    static Town *createTown(int idx);
    static Market *createMarket(int idx);
    static Storage *createStorage(int idx);
    static GraphController *createRoot();
};


#endif //WG_PREFABCREATOR_H
