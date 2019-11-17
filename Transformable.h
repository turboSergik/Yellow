//
// Created by Олег Бобров on 17.11.2019.
//

#ifndef WG_TRANSFORMABLE_H
#define WG_TRANSFORMABLE_H

#include <SFML/Graphics/Transform.hpp>
#include <list>

class Transformable {
private:
    sf::Vector2f position;
    sf::Vector2f localPosition;
    float rotation;
    float localRotation;
    sf::Vector2f scale;
    sf::Vector2f localScale;
    sf::Transform localToWorld;
    sf::Transform worldToLocal;

    sf::Vector2f up;
    sf::Vector2f right;

    Transformable *parent;
    std::list<Transformable*> children;

    void recalculateAccordingToLocals();
public:
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

    Transformable *getParent() const;

    void setParent(Transformable *parent, bool worldPositionStays = false);

    sf::Vector2f transformToLocalPosition (const sf::Vector2f &position) const;
    sf::Vector2f transformToLocalDirection (const sf::Vector2f &direction) const;
    sf::Vector2f transformToGlobalPosition (const sf::Vector2f &position) const;
    sf::Vector2f transformToGlobalDirection (const sf::Vector2f &direction) const;
};


#endif //WG_TRANSFORMABLE_H
