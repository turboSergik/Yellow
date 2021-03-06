#ifndef INPUT_HPP
#define INPUT_HPP

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Event.hpp>
#include <bitset>
#include <array>

class Input
{
    // sf::Keyboard::Return is last in int
    // enum, so it it + 1 is equal to numbers
    // of keys in enum
    static std::bitset<sf::Keyboard::KeyCount> pressedKeys;
    static std::bitset<sf::Keyboard::KeyCount> heldKeys;
    static std::bitset<sf::Keyboard::KeyCount> releasedKeys;

    static std::bitset<sf::Mouse::ButtonCount> pressedMouseButtons;
    static std::bitset<sf::Mouse::ButtonCount> releasedMouseButtons;

    static std::array<sf::Event::MouseButtonEvent,
               sf::Mouse::ButtonCount> pressedMouseEvents;

    static std::array<sf::Event::MouseButtonEvent,
               sf::Mouse::ButtonCount> releasedMouseEvents;

    static sf::Event::MouseWheelScrollEvent wheelScrollEvent;
    static bool wheelScrolled;

//    Input() = default;
//    Input(const Input &) = delete;
//    Input(Input &&) = delete;
//    Input & operator=(const Input &) = delete;
//    Input & operator=(Input &&) = delete;
public:
//    static Input & instance();
//    ~Input() = default;

    static void addKeyPressed(sf::Event::KeyEvent keyEvent);
    static void addKeyReleased(sf::Event::KeyEvent keyEvent);

    static bool isKeyDown(sf::Keyboard::Key key);
    static bool isKey(sf::Keyboard::Key key);
    static bool isKeyUp(sf::Keyboard::Key key);

    static void addMouseButtonPressed(sf::Event::MouseButtonEvent buttonEvent);
    static void addMouseButtonReleased(sf::Event::MouseButtonEvent buttonEvent);
    static bool getMouseButtonPressed(sf::Mouse::Button button);
    static bool getMouseButtonReleased(sf::Mouse::Button button);
    static sf::Event::MouseButtonEvent getMBPressedEvent(sf::Mouse::Button button);
    static sf::Event::MouseButtonEvent getMBReleasedEvent(sf::Mouse::Button button);

    static void addWheelScroll(sf::Event::MouseWheelScrollEvent mouseScrollEvent);
    static sf::Event::MouseWheelScrollEvent getWheelScrollEvent();
    static bool getWheelScrolled();

    static void reset();
};

#endif // INPUT_HPP
