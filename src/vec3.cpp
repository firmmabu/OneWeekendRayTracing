#include "../include/vec3.h"
#include <cmath>

double vec3::x() const {
    return this->e[0];
}

double vec3::y() const {
    return this->e[1];
}

double vec3::z() const {
    return this->e[2];
}

vec3 vec3::operator-() const {
    return vec3(-e[0], -e[1], -e[2]);
}

double vec3::operator[](int i) const {
    return e[i];
}

double& vec3::operator[](int i) {
    return e[i];
}

vec3& vec3::operator+=(const vec3& v) {
    this->e[0] += v.x();
    this->e[1] += v.y();
    this->e[2] += v.z();

    return *this;
}

vec3& vec3::operator*=(const vec3& v) {
    this->e[0] *= v[0];
    this->e[1] *= v[1];
    this->e[2] *= v[2];
    return *this;
}

vec3& vec3::operator/=(const vec3& v) {
    this->e[0] /= v[0];
    this->e[1] /= v[1];
    this->e[2] /= v[2];
    return *this;
}

double vec3::length_squared() const {
    return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
}

double vec3::length() const {
    return std::sqrt(length_squared());
}

vec3 vec3::random() {
    return vec3(random_double(), random_double(), random_double());
}

vec3 vec3::random(double min, double max) {
    return vec3(random_double(min, max), random_double(min, max),
                random_double(min, max));
}

bool vec3::near_zero() const {
    auto s = 1e-8;
    return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
}

/*
*this:入射光方向的单位向量
n:法线
*/
vec3 vec3::reflect(const vec3& n) {
    return *this - 2 * dot(*this, n) * n;
}

/*
*this:入射光方向的单位向量
n:法线
eta_over_etaprime:两种介质折射率之比
*/
vec3 vec3::refrect(const vec3& n, double eta_over_etaprime) {
    double cos_theta = fmin(dot(-*this, n), 1.0);

    //WRONG
    // vec3 r_prime_horizontal =
    //     eta_over_etaprime * (*this) * sqrt(1 - cos_theta * cos_theta);

    vec3 r_prime_horizontal = eta_over_etaprime * (*this + cos_theta * n);
    vec3 r_prime_vertical =
        -sqrt(fabs(1.0-r_prime_horizontal.length_squared())) * n;

    return r_prime_horizontal + r_prime_vertical;
}