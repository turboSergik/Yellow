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
#include "static/InputBuffer.hpp"
#include "network/Network.hpp"
#ifdef __linux__
// #include <X11/Xlib.h>
// extern include function which implemented in Xlib
// not included becouse of conflicting declaration Time
extern "C" int XInitThreads();

#endif


void mainLoop(sf::RenderWindow & window, 
              Camera * mainCamera) {
    
    window.setFramerateLimit(60);
    
    window.setActive(true);
    
    sf::Clock clock; // starts the clock
    
    sf::Clock fixedUpdateClock;
    
    while (window.isOpen()) {
        Input::setFromInputBuffer();
        MethodsPool::start();
        MethodsPool::update();
        // now without timer for fixedUpdate
        MethodsPool::fixedUpdate();
        MethodsPool::onDestroy();
        window.clear();        
        Renderer::draw(window, mainCamera->getRenderState());
        window.display();
        Network::update();
        Time::deltaTime = clock.restart().asSeconds();
    }
    
}

bool safeWaitEvent(sf::RenderWindow & window, sf::Event & event) {
    bool result = false;
    while (!result) {
        result = window.pollEvent(event);
        std::this_thread::yield();
    }
    return result;
}

int main() {

    srand(time(nullptr));
    
#ifdef __linux__
    XInitThreads();
#endif
    
    sf::RenderWindow window(sf::VideoMode(800, 600), "Graph");

    GameObject * root = Prefabs::graphRoot()->gameObject->instantiate();
    Camera * mainCamera = Prefabs::camera(&window);
    mainCamera->gameObject->instantiate();
    
    window.setActive(false);
        
    std::thread mainLoopThread(mainLoop, std::ref(window), mainCamera);
    
    sf::Event event{};
    
    while (window.isOpen()) {
        
        if (safeWaitEvent(window, event)) {
            // scary event handling
            // TODO handle events somewhere else
            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                mainLoopThread.join();
                root->destroyImmediate();
                return 0;
            case sf::Event::Resized:
                mainCamera->onWindowResized();
                break;
            case sf::Event::KeyPressed:
                InputBuffer::addKeyPressed(event.key);
                break;
            case sf::Event::KeyReleased:
                InputBuffer::addKeyReleased(event.key);
                break;
            case sf::Event::MouseButtonPressed:
                InputBuffer::addMouseButtonPressed(event.mouseButton);
                break;
            case sf::Event::MouseButtonReleased:
                InputBuffer::addMouseButtonReleased(event.mouseButton);
                break;
            case sf::Event::MouseWheelScrolled:
                if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
                    InputBuffer::addWheelScroll(event.mouseWheelScroll);
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
