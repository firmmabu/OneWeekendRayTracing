#include "..\include\material.h"

lambertian::lambertian(const color& albedo) : _albedo(albedo) {}

bool lambertian::scatter(const ray& r_in,
                         const hit_record& rec,
                         color& attenuation,
                         ray& scattered) const {
    vec3 scatter_direction = rec.normal + random_unit_vector();

    if (scatter_direction.near_zero())
        scatter_direction = rec.normal;

    scattered = ray(rec.p, scatter_direction);
    attenuation = _albedo;
    return true;
}

metal::metal(const color& albedo, double fuzz)
    : _albedo(albedo), _fuzz(fuzz < 1 ? fuzz : 1) {}

bool metal::scatter(const ray& r_in,
                    const hit_record& rec,
                    color& attenuation,
                    ray& reflected) const {
    vec3 reflect_direction = unit_vector(r_in.direction()).reflect(rec.normal);

    reflected = ray(rec.p, reflect_direction + _fuzz * random_unit_vector());

    attenuation = _albedo;

    return dot(reflected.direction(), rec.normal) > 0;
}

dielectric::dielectric(double refrection_index)
    : _refrection_index(refrection_index) {}

bool dielectric::scatter(const ray& r_in,
                         const hit_record& rec,
                         color& attenuation,
                         ray& refrected) const {
    attenuation = vec3(1.0, 1.0, 1.0);
    double refraction_ratio = rec.front_face ? 1.0 / this->_refrection_index
                                             : this->_refrection_index;

    vec3 unit_direction = unit_vector(r_in.direction());
    double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
    double sin_theta = sqrt(1 - cos_theta * cos_theta);

    bool cannot_refract = (refraction_ratio * sin_theta) > 1.0;
    vec3 refrected_direction;
    /*
    Schlick近似估算
    在现实世界中，玻璃发生折射的概率会随着入射角而改变
    从一个很狭窄的角度去看玻璃窗，它会变成一面镜子
    */
    if (cannot_refract ||
        reflectance(cos_theta, refraction_ratio) > random_double()) {
        refrected_direction = unit_direction.reflect(rec.normal);
    } else {
        refrected_direction =
            unit_direction.refrect(rec.normal, refraction_ratio);
    }

    refrected = ray(rec.p, refrected_direction);

    return true;
}

double dielectric::reflectance(double cosine, double ref_index) {
    auto r0 = (1 - ref_index) / (1 + ref_index);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow(1 - cosine, 5);
}