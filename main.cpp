#include <algorithm>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "json/json.hpp"
#include "static/Database.h"
#include "core/GameObject.h"
#include "core-components/Camera.h"
#include "core-components/renderers/Renderer.h"
#include "core-components/renderers/CircleRenderer.h"
#include "static/PrefabCreator.h"

GameObject *initSceneTree() {
    GameObject *root = new GameObject();
    sf::Vector2f center;
    for (auto & pair : Database::points) {
        pair.second->transform->setParent(root->transform);
        center += pair.second->transform->getLocalPosition();
    }
    center /= static_cast<float>(Database::points.size());
    root->transform->setPosition(-center);
    for (auto & pair : Database::lines) {
        pair.second->transform->setParent(root->transform);
    }
    for (auto & pair : Database::posts) {
        pair.second->transform->setParent(pair.second->point->transform);
    }
    return root;
}

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

    Database::applyLayer0(layer0);
    Database::applyLayer1(layer1);
    Database::generateCoordinates();

    sf::RenderWindow window(sf::VideoMode(800, 600), "Graph");
    window.setFramerateLimit(60);

    Camera *mainCamera = PrefabCreator::createCamera(&window);
    mainCamera->setWidth(2000);
//    Camera mainCamera = window.getView();
//    mainCamera.setWidth(4000);
//    mainCamera.setCenter(0, 0);
//    window.setView(mainCamera);

    sf::Clock clock; // starts the clock

    GameObject *root = initSceneTree();

//    GameObject *test = new GameObject();
//    auto cr = test->addComponent<CircleRenderer>();
//    cr->circle.setRadius(20);
//    cr->circle.setOrigin(20, 20);

//    test->transform->setLocalPosition({0, 100});

    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                delete root;
            }
            if (event.type == sf::Event::Resized) {
                // update the view to the new size of the window
                mainCamera->onWindowResized();
            }
        }

        window.clear();
        root->update();
        Renderer::draw(window, mainCamera->getRenderState());
        window.display();
    }

    return 0;
}
