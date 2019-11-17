//
// Created by Олег Бобров on 17.11.2019.
//

#ifndef WG_COMPONENT_H
#define WG_COMPONENT_H

class GameObject;

class Component {
public:
    GameObject *gameObject;

    virtual void update();
    virtual void onDraw();
};


#endif //WG_COMPONENT_H
