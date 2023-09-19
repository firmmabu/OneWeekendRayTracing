#include "../include/hittable.h"
#include <cmath>

void hit_record::set_face_normal(const ray& r, const vec3& outward_normal) {
    front_face = dot(r.direction(), outward_normal) < 0;
    normal = front_face ? outward_normal : -outward_normal;
}

sphere::sphere(point3 center, double radius, std::shared_ptr<material> material)
    : _center(center), _radius(radius), _material(material) {}

bool sphere::hit(const ray& r, interval ray_t, hit_record& rec) const {
    vec3 oc = r.origin() - _center;
    double a = dot(r.direction(), r.direction());
    double h = dot(r.direction(), oc);
    double c = dot(oc, oc) - _radius * _radius;
    double delta = h * h - a * c;

    if (delta < 0)
        return false;

    double sqrt_delta = sqrt(delta);
    double root = (-h - sqrt_delta) / a;
    if (!ray_t.surrounds(root)) {
        root = (-h + sqrt_delta) / a;
        if (!ray_t.surrounds(root))
            return false;
    }

    rec.t = root;
    rec.p = r.at(root);
    rec.mat_ptr = this->_material;
    vec3 outward_normal = (rec.p - _center) / _radius;
    rec.set_face_normal(r, outward_normal);

    return true;
}

hittable_list::hittable_list() {}

hittable_list::hittable_list(std::shared_ptr<hittable> object) {
    this->objects.emplace_back(object);
}

void hittable_list::clear() {
    this->objects.clear();
}

void hittable_list::add(std::shared_ptr<hittable> object) {
    this->objects.emplace_back(object);
}

bool hittable_list::hit(const ray& r, interval ray_t, hit_record& rec) const {
    hit_record temp_rec;
    bool hit_anything = false;
    double cloest_so_far = ray_t.max;

    for (const std::shared_ptr<hittable>& object : objects) {
        if (object->hit(r, interval(ray_t.min, cloest_so_far), temp_rec)) {
            hit_anything = true;
            cloest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}