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

int main() {
    srand(time(nullptr));

    sf::RenderWindow window(sf::VideoMode(800, 600), "Graph");
    window.setFramerateLimit(60);

    GameObject *root = PrefabCreator::createRoot()->gameObject;
    //TODO: crate scene class which update will be called and that allow us have multiple root objects
    Camera *mainCamera = PrefabCreator::createCamera(&window);
    mainCamera->transform->setParent(root->transform);
    //mainCamera->setWidth(2000);

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
