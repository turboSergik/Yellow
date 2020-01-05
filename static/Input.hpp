#ifndef INPUT_HPP
#define INPUT_HPP

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Event.hpp>
#include "../utility/ThreadSafeBitset.hpp"
#include <array>
#include <bitset>

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
    
public:

    static bool isKeyDown(sf::Keyboard::Key key);
    static bool isKey(sf::Keyboard::Key key);
    static bool isKeyUp(sf::Keyboard::Key key);

    static bool getMouseButtonPressed(sf::Mouse::Button button);
    static bool getMouseButtonReleased(sf::Mouse::Button button);
    static sf::Event::MouseButtonEvent getMBPressedEvent(sf::Mouse::Button button);
    static sf::Event::MouseButtonEvent getMBReleasedEvent(sf::Mouse::Button button);

    static sf::Event::MouseWheelScrollEvent getWheelScrollEvent();
    static bool getWheelScrolled();
    
    static void setFromInputBuffer();
    
};

#endif // INPUT_HPP
