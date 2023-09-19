#ifndef HITTABLE_H
#define HITTABLE_H

#include <memory>
#include <vector>
#include "..\include\interval.h"
#include "..\include\ray.h"

class material;

class hit_record {
   public:
    point3 p;
    vec3 normal;
    std::shared_ptr<material> mat_ptr;

    double t;
    bool front_face;

    void set_face_normal(const ray&, const vec3&);
};

class hittable {
   public:
    virtual ~hittable() = default;
    virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
};

class sphere : public hittable {
   public:
    sphere(point3, double, std::shared_ptr<material>);
    bool hit(const ray&, interval, hit_record&) const override;

   private:
    point3 _center;
    double _radius;
    std::shared_ptr<material> _material;
};

class hittable_list : public hittable {
   public:
    std::vector<std::shared_ptr<hittable>> objects;

    hittable_list();
    hittable_list(std::shared_ptr<hittable>);

    void clear();
    void add(std::shared_ptr<hittable>);

    bool hit(const ray& r, interval, hit_record& rec) const override;
};

#endif