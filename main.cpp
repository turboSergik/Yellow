#include <algorithm>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "static/Database.h"
#include "core/GameObject.h"
#include "core-components/Camera.h"
#include "core-components/renderers/Renderer.h"
#include "core-components/renderers/CircleRenderer.h"
#include "static/Prefabs.h"
#include "static/Time.h"
#include "Networking/PacketQueue.hpp"
#include "static/Input.hpp"

int main() {
    srand(time(nullptr));

    sf::RenderWindow window(sf::VideoMode(800, 600), "Graph");
    window.setFramerateLimit(60);

    GameObject * root = Prefabs::graphRoot()->gameObject->instantiate();
    Camera *mainCamera = Prefabs::camera(&window);
    mainCamera->gameObject->instantiate();

    sf::Clock clock; // starts the clock

    window.setKeyRepeatEnabled(false);

    while (window.isOpen()) {
        sf::Event event{};
        Input::reset();
        while (window.pollEvent(event)) {
            // scary event handling
            // TODO handle events somewhere else
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
                }
                break;
            default:
                break;
            }

        }

        Time::deltaTime = clock.restart().asSeconds();
        window.clear();
        MethodsPool::update();
        Renderer::draw(window, mainCamera->getRenderState());
        window.display();

        PacketQueue::instance().update();
    }

    return 0;
}
