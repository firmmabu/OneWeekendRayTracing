#include "../include/camera.h"
#include <fstream>
#include <iostream>
#include "../include/ray.h"

void camera::render(const hittable_list& world) {
    initialize();

    std::ofstream ofs;
    ofs.open("image.ppm", std::ios::out);
    ofs << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; ++j) {
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0, 0, 0);
            for (int sample = 0; sample < this->samples_per_pixel; ++sample) {
                ray r = get_ray(i, j);
                pixel_color += ray_color(r, world, max_depth);
            }
            write_color(ofs, pixel_color, this->samples_per_pixel);
        }
    }

    std::clog << "\rDone.                 \n";
}

void camera::initialize() {
    image_height = static_cast<int>(image_width / aspect_ratio);
    // 保证图片高度至少为1
    image_height = (image_height < 1) ? 1 : image_height;

    camera_center = lookfrom;

    // 相机参数
    // double focal_length = (lookfrom - lookat).length();
    double theta = deg_to_rad(vfov);
    double h = tan(theta / 2);
    double viewport_height = 2 * h * focus_dist;
    double viewport_width =
        viewport_height * (static_cast<double>(image_width) / image_height);

    // 既然和视线相反，那为什么不是lookat-lookfrom？已经确定lookat-lookfrom是错的，但不知道为什么
    w = unit_vector(lookfrom - lookat);
    u = unit_vector(cross(vup, w));
    v = cross(w, u);

    vec3 viewport_horizontal = viewport_width * u;
    vec3 viewport_vertical = viewport_height * -v;

    pixel_delta_horizontal = viewport_horizontal / image_width;
    pixel_delta_vertical = viewport_vertical / image_height;

    point3 viewport_up_left_corner = camera_center - focus_dist * w -
                                     viewport_horizontal / 2 -
                                     viewport_vertical / 2;
    pixel00_location = viewport_up_left_corner +
                       0.5 * (pixel_delta_horizontal +
                              pixel_delta_vertical);  // 第一个像素的中心坐标

    double defocus_radius = focus_dist * tan(deg_to_rad(defocus_angle / 2));
    defocus_disk_horizontal = u * defocus_radius;
    defocus_disk_vertical = v * defocus_radius;
}

ray camera::get_ray(int i, int j) const {
    point3 pixel_center = pixel00_location + i * pixel_delta_horizontal +
                          j * pixel_delta_vertical;
    vec3 pixel_sample = pixel_center + pixel_sample_square();

    point3 ray_origin =
        defocus_angle <= 0 ? camera_center : defocus_disk_sample();
    vec3 ray_direction = unit_vector(pixel_sample - ray_origin);

    return ray(ray_origin, ray_direction);
}

point3 camera::defocus_disk_sample() const {
    point3 p = random_in_unit_disk();
    return camera_center + p.x() * defocus_disk_horizontal +
           p.y() * defocus_disk_vertical;
}

vec3 camera::pixel_sample_square() const {
    double px = -0.5 + random_double();
    double py = -0.5 + random_double();

    return px * pixel_delta_horizontal + py * pixel_delta_vertical;
}

color camera::ray_color(const ray& r, const hittable& world, int depth) const {
    if (depth <= 0)
        return color(0, 0, 0);

    hit_record rec;
    if (world.hit(r, interval(0.001, inf), rec)) {
        ray scattered;
        color attenuation;
        // 发生散射/镜面反射/折射
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            return attenuation * ray_color(scattered, world, depth - 1);
        }

        // 发生漫反射
        vec3 direction = random_unit_vector() + rec.normal;
        return 0.5 * ray_color(ray(rec.p, direction), world, depth - 1);
    }

    vec3 unit_direction = unit_vector(r.direction());
    double a = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}