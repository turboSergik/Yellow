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
    std::bitset<sf::Keyboard::KeyCount> pressedKeys;
    std::bitset<sf::Keyboard::KeyCount> releasedKeys;
    
    std::bitset<sf::Mouse::ButtonCount> pressedMouseButtons;
    std::bitset<sf::Mouse::ButtonCount> releasedMouseButtons;
    
    std::array<sf::Event::MouseButtonEvent, 
               sf::Mouse::ButtonCount> pressedMouseEvents;
    
    std::array<sf::Event::MouseButtonEvent,
               sf::Mouse::ButtonCount> releasedMouseEvents;
    
    sf::Event::MouseWheelScrollEvent wheelScrollEvent;
    bool wheelScrolled;
    
    
    Input() = default;
    Input(const Input &) = delete;
    Input(Input &&) = delete;
    Input & operator=(const Input &) = delete;
    Input & operator=(Input &&) = delete;
public:
    static Input & instance();
    
    void addKeyPressed(sf::Event::KeyEvent keyEvent);
    void addKeyReleased(sf::Event::KeyEvent keyEvent);    
    bool getKeyPressed(sf::Keyboard::Key key);
    bool getKeyReleased(sf::Keyboard::Key key);
    
    void addMouseButtonPressed(sf::Event::MouseButtonEvent buttonEvent);
    void addMouseButtonReleased(sf::Event::MouseButtonEvent buttonEvent);
    bool getMouseButtonPressed(sf::Mouse::Button button);
    bool getMouseButtonReleased(sf::Mouse::Button button);
    sf::Event::MouseButtonEvent getMBPressedEvent(sf::Mouse::Button button);
    sf::Event::MouseButtonEvent getMBReleasedEvent(sf::Mouse::Button button);
    
    void addWheelScroll(sf::Event::MouseWheelScrollEvent mouseScrollEvent);
    sf::Event::MouseWheelScrollEvent getWheelScrollEvent();
    bool getWheelScrolled();
    
    void reset();
    ~Input() = default;
};

#endif // INPUT_HPP
