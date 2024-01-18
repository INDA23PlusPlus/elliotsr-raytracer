#pragma once

#include "util.h"
#include "vec3.h"


typedef struct MaterialLambertian {
    vec3 albedo;
} MaterialLambertian;

typedef struct MaterialMetal {
    vec3 albedo;
    double fuzz;
} MaterialMetal;

typedef struct MaterialDielectric {
    double ir;
} MaterialDielectric;

typedef enum MaterialType {
    MATERIAL_LAMBERTIAN,
    MATERIAL_METAL,
    MATERIAL_DIELECTRIC
} MaterialType;

typedef struct Hit Hit;
struct Material {
    MaterialType type;
    void* object;
};
#include "hit.h"

bool material_scatter_lambertian(const MaterialLambertian* mat, vec3 dir, const Hit* hit, vec3* attenuation, vec3* scattered) {

    vec3 scatter_direction = vec3_add(hit->normal, vec3_rand_unit());

    if (vec3_nearzero(scatter_direction)) {
        scatter_direction = hit->normal;
    }

    *scattered = scatter_direction;

    *attenuation = mat->albedo;

    return true;
}

bool material_scatter_metal(const MaterialMetal* mat, vec3 dir, const Hit* hit, vec3* attenuation, vec3* scattered) {

    vec3 reflected = vec3_reflect(vec3_norm(dir), hit->normal);

    *scattered = vec3_add(reflected, vec3_scale(vec3_rand_sphere(), mat->fuzz));

    *attenuation = mat->albedo;

    return vec3_dot(*scattered, hit->normal) > 0.0;
}

// Schlick's reflectance approximation
double schlick_reflectance(double cos, double ref_idx) {
    double r0 = (1.0 - ref_idx) / (1.0 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow(1.0 - cos, 5.0);
}

bool material_scatter_dielectric(const MaterialDielectric* mat, vec3 dir, const Hit* hit, vec3* attenuation, vec3* scattered) {

    *attenuation = vec3_all(1.0);

    double refraction_ratio = hit->front_face ? (1.0 / mat->ir) : mat->ir;

    vec3 unit_direction = vec3_norm(dir);
    double cos_theta = fmin(vec3_dot(vec3_flip(unit_direction), hit->normal), 1.0);
    double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

    bool cannot_refract = (refraction_ratio * sin_theta) > 1.0;
    vec3 direction;

    if (cannot_refract || schlick_reflectance(cos_theta, refraction_ratio) > frand())
        direction = vec3_reflect(unit_direction, hit->normal);
    else
        direction = vec3_refract(unit_direction, hit->normal, refraction_ratio);

    *scattered = direction;

    return true;
}

bool material_scatter(Material mat, vec3 dir, const Hit* hit, vec3* attenuation, vec3* scattered) {
    switch (mat.type) {
        case MATERIAL_LAMBERTIAN: return material_scatter_lambertian(mat.object, dir, hit, attenuation, scattered);
        case MATERIAL_METAL: return material_scatter_metal(mat.object, dir, hit, attenuation, scattered);
        case MATERIAL_DIELECTRIC: return material_scatter_dielectric(mat.object, dir, hit, attenuation, scattered);
    }
}

