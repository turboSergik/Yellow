#include <algorithm>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "static/Database.h"
#include "core/GameObject.h"
#include "core-components/Camera.h"
#include "core-components/renderers/Renderer.h"
#include "core-components/renderers/CircleRenderer.h"
#include "static/PrefabCreator.h"
#include "static/Time.h"
#include "Networking/PacketQueue.hpp"
#include "static/Input.hpp"

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

    Input & input = Input::instance();
    
    while (window.isOpen()) {
        sf::Event event{};
        input.reset();
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                delete root;
                return 0;
            case sf::Event::Resized:
                // update the view to the new size of the window
                mainCamera->onWindowResized();
                break;
            case sf::Event::KeyPressed:
                input.addKey(event.key);
                break;
            default:
                break;
            }
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
        
        PacketQueue::instance().update();
    }
    
    return 0;
}
