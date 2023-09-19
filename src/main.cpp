#include <cmath>
#include <fstream>
#include <iostream>
#include "../include/camera.h"
#include "../include/color.h"
#include "../include/common.h"
#include "../include/hittable.h"
#include "../include/material.h"

using std::ios;

int main() {
    // World
    hittable_list world;

    // 地面
    auto ground_material = std::make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(
        std::make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

    // 随机小球
    for (int a = -11; a < 11; ++a) {
        for (int b = -11; b < 11; ++b) {
            auto choose_material = random_double();
            point3 center(a + 0.9 * random_double(), 0.2,
                          b + 0.9 * random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                std::shared_ptr<material> sphere_material;
                if (choose_material < 0.8) {
                    // 漫反射
                    auto albedo = color::random() * color::random();
                    sphere_material = std::make_shared<lambertian>(albedo);
                } else if (choose_material < 0.95) {
                    // 镜面反射
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = std::make_shared<metal>(albedo, fuzz);
                } else {
                    // 折射
                    sphere_material = std::make_shared<dielectric>(1.5);
                }
                world.add(
                    std::make_shared<sphere>(center, 0.2, sphere_material));
            }
        }
    }

    auto material1 = std::make_shared<dielectric>(1.5);
    world.add(std::make_shared<sphere>(point3(0, 1, 0), 1.0, material1));
    auto material2 = std::make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(std::make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));
    auto material3 = std::make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(std::make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 1200;
    cam.samples_per_pixel = 5;
    cam.max_depth = 50;

    cam.vfov = 20;
    cam.lookfrom = point3(13, 2, 3);
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0.6;
    cam.focus_dist = 10.0;

    cam.render(world);

    return 0;
}