#include "../include/interval.h"
#include "../include/common.h"

interval::interval() : min(+inf), max(-inf) {}

interval::interval(double _min, double _max) : min(_min), max(_max) {}

bool interval::contains(double x) const {
    return x >= min && x <= max;
}

bool interval::surrounds(double x) const {
    return x > min && x < max;
}

double interval::clamp(double x) const {
    if (x > max)
        return max;
    if (x < min)
        return min;

    return x;
}

const interval interval::empty(+inf, -inf);
const interval interval::universe(-inf, +inf);