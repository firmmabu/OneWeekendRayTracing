#ifndef VEC3_H
#define VEC3_H

#include <iostream>
#include "..\include\common.h"

class vec3 {
   private:
    double e[3];

   public:
    // 构造函数
    vec3() : e{0, 0, 0} {}
    vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

    // 访问器
    double x() const;
    double y() const;
    double z() const;

    // 重载单目运算符-
    vec3 operator-() const;
    // 重载[]
    double operator[](int) const;
    double& operator[](int);
    // 重载+=、*=、/=
    vec3& operator+=(const vec3&);
    vec3& operator*=(const vec3&);
    vec3& operator/=(const vec3&);

    double length_squared() const;
    double length() const;

    static vec3 random();
    static vec3 random(double, double);

    bool near_zero() const;

    vec3 reflect(const vec3&);

    vec3 refrect(const vec3&, double);
};
// 空间中的点用三维向量表示
using point3 = vec3;
// 重载输出
inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
    return out << v[0] << ' ' << v[1] << ' ' << v[2];
}
// 重载+
inline vec3 operator+(const vec3& u, const vec3& v) {
    return vec3(u[0] + v[0], u[1] + v[1], u[2] + v[2]);
}
// 重载-
inline vec3 operator-(const vec3& u, const vec3& v) {
    return vec3(u[0] - v[0], u[1] - v[1], u[2] - v[2]);
}
// 重载两个向量对应元素相乘*
inline vec3 operator*(const vec3& u, const vec3& v) {
    return vec3(u[0] * v[0], u[1] * v[1], u[2] * v[2]);
}
// 重载数乘*
inline vec3 operator*(double t, const vec3& v) {
    return vec3(t * v[0], t * v[1], t * v[2]);
}
inline vec3 operator*(const vec3& v, double t) {
    return t * v;
}
// 重载除以一个常数
inline vec3 operator/(vec3 v, double t) {
    return (1 / t) * v;
}
// 点乘
inline double dot(const vec3& u, const vec3& v) {
    return u[0] * v[0] + u[1] * v[1] + u[2] * v[2];
}
// 叉乘
inline vec3 cross(const vec3& u, const vec3& v) {
    return vec3(u[1] * v[2] - u[2] * v[1], u[2] * v[0] - u[0] * v[2],
                u[0] * v[1] - u[1] * v[0]);
}
// 转换为单位向量
inline vec3 unit_vector(vec3 v) {
    return v / v.length();
}

// 生成一个在单位球体内的点
inline point3 random_in_unit_sphere() {
    point3 p = vec3::random(-1, 1);
    while (p.length_squared() > 1) {
        p = vec3::random(-1, 1);
    }
    return p;
}

inline vec3 random_unit_vector() {
    return unit_vector(random_in_unit_sphere());
}

inline vec3 random_on_hemisphere(vec3& normal) {
    vec3 random_vector = random_unit_vector();
    if (dot(random_vector, normal) >= 0.0)
        return random_vector;

    return -random_vector;
}

inline vec3 random_in_unit_disk() {
    for (;;) {
        // 平行与xy平面的长度为2的正方形
        point3 p = point3(random_double(-1, 1), random_double(-1, 1), 0);

        // 如果随机到的点在半径为1的圆内，就return
        if (p.length_squared() < 1) {
            return p;
        }
    }
}

#endif