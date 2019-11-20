#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <numeric>
#include "graphdrawing.h"

const float dt = 0.01f;
const float init_size = 200.f;
const float vmax = 100.f;

const float k = 0.1f;
const float q2 = 5000.f;
const float gamma_s = 1.f;
const float l0 = 100.f;
// const float eps = 1e-4f;
const float eps = 1e-2f;

void add(std::unordered_map<int, sf::Vector2f> &x, 
         const std::unordered_map<int, sf::Vector2f> &dx) {
    for (auto & pair : x) {
        pair.second += dx.at(pair.first) * dt;
    }
}

float randFloat() {
    return static_cast<float>(rand()) / 
           static_cast<float>(RAND_MAX);
}

std::unordered_map<int, sf::Vector2f> 
randomPosition(const std::unordered_map<int, std::list<int>> & graph, float max) {
    std::unordered_map<int, sf::Vector2f> r;
    for (auto & pair : graph) {
        r[pair.first] = sf::Vector2f{randFloat(), randFloat()} * max;
    }
    return r;
}

std::unordered_map<int, sf::Vector2f> 
randomVelocity(const std::unordered_map<int, std::list<int>> & graph, float max) {
    std::unordered_map<int, sf::Vector2f> r;
    for (auto & pair : graph) {
        r[pair.first] = sf::Vector2f{randFloat() - 1, randFloat() - 1} * max;
    }
    return r;
}

float forceSum(const std::unordered_map<int, sf::Vector2f> & forces) {
    return std::accumulate(forces.begin(), forces.end(), 0.f, 
                           [](float first, const std::pair<int, sf::Vector2f> & pair) {
        return first + sqrtf(pair.second.x * pair.second.x +
                             pair.second.y * pair.second.y);
    });
}


float friction(float v){
    return gamma_s * v * v;
}

float edge(float dl) {
    return q2 / (dl*dl) - k * logf(dl / l0);
}

float noedge(float dl) {
    return q2 / (dl*dl);
}

bool hasEdge(const std::unordered_map<int, std::list<int>> & graph, 
             int from, int to) {
    for (const int & point : graph.at(from)) {
        if (point == to) {
            return true;
        }
    }
    return false;
}

std::unordered_map<int, sf::Vector2f> force(const std::unordered_map<int, std::list<int>> & graph, 
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
            
            sf::Vector2f f = fs * dv / dl;
            
            forces[pairI.first] += f;
            forces[pairJ.first] -= f;
            
        }
    }
    return forces;
}

