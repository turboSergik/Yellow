//
// Created by Олег Бобров on 17.11.2019.
//

#ifndef WG_TRANSFORM_H
#define WG_TRANSFORM_H

#include <SFML/Graphics/Transform.hpp>
#include <list>

class GameObject;

class Transform {
private:
    sf::Vector2f position;
    sf::Vector2f localPosition;
    float rotation;
    float localRotation;
    sf::Vector2f scale = {1, 1};
    sf::Vector2f localScale = {1, 1};
    sf::Transform localToWorld;
    sf::Transform worldToLocal;

    sf::Vector2f up;
    sf::Vector2f right;

    Transform *parent = nullptr;
    std::list<Transform*> children;

    void recalculateAccordingToLocals();
public:
    GameObject *gameObject = nullptr; //TODO: prevent changes of this field

    explicit Transform(GameObject *gameObject);
    ~Transform();

    const sf::Vector2f &getUp() const;

    const sf::Vector2f &getRight() const;

    const sf::Vector2f &getPosition() const;

    void setPosition(const sf::Vector2f &position);

    const sf::Vector2f &getLocalPosition() const;

    void setLocalPosition(const sf::Vector2f &localPosition);

    float getRotation() const;

    void setRotation(float rotation);

    float getLocalRotation() const;

    void setLocalRotation(float localRotation);

    const sf::Vector2f &getScale() const;

    void setScale(const sf::Vector2f &scale);

    const sf::Vector2f &getLocalScale() const;

    void setLocalScale(const sf::Vector2f &localScale);

    void setPositionAndRotation(const sf::Vector2f &position, float rotation);

    Transform *getParent() const;

    void setParent(Transform *parent, bool worldPositionStays = false);

    const sf::Transform &getLocalToWorldTransform() const;

    const sf::Transform &getWorldToLocalTransform() const;

    sf::Vector2f toLocalPosition (const sf::Vector2f &position) const;
    sf::Vector2f toLocalDirection (const sf::Vector2f &direction) const;
    sf::Vector2f toGlobalPosition (const sf::Vector2f &position) const;
    sf::Vector2f toGlobalDirection (const sf::Vector2f &direction) const;
    std::list<Transform*>::iterator begin();
    std::list<Transform*>::iterator end();
};


#endif //WG_TRANSFORM_H
