//
// Created by Олег Бобров on 10.12.2019.
//

#ifndef WG_PLAYERCONTROLLER_HPP
#define WG_PLAYERCONTROLLER_HPP


#include "../core/Component.h"
#include "Point.h"

class PlayerController : public Component {
private:
    float timeFromLastTurn = 0.5f;
    float waitingTime = 0.5f;//time to wait (show animations)
public:
    Point * playerPoint = nullptr;
    Town * playerTown = nullptr;
    std::vector<Train*> playerTrains;
    std::vector<Market*> markets;
    std::vector<Storage*> storages;
    bool isMapUpdated = false;

    void update();
    void tryTrainUpdate(Train*);
    void strategyIteration();
    std::pair<std::vector<int>, int> trainWayToProducts(Train*);
    std::pair<std::vector<int>, int> trainWayToStorage(Train*);


};


#endif //WG_PLAYERCONTROLLER_HPP
