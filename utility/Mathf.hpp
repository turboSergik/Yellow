//
// Created by Олег Бобров on 09.01.2020.
//

#ifndef WG_MATHF_HPP
#define WG_MATHF_HPP


namespace Mathf {
    const float PI = 3.14159265358f;
    const float DEG2RAD = PI/180.f;
    const float RAD2DEG = 180.f/PI;

    float clamp(float value, float min = 0, float max = 1);

    template <typename T>
    T lerp (const T & a, const T & b, float t) {
        t = clamp(t);
        return a + (b - a) * t;
    }
};


#endif //WG_MATHF_HPP
