//
// Created by Олег Бобров on 09.01.2020.
//

#include "Mathf.hpp"

float Mathf::clamp(float value, float min, float max) {
    if (value > max) return max;
    if (value < min) return min;
    return value;
}