#ifndef INPUT_HPP
#define INPUT_HPP

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>
#include <bitset>

class Input
{
    // sf::Keyboard::Return is last in int
    // enum, so it it + 1 is equal to numbers
    // of keys in enum
    std::bitset<sf::Keyboard::Return + 1> pressedKeys;
    Input() = default;
    Input(const Input &) = delete;
    Input(Input &&) = delete;
    Input & operator=(const Input &) = delete;
    Input & operator=(Input &&) = delete;
public:
    static Input & instance();
    void addKey(sf::Event::KeyEvent keyEvent);
    bool getKeyPressed(sf::Keyboard::Key key);
    void reset();
    ~Input() = default;
};

#endif // INPUT_HPP
