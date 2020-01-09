#include <algorithm>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
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

static std::atomic<bool> closeCalled;
static std::mutex windowMutex;

void mainLoop(sf::RenderWindow & window,
              Camera * mainCamera) {

    //window.setFramerateLimit(60);

    window.setActive(true);

    sf::Clock clock; // starts the clock

    float time = 0.f;
    while (!closeCalled.load(std::memory_order_release)) {
        Time::deltaTime = clock.restart().asSeconds();
        Input::setFromInputBuffer();
        MethodsPool::start();
        MethodsPool::update();
        time += Time::deltaTime;
        while (time >= Time::fixedDeltaTime) {
            MethodsPool::fixedUpdate();
            time -= Time::fixedDeltaTime;
        }
        MethodsPool::onDestroy();
        windowMutex.lock();
        window.clear();
        Renderer::draw(window, mainCamera->getRenderState());
        window.display();
        windowMutex.unlock();
        Network::update();
    }

}

bool waitEvent(sf::Window & window, sf::Event & event) {
    bool result = false;
    while (!result) {
        result = window.pollEvent(event);
        std::this_thread::yield();
    }
    return result;
}

int main() {
#ifdef __linux__
    XInitThreads();
#endif

    closeCalled.store(false, std::memory_order_acquire);

    sf::RenderWindow window(sf::VideoMode(1000, 600), "Graph");

    window.setActive(false);

    GameObject * root = Prefabs::graphRoot()->gameObject->instantiate();
    Camera * mainCamera = Prefabs::camera(&window);
    mainCamera->gameObject->instantiate();

    std::thread mainLoopThread(mainLoop, std::ref(window), mainCamera);

    sf::Event event{};

    while (window.isOpen()) {

        if (waitEvent(window, event)) {
            // scary event handling
            // TODO handle events somewhere else
            switch (event.type) {
            case sf::Event::Closed:
                closeCalled.store(true, std::memory_order_acquire);
                mainLoopThread.join();
                root->destroyImmediate();
                window.close();
                return 0;
            case sf::Event::Resized:
                windowMutex.lock();
                mainCamera->onWindowResized();
                windowMutex.unlock();
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
