#ifndef CAMERA_H
#define CAMERA_H

#include "../include/color.h"
#include "../include/common.h"
#include "../include/hittable.h"
#include "../include/material.h"

class camera {
   public:
    double aspect_ratio = 1.0;
    int image_width = 100;
    int samples_per_pixel = 10;
    int max_depth = 10;

    double vfov = 90;
    point3 lookfrom = point3(0, 0, -1);
    point3 lookat = point3(0, 0, 0);
    vec3 vup = vec3(0, 1, 0);  // view up，表示相机朝上的向量

    double defocus_angle = 0;
    double focus_dist = 10;

    void render(const hittable_list&);

   private:
    double image_height;
    point3 camera_center;
    point3 pixel00_location;
    vec3 pixel_delta_horizontal;
    vec3 pixel_delta_vertical;
    /*
    uvw：相机坐标系的轴
    u:指向相机的右边
    v:指向相机的上方
    w:指向相机的后方，和视线方向相反
    */
    vec3 u, v, w;
    vec3 defocus_disk_horizontal;
    vec3 defocus_disk_vertical;

    void initialize();

    ray get_ray(int, int) const;
    vec3 pixel_sample_square() const;
    point3 defocus_disk_sample() const;
    color ray_color(const ray&, const hittable&, int) const;
};

#endif