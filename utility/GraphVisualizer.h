//
// Created by Олег Бобров on 21.11.2019.
//

#ifndef WG_GRAPHVISUALIZER_H
#define WG_GRAPHVISUALIZER_H


#include <unordered_map>
#include <list>
#include <vector>
#include <SFML/System/Vector2.hpp>
#include <thread>
#include <mutex>

class GraphVisualizer {
private:
    std::thread forceMethodThread;
    std::mutex mutex;
    std::unordered_map<int, std::list<int>> graph;
    std::unordered_map<int, sf::Vector2f> positions;
    std::unordered_map<int, sf::Vector2f> velocities;
    std::unordered_map<int, sf::Vector2f> deltaVelocities;
    //TODO: optimize code

    void increase(std::unordered_map<int, sf::Vector2f> & value,
            const std::unordered_map<int, sf::Vector2f> & delta);
    void forceMethodThreadFunction();
    float randFloat();
    std::unordered_map<int, sf::Vector2f> randomizePositions(float maxValue);
    std::unordered_map<int, sf::Vector2f> randomizeVelocities(float maxValue);
    float deltaVelocitiesSquaredSum();
    float friction(float v);
    float edge(float dl);
    float notEdge(float dl);
    bool isEdgeExists(int from, int to);
    std::unordered_map<int, sf::Vector2f> calculateForces();
    std::unordered_map<int, sf::Vector2f> forceFrictionModel();
public:
    ~GraphVisualizer();
    void setGraph(const std::unordered_map<int, std::list<int>> & graph);
    const std::unordered_map<int, sf::Vector2f> & getPositions() const;
    const std::unordered_map<int, sf::Vector2<float>> & forceMethodIteration();
    void startForceMethodThread();
    bool isForceMethodFinished();
    void lock();
    void unlock();
};


#endif //WG_GRAPHVISUALIZER_H
