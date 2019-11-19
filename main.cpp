#include <algorithm>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "json/json.hpp"
#include "static/Database.h"
#include "core/GameObject.h"
#include "core-components/Camera.h"
#include "core-components/renderers/Renderer.h"
#include "core-components/renderers/CircleRenderer.h"
#include "static/PrefabCreator.h"
#include "static/Time.h"

//GameObject *initSceneTree() {
//    GameObject *root = new GameObject();
//    sf::Vector2f center;
//    for (auto & pair : Database::points) {
//        pair.second->transform->setParent(root->transform);
//        center += pair.second->transform->getLocalPosition();
//    }
//    center /= static_cast<float>(Database::points.size());
//    root->transform->setPosition(-center);
//    for (auto & pair : Database::lines) {
//        pair.second->transform->setParent(root->transform);
//    }
//    for (auto & pair : Database::posts) {
//        pair.second->transform->setParent(pair.second->point->transform);
//    }
//    return root;
//}

int main() {
    srand(time(nullptr));
    //auto graph = readGraphFromJson("big_graph.json");
    //auto positions = GraphDrawing::forceMethod(graph);




    sf::RenderWindow window(sf::VideoMode(800, 600), "Graph");
    window.setFramerateLimit(60);

    GameObject *root = PrefabCreator::createRoot()->gameObject;
    Camera *mainCamera = PrefabCreator::createCamera(&window);
    mainCamera->transform->setParent(root->transform);
    //mainCamera->setWidth(2000);

//    GameObject *test = new GameObject();
//    auto cr = test->addComponent<CircleRenderer>();
//    cr->circle.setRadius(20);
//    cr->circle.setOrigin(20, 20);
//    test->transform->setLocalPosition({0, 100});

    sf::Clock clock; // starts the clock

    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                delete root;
                return 0;
            }
            if (event.type == sf::Event::Resized) {
                // update the view to the new size of the window
                mainCamera->onWindowResized();
            }
        }
        Time::deltaTime = clock.restart().asSeconds();
        window.clear();
        root->update();
        Renderer::draw(window, mainCamera->getRenderState());
        window.display();
    }

    return 0;
}
