#ifndef GRAPHDRAWING_H
#define GRAPHDRAWING_H

#include <unordered_map>
#include <list>
#include <vector>
#include <SFML/System/Vector2.hpp>

const int CIRCLE_RADIUS = 70;

const float K_KOEFFICIENT = 0.01;
const float L_KOEFFICIENT = 175;
const float Q_KOEFFICIENT = 10;
const float Dt = 0.1;
const float MIN_KINETIC = 50;
const float MAX_VELOCITY = 100;
const float MASS = 1000;
const float FRICTION = 10000;
const float VERTEXES_DISTANCE = 50;

const float TREE_LAYER_DISTANSE = 100;

namespace GraphDrawing {
    std::unordered_map<int, sf::Vector2<float>> 
    forceMethod(const std::unordered_map<int, std::list<int>> & graph);
};

#endif // GRAPHDRAWING_H
