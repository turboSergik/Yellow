#include <algorithm>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include "static/Database.h"
#include "core/GameObject.h"
#include "core-components/Camera.h"
#include "core-components/renderers/Renderer.h"
#include "core-components/renderers/CircleRenderer.h"
#include "static/Prefabs.h"
#include "static/Time.h"
#include "network/PacketQueue.hpp"
#include "static/Input.hpp"
#include "network/Network.hpp"

void mainLoop(sf::RenderWindow & window, 
              Camera * mainCamera) {
    
    window.setFramerateLimit(60);
    
//    GameObject * root = Prefabs::graphRoot()->gameObject->instantiate();
//    Camera *mainCamera = Prefabs::camera(&window);
//    mainCamera->gameObject->instantiate();
    
    window.setActive(true);
    
    sf::Clock clock; // starts the clock
    
    while (window.isOpen()) {
        MethodsPool::update();
        Input::reset();
        window.clear();        
        Renderer::draw(window, mainCamera->getRenderState());
        window.display();
        Network::update();
        Time::deltaTime = clock.restart().asSeconds();
    }
    
}

int main() {
    srand(time(nullptr));

    sf::RenderWindow window(sf::VideoMode(800, 600), "Graph");

    GameObject * root = Prefabs::graphRoot()->gameObject->instantiate();
    Camera * mainCamera = Prefabs::camera(&window);
    mainCamera->gameObject->instantiate();
    
    window.setActive(false);
        
    std::thread mainLoopThread(mainLoop, std::ref(window), mainCamera);
    
    window.setKeyRepeatEnabled(false);

    while (window.isOpen()) {
        sf::Event event{};
        
        if (window.waitEvent(event)) {
            // scary event handling
            // TODO handle events somewhere else
            switch (event.type) {
            case sf::Event::Closed:
                root->destroyImmediate();
                window.close();
                mainLoopThread.join();
                return 0;
                // break;
            case sf::Event::Resized:
                // update the view to the new size of the window
                mainCamera->onWindowResized();
                break;
            case sf::Event::KeyPressed:
                Input::addKeyPressed(event.key);
                break;
            case sf::Event::KeyReleased:
                Input::addKeyReleased(event.key);
                break;
            case sf::Event::MouseButtonPressed:
                Input::addMouseButtonPressed(event.mouseButton);
                break;
            case sf::Event::MouseButtonReleased:
                Input::addMouseButtonReleased(event.mouseButton);
                break;
            case sf::Event::MouseWheelScrolled:
                if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
                    Input::addWheelScroll(event.mouseWheelScroll);
                    // std::cout << event.mouseWheelScroll.delta << std::endl;                    
                }
                break;
            default:
                break;
            }

        }
    }
    
    return 0;
}
