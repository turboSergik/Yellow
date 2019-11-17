#include <algorithm>
#include <fstream>
#include <SFML/Graphics.hpp>
#include "json/json.hpp"
#include "static/Database.h"
#include "core-components/Camera.h"
#include "core-components/renderers/Renderer.h"


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
        //center /= float(graph.size());
        //mainCamera.setCenter(center);
        mainCamera.update(clock.restart().asSeconds());
        window.setView(mainCamera);
        Renderer::draw(window, sf::RenderStates::Default);
        window.display();
    }

    return 0;
}
