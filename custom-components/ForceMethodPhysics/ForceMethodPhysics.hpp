//
// Created by Олег Бобров on 19.01.2020.
//

#ifndef WG_FORCEMETHODPHYSICS_HPP
#define WG_FORCEMETHODPHYSICS_HPP

#include "../../core/GameObject.h"
#include "../../core-components/colliders/Collider.hpp"
#include "../../core-components/Camera.h"
#include "../../static/Input.hpp"
#include "../../core-components/RigidBody.hpp"
#include "../../static/Database.h"
#include "../../utility/Config.hpp"

class ForceMethodPhysics : public Component {
private:
    Collider * draggingCollider = nullptr;
public:
    void start();
    void update();
    void fixedUpdate();
    void onDestroy();
    void onMapLayer10(const nlohmann::json & json);
};


#endif //WG_FORCEMETHODPHYSICS_HPP