std::unordered_map<int, sf::Vector2f> ForceFrictionModel(const std::unordered_map<int, std::list<int>> & graph, 
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

float forceMethodIteraton(const std::unordered_map<int, std::list<int>> & graph,
           std::unordered_map<int, sf::Vector2f> & positions,
           std::unordered_map<int, sf::Vector2f> & velocities) {

    std::unordered_map<int, sf::Vector2f> fv = 
            ForceFrictionModel(graph, positions, velocities);
    
    add(velocities, fv);
    add(positions, velocities);
    
    return forceSum(fv);
}

std::unordered_map<int, sf::Vector2<float>> 
GraphDrawing::forceMethod(const std::unordered_map<int, std::list<int>> & graph) {
    
    std::unordered_map<int, sf::Vector2f> positions = 
            randomPosition(graph, init_size * sqrtf(graph.size()));
    std::unordered_map<int, sf::Vector2f> velocities = 
            randomVelocity(graph, vmax);
    
    // step computes sum of modules of forces.
    // so we call step while it greater than 1e-2f;
    
    float graphEps = eps * graph.size();
    while (forceMethodIteraton(graph, positions, velocities) > graphEps);
    
    return positions;
}

//typedef sf::Vector2<float> vector2;

//float computeAngle(int);
//float computeHooke(vector2, vector2);
//float computeLogHooke(vector2, vector2);

//std::unordered_map<int, vector2>
//computeIteration(const std::unordered_map<int, vector2> & position, 
//                 const std::unordered_map<int, vector2> & velocity,
//                 const std::unordered_map<int, std::list<int>> & graph);

//float computeKineticEnergy(const std::unordered_map<int, vector2> &velocity);

//// generate random number from 0 to 1
//float randomFloat();

//float vector2Length(const vector2 & v);
//vector2 normalized(const vector2 & v);
//float distance(const vector2 & first, const vector2 & second);

//std::unordered_map<int, vector2>
//GraphDrawing::forceMethod(const std::unordered_map<int, std::list<int>> & graph)
//{
//    std::unordered_map<int, vector2> position;
//    std::unordered_map<int, vector2> velocity;
    
//    for (auto & pair : graph) {
//        float randomVelocityX = randomFloat() * MAX_VELOCITY;
//        float randomVelocityY = randomFloat() * MAX_VELOCITY;
//        velocity.insert({pair.first, vector2(randomVelocityX, randomVelocityY)});
//    }

//    for (auto & pair : graph) { 
//        float currentPositionX = randomFloat() * L_KOEFFICIENT * graph.size();
//        float currentPositionY = randomFloat()* L_KOEFFICIENT * graph.size();
        
//        for (const int & item : pair.second) {
//            if (position.count(item) > 0 &&
//                    distance(vector2(currentPositionX, currentPositionY), 
//                                            position[item]) > L_KOEFFICIENT){
                
//                currentPositionX = randomFloat() * L_KOEFFICIENT * graph.size();
//                currentPositionY = randomFloat() * L_KOEFFICIENT * graph.size();
//            }
//        }
//        position.insert({pair.first, vector2(currentPositionX, currentPositionY)});
//    }


//    do{
//        std::unordered_map<int, vector2> totalForce = computeIteration(position, velocity, graph);

//        std::unordered_map<int, vector2> newPosition = position;
//        std::unordered_map<int, vector2> newVelocity = velocity;

//        for(auto & force : totalForce) {
//            newVelocity[force.first] += force.second * Dt;
//            newPosition[force.first] += velocity[force.first] * Dt;
//        }
//        velocity = newVelocity;
//        position = newPosition;
        
//    } while(computeKineticEnergy(velocity) > MIN_KINETIC);

//    return position;
//}

//float computeAngle(int nodesNu)
//{
//    return acosf(-1) * 2 / nodesNu;
//}

//float computeHooke(vector2 firstVector, vector2 secondVector)
//{
//    return K_KOEFFICIENT * (vector2Length(firstVector - secondVector) - L_KOEFFICIENT);
//}

//float computeLogHooke(vector2 firstVector, vector2 secondVector)
//{
//    return K_KOEFFICIENT * ((logf(vector2Length(firstVector - secondVector) / L_KOEFFICIENT)));
//}

//std::unordered_map<int, vector2>
//computeIteration(const std::unordered_map<int, vector2> & position, 
//                 const std::unordered_map<int, vector2> & velocity,
//                 const std::unordered_map<int, std::list<int>> & graph)
//{
//    std::unordered_map<int, vector2> 
//            totalForce, 
//            coulombForce,
//            hookeForce, 
//            fricForce;

//    for (auto & pair : graph) {
//        totalForce[pair.first] = vector2(0, 0);
//        coulombForce[pair.first] = vector2(0, 0);
//        hookeForce[pair.first] = vector2(0, 0);
//        fricForce[pair.first] = vector2(0, 0);
//    }


//    for (auto & positionI : position) {
//        for (auto & positionJ : position) {
//            if(positionI.first != positionJ.first) {
//                coulombForce[positionI.first] += 
//                        (normalized(positionI.second - positionJ.second)) *
//                        (powf(Q_KOEFFICIENT, 2) / 
//                         vector2Length(positionI.second - positionJ.second));
//            }
//        }
//    }
    
//    for (auto & pair : graph) {
//        for (auto & item : pair.second) {
//            hookeForce[pair.first] += ((position.at(pair.first) - position.at(item))) *
//                    computeLogHooke(position.at(pair.first), position.at(item));
//        }
//    }

//    for (auto & pair : velocity) {
//        fricForce[pair.first] = normalized(pair.second) * -FRICTION;
//    }

//    for (auto & pair : graph) {
//        const int & i = pair.first;
//        totalForce[i] = (coulombForce[i] + hookeForce[i] + fricForce[i]) / MASS;
//    }

//    return totalForce;
//}

//float computeKineticEnergy(const std::unordered_map<int, vector2> & velocity)
//{
//    float kineticEnergy = 0;
//    for (auto & pair : velocity) {
//        float length = vector2Length(pair.second);
//        kineticEnergy += length * length;
//    }
//    return kineticEnergy / 2;
//}

//float randomFloat() {
//    return static_cast<float>(rand()) / RAND_MAX;
//}

//float vector2Length(const vector2 & v) {
//    return sqrtf(v.x * v.x + v.y * v.y);
//}

//float distance(const vector2 & first, const vector2 & second) {
//    vector2 diff = first - second;
//    return sqrtf(diff.x * diff.x + diff.y * diff.y);
//}

//vector2 normalized(const vector2 & v) {
//    return v / vector2Length(v);
//}
