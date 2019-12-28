#include "Vector2.hpp"
#include <cmath>

lng::Vector2::Vector2(const sf::Vector2f & original) {
    this->x = original.x;
    this->y = original.y;
}

lng::Vector2 lng::Vector2::operator-() {
    return lng::Vector2(-this->x, -this->y);
}

lng::Vector2 lng::Vector2::operator+() {
    return *this;
}

lng::Vector2 lng::Vector2::operator+(const sf::Vector2f & operand) {
    return lng::Vector2(this->x + operand.x, this->y + operand.y);
}

lng::Vector2 & lng::Vector2::operator+=(const sf::Vector2f & operand) {
    this->x += operand.x; 
    this->y += operand.y;
    return *this;
}

lng::Vector2 lng::Vector2::operator-(const sf::Vector2f & operand) {
    return lng::Vector2(this->x + operand.x, this->y + operand.y);
}

lng::Vector2 & lng::Vector2::operator-=(const sf::Vector2f & operand) {
    this->x -= operand.x; 
    this->y -= operand.y;
    return *this;
}

lng::Vector2 lng::Vector2::operator*(const sf::Vector2f & operand) {
    return lng::Vector2(this->x + operand.x, this->y + operand.y);
}

lng::Vector2 & lng::Vector2::operator*=(const sf::Vector2f & operand) {
    this->x *= operand.x; 
    this->y *= operand.y;
    return *this;
}

lng::Vector2 lng::Vector2::operator/(const sf::Vector2f & operand) {
    return lng::Vector2(this->x + operand.x, this->y + operand.y);
}

lng::Vector2 & lng::Vector2::operator/=(const sf::Vector2f & operand) {
    this->x /= operand.x; 
    this->y /= operand.y;
    return *this;
}

void lng::Vector2::normalize() {
    float magn = this->magnitude();
    if (magn != 0.f) {
        *this /= magn;
    }
}

lng::Vector2 lng::Vector2::normalized() {
    lng::Vector2 result = *this;
    result.normalize();
    return result;
}

float lng::Vector2::sqrMagnitude() {
    return x * x + y * y;
}

float lng::Vector2::magnitude() {
    return hypotf(this->x, this->y);
}

float lng::Vector2::dot(const sf::Vector2f & operand) {
    return this->x * operand.x + this->y * operand.y;
}

float lng::Vector2::cross(const sf::Vector2f & operand) {
    return this->x * operand.y - operand.x * this->y;
}
