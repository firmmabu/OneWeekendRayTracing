#ifndef MATERIAL_H
#define MATERIAL_H
#include "..\include\common.h"
#include "..\include\hittable.h"
#include "..\include\color.h"

class hit_record;

class material {
   public:
    virtual ~material() = default;
    virtual bool scatter(const ray&, const hit_record&, color&, ray&) const = 0;
};

class lambertian : public material {
   public:
    lambertian(const color&);

    bool scatter(const ray&, const hit_record&, color&, ray&) const override;

   private:
    color _albedo;
};

class metal : public material {
   public:
    metal(const color&, double);

    bool scatter(const ray&, const hit_record&, color&, ray&) const override;

   private:
    color _albedo;
    double _fuzz;
};

class dielectric : public material {
   public:
    dielectric(double);

    bool scatter(const ray&, const hit_record&, color&, ray&) const override;

   private:
    double _refrection_index;
    static double reflectance(double,double);
};

#endif