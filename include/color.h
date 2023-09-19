#ifndef COLOR_H
#define COLOR_H

#include <cmath>
#include <iostream>
#include "../include/vec3.h"

// 用三维向量表示归一化的rgb颜色
using color = vec3;

void write_color(std::ostream&, color, int);

inline double linear2gamma(double linear) {
    return sqrt(linear);
}

#endif