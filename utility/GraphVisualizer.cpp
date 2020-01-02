//
// Created by Олег Бобров on 21.11.2019.
//

#include "GraphVisualizer.h"
#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <numeric>
#include <iostream>

const float eps = 1e-4f;

const float dt = 0.01f;
const float init_size = 200.f;
const float maxVelocity = 50.f;

const float k = 0.5f;
const float q2 = 2000.f;
const float gamma_s = 0.1f;
const float l0 = 100.f;


void GraphVisualizer::setGraph(const std::unordered_map<int, std::list<int>> & graph) {
    GraphVisualizer::graph = graph;
    GraphVisualizer::positions = GraphVisualizer::randomizePositions(init_size * sqrtf(graph.size()));
    GraphVisualizer::velocities = GraphVisualizer::randomizeVelocities(maxVelocity);
}

void GraphVisualizer::increase(std::unordered_map<int, Vector2> & value,
        const std::unordered_map<int, Vector2> & delta) {
    for (auto & pair : value) {
        pair.second += delta.at(pair.first) * dt;
    }
}

float GraphVisualizer::randFloat() {
    return static_cast<float>(rand())/static_cast<float>(RAND_MAX);
}

std::unordered_map<int, Vector2> GraphVisualizer::randomizePositions(float maxValue) {
    std::unordered_map<int, Vector2> r;
    for (auto & pair : GraphVisualizer::graph) {
        r[pair.first] = Vector2{randFloat(), randFloat()} * maxValue;
    }
    return r;
}

std::unordered_map<int, Vector2> GraphVisualizer::randomizeVelocities(float maxValue) {
    std::unordered_map<int, Vector2> r;
    for (auto & pair : GraphVisualizer::graph) {
        r[pair.first] = Vector2{randFloat() - 1, randFloat() - 1} * maxValue;
    }
    return r;
}

float GraphVisualizer::deltaVelocitiesSquaredSum() {
    return std::accumulate(
            GraphVisualizer::deltaVelocities.begin(), GraphVisualizer::deltaVelocities.end(), 0.f,
            [](float first, const std::pair<int, Vector2> &pair) {
                return first + sqrtf(pair.second.x * pair.second.x +
                                     pair.second.y * pair.second.y);
            });
}


float GraphVisualizer::friction(float v) {
    return gamma_s * v * v;
}

float GraphVisualizer::edge(float dl) {
    return q2 / (dl*dl) - k * logf(dl / l0);
}

float GraphVisualizer::notEdge(float dl) {
    return q2 / (dl*dl);
}

bool GraphVisualizer::isEdgeExists(int from, int to) {
    for (const int & point : GraphVisualizer::graph.at(from)) {
        if (point == to) {
            return true;
        }
    }
    return false;
}

std::unordered_map<int, Vector2> GraphVisualizer::calculateForces() {
    std::unordered_map<int, Vector2> forces;
    for (const auto & pair : GraphVisualizer::positions) {
        forces[pair.first] = {0.f, 0.f};
    }

    for (const auto & pairI : GraphVisualizer::positions) {
        for (const auto & pairJ : GraphVisualizer::positions) {
            Vector2 dv = pairI.second - pairJ.second;
            float dl = sqrtf(dv.x * dv.x + dv.y * dv.y);
            if (dl == 0.f) {
                continue;
            }
            float fs = isEdgeExists(pairI.first, pairJ.first)
                       ? edge(dl) : notEdge(dl);

            Vector2 f = fs * dv;

            forces[pairI.first] += f;
            forces[pairJ.first] -= f;

        }
    }
    return forces;
}

std::unordered_map<int, Vector2> GraphVisualizer::forceFrictionModel() {
    std::unordered_map<int, Vector2> forces = calculateForces();

    for (auto & pair : GraphVisualizer::velocities) {
        Vector2 & vec = pair.second;
        float dl = sqrtf(vec.x * vec.x + vec.y * vec.y);

        if (dl == 0.f) {
            continue;
        }

        forces[pair.first] -= friction(dl) * pair.second / dl;
    }

    return forces;
}

GraphVisualizer::GraphVisualizer() {
    stopThread.store(false, std::memory_order_relaxed);
}

const std::unordered_map<int, Vector2> & GraphVisualizer::forceMethodIteration() {

    GraphVisualizer::deltaVelocities = forceFrictionModel();

    increase(GraphVisualizer::velocities, deltaVelocities);
    // dumpfer in case you need flexible system
//    for (auto & vel : GraphVisualizer::velocities) {
//        vel.second *= 0.977f;
//    }
    GraphVisualizer::mutex.lock();
    increase(GraphVisualizer::positions, GraphVisualizer::velocities);
    GraphVisualizer::mutex.unlock();

    return GraphVisualizer::positions;
}

void GraphVisualizer::forceMethodThreadFunction() {
    do {
        forceMethodIteration();
    } while (!stopThread.load(std::memory_order_acquire) && !GraphVisualizer::isForceMethodFinished());
}

bool GraphVisualizer::isForceMethodFinished() {
    return deltaVelocitiesSquaredSum() < eps * GraphVisualizer::graph.size();
}

const std::unordered_map<int, Vector2> & GraphVisualizer::getPositions() const {
    return GraphVisualizer::positions;
}

void GraphVisualizer::startForceMethodThread() {
    GraphVisualizer::forceMethodThread = std::thread(&GraphVisualizer::forceMethodThreadFunction, this);
    //GraphVisualizer::forceMethodThread.detach();
}

void GraphVisualizer::lock() {
    GraphVisualizer::mutex.lock();
}

void GraphVisualizer::unlock() {
    GraphVisualizer::mutex.unlock();
}

GraphVisualizer::~GraphVisualizer() {
    stopThread = true;
    if (GraphVisualizer::forceMethodThread.joinable()) {
        GraphVisualizer::forceMethodThread.join();
    }
}
