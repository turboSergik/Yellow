#include <algorithm>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include <mutex>
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

std::mutex windowMutex;

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
        windowMutex.lock();
        window.clear();        
        Renderer::draw(window, mainCamera->getRenderState());
        window.display();
        windowMutex.unlock();
        Network::update();
        Time::deltaTime = clock.restart().asSeconds();
    }
    
}

bool safeWaitEvent(sf::RenderWindow & window, sf::Event & event) {
    bool result = false;
    while (!result) {
        // may be not best desidion
        windowMutex.lock();
        result = window.pollEvent(event);
        windowMutex.unlock();
        std::this_thread::yield();
    }
    return result;
}

int main() {
    srand(time(nullptr));

    sf::RenderWindow window(sf::VideoMode(800, 600), "Graph");

    GameObject * root = Prefabs::graphRoot()->gameObject->instantiate();
    Camera * mainCamera = Prefabs::camera(&window);
    mainCamera->gameObject->instantiate();
    
    window.setActive(false);
        
    std::thread mainLoopThread(mainLoop, std::ref(window), mainCamera);
    
    // window.setKeyRepeatEnabled(false);
    sf::Event event{};
    
    while (window.isOpen()) {
        
        if (safeWaitEvent(window, event)) {
            // scary event handling
            // TODO handle events somewhere else
            switch (event.type) {
            case sf::Event::Closed:
                root->destroyImmediate();
                windowMutex.lock();
                window.close();
                windowMutex.unlock();
                mainLoopThread.join();
                return 0;
                // break;
            case sf::Event::Resized:
                // update the view to the new size of the window
                windowMutex.lock();
                mainCamera->onWindowResized();
                windowMutex.unlock();
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
