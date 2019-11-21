//
// Created by Олег Бобров on 21.11.2019.
//

#include "GraphVisualizer.h"
#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <numeric>

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

//const float dt = 0.015f;
//const float init_size = 200.f;
//const float vmax = 100.f;

//const float k = 0.1f;
//const float q2 = 5000.f;
//const float gamma_s = 1.f;
//const float l0 = 100.f;
const float eps = 1e-4f;
// const float eps = 1e-2f;

const float dt = 0.01f;
const float init_size = 200.f;
const float vmax = 50.f;

const float k = 0.1f;
const float q2 = 2000.f;
const float gamma_s = 1.f;
const float l0 = 100.f;


void GraphVisualizer::setGraph(const std::unordered_map<int, std::list<int>> & graph) {
    GraphVisualizer::graph = graph;
    GraphVisualizer::positions = GraphVisualizer::randomPosition(
            GraphVisualizer::graph,
            init_size * sqrtf(graph.size()));
    GraphVisualizer::velocities = GraphVisualizer::randomVelocity(
            GraphVisualizer::graph,
            vmax);

}

void GraphVisualizer::add(std::unordered_map<int, sf::Vector2f> &x,
                          const std::unordered_map<int, sf::Vector2f> &dx) {
    for (auto & pair : x) {
        pair.second += dx.at(pair.first) * dt;
    }
}

float GraphVisualizer::randFloat() {
    return static_cast<float>(rand()) /
           static_cast<float>(RAND_MAX);
}

std::unordered_map<int, sf::Vector2f> GraphVisualizer::randomPosition(
        const std::unordered_map<int, std::list<int>> & graph, float max) {
    std::unordered_map<int, sf::Vector2f> r;
    for (auto & pair : graph) {
        r[pair.first] = sf::Vector2f{randFloat(), randFloat()} * max;
    }
    return r;
}

std::unordered_map<int, sf::Vector2f> GraphVisualizer::randomVelocity(
        const std::unordered_map<int, std::list<int>> & graph, float max) {
    std::unordered_map<int, sf::Vector2f> r;
    for (auto & pair : graph) {
        r[pair.first] = sf::Vector2f{randFloat() - 1, randFloat() - 1} * max;
    }
    return r;
}

float GraphVisualizer::forceSum(const std::unordered_map<int, sf::Vector2f> & forces) {
    return std::accumulate(forces.begin(), forces.end(), 0.f,
                           [](float first, const std::pair<int, sf::Vector2f> & pair) {
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

float GraphVisualizer::noedge(float dl) {
    return q2 / (dl*dl);
}

bool GraphVisualizer::hasEdge(const std::unordered_map<int, std::list<int>> & graph,
                              int from, int to) {
    for (const int & point : graph.at(from)) {
        if (point == to) {
            return true;
        }
    }
    return false;
}

std::unordered_map<int, sf::Vector2f> GraphVisualizer::force(
        const std::unordered_map<int, std::list<int>> & graph,
        const std::unordered_map<int, sf::Vector2f> & positions) {

    std::unordered_map<int, sf::Vector2f> forces;
    for (auto & pair : positions) {
        forces[pair.first] = {0.f, 0.f};
    }

    for (auto & pairI : positions) {
        for (auto & pairJ : positions) {
            sf::Vector2f dv = pairI.second - pairJ.second;
            float dl = sqrtf(dv.x * dv.x + dv.y * dv.y);

            if (dl == 0) {
                continue;
            }

            float fs = hasEdge(graph, pairI.first, pairJ.first)
                       ? edge(dl) : noedge(dl);

            sf::Vector2f f = fs * dv;

            forces[pairI.first] += f;
            forces[pairJ.first] -= f;

        }
    }
    return forces;
}

std::unordered_map<int, sf::Vector2f> GraphVisualizer::ForceFrictionModel(
        const std::unordered_map<int, std::list<int>> & graph,
        std::unordered_map<int, sf::Vector2f> & positions,
        std::unordered_map<int, sf::Vector2f> & velocities) {
    std::unordered_map<int, sf::Vector2f> f = force(graph, positions);

    for (auto & pair : velocities) {
        sf::Vector2f & vec = pair.second;
        float dl = sqrtf(vec.x * vec.x + vec.y * vec.y);

        if (dl == 0) {
            continue;
        }

        f[pair.first] -= friction(dl) * pair.second / dl;
    }

    return f;
}

const std::unordered_map<int, sf::Vector2<float>> & GraphVisualizer::forceMethodIteration() {

    GraphVisualizer::fv = ForceFrictionModel(
            GraphVisualizer::graph,
            GraphVisualizer::positions,
            GraphVisualizer::velocities);

    add(GraphVisualizer::velocities, fv);
    add(GraphVisualizer::positions, GraphVisualizer::velocities);

    return GraphVisualizer::positions;
}

const std::unordered_map<int, sf::Vector2<float>> & GraphVisualizer::forceMethod() {

    GraphVisualizer::positions = GraphVisualizer::randomPosition(
            GraphVisualizer::graph,
            init_size * sqrtf(graph.size()));
    GraphVisualizer::velocities = GraphVisualizer::randomVelocity(
            GraphVisualizer::graph,
            vmax);

    // step computes sum of modules of forces.
    // so we call step while it greater than 1e-2f;

    float graphEps = eps * graph.size();
    do {
        forceMethodIteration();
    } while (forceSum(GraphVisualizer::fv) > graphEps);

    return GraphVisualizer::positions;
}
