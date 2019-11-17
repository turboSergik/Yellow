//
// Created by Олег Бобров on 17.11.2019.
//

#ifndef WG_COMPONENT_H
#define WG_COMPONENT_H

#include "Transform.h"

class Component {
public:
    GameObject *gameObject; //TODO: prevent changes of this field
    Transform *transform;

    virtual void update();
    virtual void onDraw();
};


#endif //WG_COMPONENT_H
