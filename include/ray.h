#ifndef RAY_H
#define RAY_H

#include "..\include\vec3.h"

class ray {
   private:
    point3 orig;
    vec3 dir;

   public:
    // 构造函数
    ray();
    ray(const point3&, const vec3&);

    // 访问器
    point3 origin() const;
    vec3 direction() const;

    point3 at(double) const;
};

#endif