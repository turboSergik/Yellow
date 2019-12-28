//
// Created by Олег Бобров on 17.11.2019.
//

#ifndef WG_TRANSFORM_H
#define WG_TRANSFORM_H

#include <SFML/Graphics/Transform.hpp>
#include <list>
#include "../linalg/Vector2.hpp"

class GameObject;

class Transform {
private:
    lng::Vector2 position;
    lng::Vector2 localPosition;
    float rotation = 0;
    float localRotation = 0;
    lng::Vector2 scale = {1, 1};
    lng::Vector2 localScale = {1, 1};
    sf::Transform localToWorld;
    sf::Transform worldToLocal;

    lng::Vector2 up = {0, 1};
    lng::Vector2 right = {1, 0};

    Transform *parent = nullptr;
    std::list<Transform*> children;

    std::list<Transform*>::iterator it;

    void recalculateAccordingToLocals();
    
    friend class GameObject;
public:
    GameObject *gameObject = nullptr; //TODO: prevent changes of this field

    explicit Transform(GameObject *gameObject);
    ~Transform();

    const lng::Vector2 &getUp() const;

    const lng::Vector2 &getRight() const;

    const lng::Vector2 &getPosition() const;

    void setPosition(const lng::Vector2 &position);

    const lng::Vector2 &getLocalPosition() const;

    void setLocalPosition(const lng::Vector2 &localPosition);

    float getRotation() const;

    void setRotation(float rotation);

    float getLocalRotation() const;

    void setLocalRotation(float localRotation);

    const lng::Vector2 &getScale() const;

    void setScale(const lng::Vector2 &scale);

    const lng::Vector2 &getLocalScale() const;

    void setLocalScale(const lng::Vector2 &localScale);

    void setPositionAndRotation(const lng::Vector2 &position, float rotation);

    Transform *getParent() const;

    void setParent(Transform *parent, bool worldPositionStays = false);

    const sf::Transform &getLocalToWorldTransform() const;

    const sf::Transform &getWorldToLocalTransform() const;

    lng::Vector2 toLocalPosition (const lng::Vector2 &position) const;
    lng::Vector2 toLocalDirection (const lng::Vector2 &direction) const;
    lng::Vector2 toGlobalPosition (const lng::Vector2 &position) const;
    lng::Vector2 toGlobalDirection (const lng::Vector2 &direction) const;
    std::list<Transform*>::iterator begin();
    std::list<Transform*>::iterator end();
};


#endif //WG_TRANSFORM_H
