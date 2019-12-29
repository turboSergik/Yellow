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
#include <atomic>
#include "../linalg/Vector2.hpp"

class GraphVisualizer {
private:
    std::thread forceMethodThread;
    std::mutex mutex;
    std::atomic<bool> stopThread;
    std::unordered_map<int, std::list<int>> graph;
    std::unordered_map<int, Vector2> positions;
    std::unordered_map<int, Vector2> velocities;
    std::unordered_map<int, Vector2> deltaVelocities;
    //TODO: optimize code

    void increase(std::unordered_map<int, Vector2> & value,
            const std::unordered_map<int, Vector2> & delta);
    void forceMethodThreadFunction();
    float randFloat();
    std::unordered_map<int, Vector2> randomizePositions(float maxValue);
    std::unordered_map<int, Vector2> randomizeVelocities(float maxValue);
    float deltaVelocitiesSquaredSum();
    float friction(float v);
    float edge(float dl);
    float notEdge(float dl);
    bool isEdgeExists(int from, int to);
    std::unordered_map<int, Vector2> calculateForces();
    std::unordered_map<int, Vector2> forceFrictionModel();
public:
    GraphVisualizer();
    ~GraphVisualizer();
    void setGraph(const std::unordered_map<int, std::list<int>> & graph);
    const std::unordered_map<int, Vector2> & getPositions() const;
    const std::unordered_map<int, Vector2> & forceMethodIteration();
    void startForceMethodThread();
    bool isForceMethodFinished();
    void lock();
    void unlock();
};


#endif //WG_GRAPHVISUALIZER_H
