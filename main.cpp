#include <algorithm>
#include <vector>
#include <utility>
#include <iostream>
#include <fstream>
#include <string>
#include <SFML/Graphics.hpp>

#include "graphdrawing.hpp"

#include "json.hpp"
using json = nlohmann::json;

class Camera : public sf::View {
private:
    float width; //horizontal half-size
public:
    using sf::View::View;

    Camera(const sf::View &view) : sf::View(view) {
        setWidth(10);
    }
    float getWidth() {
        return width;
    }
    void setWidth(float width) {
        this->width = width;
        setSize(sf::View::getSize());
    }
    void setSize(float width, float height) {
        sf::View::setSize(2 * this->width, 2 * this->width * height / width);
    }
    void setSize(const sf::Vector2f &size) {
        sf::View::setSize(2 * this->width, 2 * this->width * size.y / size.x);
    }
};


int main()
{
    std::unordered_map<int, std::list<int>> graph;
        
    srand(time(0));

    size_t a;
    size_t b;

    json jsonGraph;
    
    std::ifstream in("small_graph.json");
    in >> jsonGraph;
    in.close();

    for (auto & point : jsonGraph["points"]) {
        graph[point["idx"]] = std::list<int>{};
    }
    
    for (auto & line : jsonGraph["lines"]) {
        auto & points = line["points"];
        std::pair<int, int> pointsPair{points[0], points[1]};
        graph[pointsPair.first].push_back(pointsPair.second);
        graph[pointsPair.second].push_back(pointsPair.first);
    }
    
    std::cout << std::endl;
    
    std::unordered_map<int, sf::Vector2<float>> positions = 
            GraphDrawing::forceMethod(graph);
    
    
    sf::CircleShape shape(20);
    shape.setOrigin(20, 20);
    shape.setFillColor(sf::Color::Red);
    sf::Vertex line[2];
    
    
    // sf::String name = "Graph";
    sf::RenderWindow window(sf::VideoMode(800, 600), "Graph");
    window.setFramerateLimit(60);
    Camera mainCamera = window.getView();
    mainCamera.setWidth(2000);
    mainCamera.setCenter(0, 0);
    window.setView(mainCamera);
    
    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::Resized) {
                // update the view to the new size of the window
                mainCamera.setSize(window.getSize().x, window.getSize().y);
                window.setView(mainCamera);
            }
        }
        
        window.clear();
        sf::Vector2f center(0, 0);
        for (auto position : positions) {
            center += position.second;
            shape.setPosition(position.second);
            window.draw(shape);
        }
        for (const auto& pair : graph) {
            line[0].position = positions[pair.first];
            for (auto index : pair.second) {
                line[1].position = positions[index];
                window.draw(line, 2, sf::Lines);
            }
        }
        center /= float(graph.size());
        mainCamera.setCenter(center);
        window.setView(mainCamera);
        window.display();
    }

    return 0;
}
