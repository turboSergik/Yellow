//
// Created by Олег Бобров on 21.11.2019.
//

#ifndef WG_GRAPHVISUALIZER_H
#define WG_GRAPHVISUALIZER_H


#include <unordered_map>
#include <list>
#include <vector>
#include <SFML/System/Vector2.hpp>

class GraphVisualizer {
private:
    std::unordered_map<int, std::list<int>> graph;
    std::unordered_map<int, sf::Vector2f> positions;
    std::unordered_map<int, sf::Vector2f> velocities;
    std::unordered_map<int, sf::Vector2f> fv;

    void add(
            std::unordered_map<int, sf::Vector2f> &x,
            const std::unordered_map<int, sf::Vector2f> &dx);
    float randFloat();
    std::unordered_map<int, sf::Vector2f> randomPosition(
            const std::unordered_map<int,
                    std::list<int>> & graph, float max);
    std::unordered_map<int, sf::Vector2f> randomVelocity(
            const std::unordered_map<int,
                    std::list<int>> & graph,
            float max);
    float forceSum(const std::unordered_map<int, sf::Vector2f> & forces);
    float friction(float v);
    float edge(float dl);
    float noedge(float dl);
    bool hasEdge(
            const std::unordered_map<int,
                    std::list<int>> & graph,
            int from,
            int to);
    std::unordered_map<int, sf::Vector2f> force(
            const std::unordered_map<int, std::list<int>> & graph,
            const std::unordered_map<int, sf::Vector2f> & positions);
    std::unordered_map<int, sf::Vector2f> ForceFrictionModel(
            const std::unordered_map<int, std::list<int>> & graph,
            std::unordered_map<int, sf::Vector2f> & positions,
            std::unordered_map<int, sf::Vector2f> & velocities);
public:
    void setGraph(const std::unordered_map<int, std::list<int>> & graph);
    const std::unordered_map<int, sf::Vector2<float>> & forceMethodIteration();
    const std::unordered_map<int, sf::Vector2<float>> & forceMethod();
};


#endif //WG_GRAPHVISUALIZER_H
