#include <SFML/System/Vector2.hpp>
#include <cmath>
#include "graphdrawing.hpp"


typedef sf::Vector2<float> vector2;

float computeAngle(int);
float computeHooke(vector2, vector2);
float computeLogHooke(vector2, vector2);

std::unordered_map<int, vector2>
computeIteration(const std::unordered_map<int, vector2> & position, 
                 const std::unordered_map<int, vector2> & velocity,
                 const std::unordered_map<int, std::list<int>> & graph);

float computeKineticEnergy(const std::unordered_map<int, vector2> &velocity);

// generate random number from 0 to 1
float randomFloat();

float vector2Length(const vector2 & v);
vector2 normalized(const vector2 & v);
float distance(const vector2 & first, const vector2 & second);

std::unordered_map<int, vector2>
GraphDrawing::forceMethod(const std::unordered_map<int, std::list<int>> & graph)
{
    std::unordered_map<int, vector2> position;
    std::unordered_map<int, vector2> velocity;
    
    for (auto & pair : graph) {
        float randomVelocityX = randomFloat() * MAX_VELOCITY;
        float randomVelocityY = randomFloat() * MAX_VELOCITY;
        velocity.insert({pair.first, vector2(randomVelocityX, randomVelocityY)});
    }

    for (auto & pair : graph) { 
        float currentPositionX = randomFloat() * L_KOEFFICIENT * graph.size();
        float currentPositionY = randomFloat()* L_KOEFFICIENT * graph.size();
        
        for (const int & item : pair.second) {
            if (position.count(item) > 0 &&
                    distance(vector2(currentPositionX, currentPositionY), 
                                            position[item]) > L_KOEFFICIENT){
                
                currentPositionX = randomFloat() * L_KOEFFICIENT * graph.size();
                currentPositionY = randomFloat() * L_KOEFFICIENT * graph.size();
            }
        }
        position.insert({pair.first, vector2(currentPositionX, currentPositionY)});
    }


    do{
        std::unordered_map<int, vector2> totalForce = computeIteration(position, velocity, graph);

        std::unordered_map<int, vector2> newPosition = position;
        std::unordered_map<int, vector2> newVelocity = velocity;

        for(auto & force : totalForce) {
            newVelocity[force.first] += force.second * Dt;
            newPosition[force.first] += velocity[force.first] * Dt;
        }
        velocity = newVelocity;
        position = newPosition;
        
    } while(computeKineticEnergy(velocity) > MIN_KINETIC);

    return position;
}

float computeAngle(int nodesNu)
{
    return acosf(-1) * 2 / nodesNu;
}

float computeHooke(vector2 firstVector, vector2 secondVector)
{
    return K_KOEFFICIENT * (vector2Length(firstVector - secondVector) - L_KOEFFICIENT);
}

float computeLogHooke(vector2 firstVector, vector2 secondVector)
{
    return K_KOEFFICIENT * ((logf(vector2Length(firstVector - secondVector) / L_KOEFFICIENT)));
}

std::unordered_map<int, vector2>
computeIteration(const std::unordered_map<int, vector2> & position, 
                 const std::unordered_map<int, vector2> & velocity,
                 const std::unordered_map<int, std::list<int>> & graph)
{
    std::unordered_map<int, vector2> 
            totalForce, 
            coulombForce,
            hookeForce, 
            fricForce;

    for (auto & pair : graph) {
        totalForce[pair.first] = vector2(0, 0);
        coulombForce[pair.first] = vector2(0, 0);
        hookeForce[pair.first] = vector2(0, 0);
        fricForce[pair.first] = vector2(0, 0);
    }


    for (auto & positionI : position) {
        for (auto & positionJ : position) {
            if(positionI.first != positionJ.first) {
                coulombForce[positionI.first] += 
                        (normalized(positionI.second - positionJ.second)) *
                        (powf(Q_KOEFFICIENT, 2) / 
                         vector2Length(positionI.second - positionJ.second));
            }
        }
    }
    
    for (auto & pair : graph) {
        for (auto & item : pair.second) {
            hookeForce[pair.first] += ((position.at(pair.first) - position.at(item))) *
                    computeLogHooke(position.at(pair.first), position.at(item));
        }
    }

    for (auto & pair : velocity) {
        fricForce[pair.first] = normalized(pair.second) * -FRICTION;
    }

    for (auto & pair : graph) {
        const int & i = pair.first;
        totalForce[i] = (coulombForce[i] + hookeForce[i] + fricForce[i]) / MASS;
    }

    return totalForce;
}

float computeKineticEnergy(const std::unordered_map<int, vector2> & velocity)
{
    float kineticEnergy = 0;
    for (auto & pair : velocity) {
        float length = vector2Length(pair.second);
        kineticEnergy += length * length;
    }
    return kineticEnergy / 2;
}

float randomFloat() {
    return static_cast<float>(rand()) / RAND_MAX;
}

float vector2Length(const vector2 & v) {
    return sqrtf(v.x * v.x + v.y * v.y);
}

float distance(const vector2 & first, const vector2 & second) {
    vector2 diff = first - second;
    return sqrtf(diff.x * diff.x + diff.y * diff.y);
}

vector2 normalized(const vector2 & v) {
    return v / vector2Length(v);
}
