#include <algorithm>
#include <vector>
#include <utility>
#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include "static/graphdrawing.h"
#include "json.hpp"
#include "Database.h"
#include "base-components/Camera.h"


int main() {
    srand(time(nullptr));
    //auto graph = readGraphFromJson("big_graph.json");
    //auto positions = GraphDrawing::forceMethod(graph);

    nlohmann::json layer0, layer1;
    std::ifstream in;
    in.open("map_layer0.json");
    in >> layer0;
    in.close();
    in.open("map_layer1.json");
    in >> layer1;
    in.close();

//    Database map(layer0["idx"]);
//    map.applyLayer0(layer0);
//    map.applyLayer1(layer1);
//    map.generateCoordinates();

    sf::RenderWindow window(sf::VideoMode(800, 600), "Graph");
    window.setFramerateLimit(60);
    Camera mainCamera = window.getView();
    mainCamera.setWidth(1000);
    mainCamera.setCenter(0, 0);
    window.setView(mainCamera);

    sf::Clock clock; // starts the clock

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
        //window.draw(map);
        sf::RectangleShape shape({1000, 1000});
        shape.setOrigin(500, 500);
        sf::Transform t1;
        t1.translate({1000, 0}).rotate(45).scale({2, 2});
        std::cout << t1.transformPoint(-500, 500).x << " " << t1.transformPoint(-500, 500).y << std::endl;
        window.draw(shape, t1);
        //        center /= float(graph.size());
//        mainCamera.setCenter(center);
        mainCamera.update(clock.restart().asSeconds());
        window.setView(mainCamera);
        window.display();
    }

    return 0;
}