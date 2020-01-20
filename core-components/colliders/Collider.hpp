//
// Created by Олег Бобров on 10.01.2020.
//

#ifndef WG_COLLIDER_HPP
#define WG_COLLIDER_HPP


#include "../../core/Component.h"

class Collider : public Component {
private:
    static std::list<Collider*> colliders;
    std::list<Collider*>::iterator it;
public:
    static Collider * overlapPoint(const Vector2 & point);

    void start();
    void onDestroy();
    virtual bool isOverlapsPoint(const Vector2 & point) const = 0;
};


#endif //WG_COLLIDER_HPP