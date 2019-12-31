#include "Vector2.hpp"
#include <cmath>

Vector2 Vector2::ZERO = {0, 0};
Vector2 Vector2::ONE = {1, 1};
Vector2 Vector2::RIGHT = {1, 0};
Vector2 Vector2::LEFT = {-1, 0};
Vector2 Vector2::UP = {0, 1};
Vector2 Vector2::DOWN = {0, -1};

Vector2::Vector2(const sf::Vector2f & original) {
    this->x = original.x;
    this->y = original.y;
}

Vector2 Vector2::operator-() {
    return Vector2(-this->x, -this->y);
}

Vector2 Vector2::operator+() {
    return *this;
}

Vector2 Vector2::operator+(const sf::Vector2f & operand) {
    return Vector2(this->x + operand.x, this->y + operand.y);
}

Vector2 & Vector2::operator+=(const sf::Vector2f & operand) {
    this->x += operand.x;
    this->y += operand.y;
    return *this;
}

Vector2 Vector2::operator-(const sf::Vector2f & operand) {
    return Vector2(this->x - operand.x, this->y - operand.y);
}

Vector2 & Vector2::operator-=(const sf::Vector2f & operand) {
    this->x -= operand.x;
    this->y -= operand.y;
    return *this;
}

Vector2 Vector2::operator*(const sf::Vector2f & operand) {
    return Vector2(this->x * operand.x, this->y * operand.y);
}

Vector2 & Vector2::operator*=(const sf::Vector2f & operand) {
    this->x *= operand.x;
    this->y *= operand.y;
    return *this;
}

Vector2 Vector2::operator/(const sf::Vector2f & operand) {
    return Vector2(this->x / operand.x, this->y / operand.y);
}

Vector2 & Vector2::operator/=(const sf::Vector2f & operand) {
    this->x /= operand.x;
    this->y /= operand.y;
    return *this;
}

void Vector2::normalize() {
    float magn = this->magnitude();
    if (magn != 0.f) {
        *this /= magn;
    }
}

Vector2 Vector2::normalized() const {
    Vector2 result = *this;
    result.normalize();
    return result;
}

float Vector2::sqrMagnitude() const {
    return x * x + y * y;
}

float Vector2::magnitude() const {
    return hypotf(this->x, this->y);
}

float Vector2::dot(const sf::Vector2f & v1, const sf::Vector2f & v2) {
    return v1.x * v2.x + v1.y * v2.y;
}

float Vector2::cross(const sf::Vector2f & v1, const sf::Vector2f & v2) {
    return v1.x * v2.y - v2.x * v1.y;
}