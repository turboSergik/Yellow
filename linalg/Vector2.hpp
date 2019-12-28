#ifndef VECTOR2_HPP
#define VECTOR2_HPP

#include <SFML/System/Vector2.hpp>
#include <type_traits>

namespace lng {

class Vector2 : public sf::Vector2f
{
    
public:
    
    Vector2() = default;
    
    template <typename T, 
    typename = typename std::enable_if<std::is_arithmetic<T>::value>::type>
    explicit Vector2(T);
    
    template <typename T1, typename T2>
    Vector2(T1, T2);
    
    Vector2(const Vector2 &) = default;
    Vector2(const sf::Vector2f &);
    
    Vector2 operator-();
    Vector2 operator+();
    
    Vector2 operator+(const sf::Vector2f &);
    Vector2 & operator+=(const sf::Vector2f &);
    Vector2 operator-(const sf::Vector2f &);
    Vector2 & operator-=(const sf::Vector2f &);
    Vector2 operator*(const sf::Vector2f &);
    Vector2 & operator*=(const sf::Vector2f &);
    Vector2 operator/(const sf::Vector2f &);
    Vector2 & operator/=(const sf::Vector2f &);
    
    
    template <typename T, 
    typename = typename std::enable_if<std::is_arithmetic<T>::value>::type>
    Vector2 & operator+=(T);
    
    template <typename T, 
    typename = typename std::enable_if<std::is_arithmetic<T>::value>::type>
    Vector2 & operator-=(T);
    
    template <typename T, 
    typename = typename std::enable_if<std::is_arithmetic<T>::value>::type>
    Vector2 & operator*=(T);
    
    template <typename T, 
    typename = typename std::enable_if<std::is_arithmetic<T>::value>::type>
    Vector2 & operator/=(T);
    
    
    void normalize();
    Vector2 normalized();
    float sqrMagnitude();
    float magnitude();
    static float dot(const sf::Vector2f &, const sf::Vector2f &);
    static float cross(const sf::Vector2f &, const sf::Vector2f &);
    
};

template <typename T, typename>
Vector2::Vector2(T initVal) {
    this->x = static_cast<float>(initVal);
    this->y = static_cast<float>(initVal);
}

template <typename T1, typename T2>
Vector2::Vector2(T1 x, T2 y) {    
    this->x = static_cast<float>(x);
    this->y = static_cast<float>(y);
}

template <typename T, 
typename = typename std::enable_if<std::is_arithmetic<T>::value>::type>
Vector2 operator+(T value, const Vector2 & vector) {
    return Vector2(vector.x + static_cast<float>(value), 
                   vector.y + static_cast<float>(value));
}

template <typename T, 
typename = typename std::enable_if<std::is_arithmetic<T>::value>::type>
Vector2 operator+(const Vector2 & vector, T value) {
    return Vector2(vector.x + static_cast<float>(value), 
                   vector.y + static_cast<float>(value));
}

template <typename T, 
typename = typename std::enable_if<std::is_arithmetic<T>::value>::type>
Vector2 operator-(T value, const Vector2 & vector) {
    return Vector2(static_cast<float>(value) - vector.x, 
                   static_cast<float>(value) - vector.y);
}

template <typename T, 
typename = typename std::enable_if<std::is_arithmetic<T>::value>::type>
Vector2 operator-(const Vector2 & vector, T value) {
    return Vector2(vector.x - static_cast<float>(value), 
                   vector.y - static_cast<float>(value));
}

template <typename T, 
typename = typename std::enable_if<std::is_arithmetic<T>::value>::type>
Vector2 operator*(T value, const Vector2 & vector) {
    return Vector2(vector.x * static_cast<float>(value), 
                   vector.y * static_cast<float>(value));
}

template <typename T, 
typename = typename std::enable_if<std::is_arithmetic<T>::value>::type>
Vector2 operator*(const Vector2 & vector, T value) {
    return Vector2(vector.x * static_cast<float>(value), 
                   vector.y * static_cast<float>(value));}

template <typename T, 
typename = typename std::enable_if<std::is_arithmetic<T>::value>::type>
Vector2 operator/(T value, const Vector2 & vector) {
    return Vector2(vector.x / static_cast<float>(value), 
                   vector.y / static_cast<float>(value));
}

template <typename T, 
typename = typename std::enable_if<std::is_arithmetic<T>::value>::type>
Vector2 operator/(const Vector2 & vector, T value) {
    return Vector2(static_cast<float>(value) / vector.x, 
                   static_cast<float>(value) / vector.y);
}

template <typename T, typename>
Vector2 & Vector2::operator+=(T value) {
    this->x += value;
    this->y += value;
    return *this;
}

template <typename T, typename>
Vector2 & Vector2::operator-=(T value) {
    this->x -= value;
    this->y -= value;
    return *this;
}

template <typename T, typename>
Vector2 & Vector2::operator*=(T value) {
    this->x *= value;
    this->y *= value;
    return *this;
}

template <typename T, typename>
Vector2 & Vector2::operator/=(T value) {
    this->x /= value;
    this->y /= value;
    return *this;
}


}



#endif // VECTOR2_HPP
