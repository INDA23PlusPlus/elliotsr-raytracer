#pragma once

#include <stdbool.h>

#include "util.h"
#include "vec3.h"
#include "interval.h"
#include "hit.h"
#include "ray.h"

typedef struct Sphere {
    vec3 center;
    float radius;
    Material mat;
} Sphere;

typedef struct Plane {
    vec3 point;
    vec3 normal;
    Material mat;
} Plane;

typedef enum HittableType {
    HITTABLE_SPHERE,
    HITTABLE_PLANE
} HittableType;

typedef struct Hittable {
    HittableType type;
    void* object;
    Material mat;
} Hittable;

bool hittable_hit_sphere(const Sphere* sphere, Ray ray, Interval ray_t, Hit* hit) {
    
    vec3 oc = vec3_sub(ray.origin, sphere->center);
    double a = vec3_sqrlen(ray.direction);
    double half_b = vec3_dot(oc, ray.direction);
    double c = vec3_sqrlen(oc) - sphere->radius * sphere->radius;

    double discriminant = half_b * half_b - a * c;
    if (discriminant < 0.0) {
        return false;
    }

    double sqrtd = sqrt(discriminant);
    double root = (-half_b - sqrtd) / a;
    if (!interval_surrounds(ray_t, root)) {
        root = (-half_b + sqrtd) / a;
        if (!interval_surrounds(ray_t, root)) {
            return false;
        }
    }

    hit->t = root;
    hit->p = ray_at(ray, hit->t);
    vec3 outward_normal = vec3_scale(vec3_sub(hit->p, sphere->center), 1.0 / sphere->radius);
    set_face_normal(hit, ray.direction, outward_normal);

    hit->mat = &sphere->mat;

    return true;
}

bool hittable_hit_plane(const Plane* plane, Ray ray, Interval ray_t, Hit* hit) {
    // ...
    return true;
}

bool hittable_hit(Hittable hittable, Ray ray, Interval ray_t, Hit* hit) {
    switch(hittable.type) {
        case HITTABLE_SPHERE: return hittable_hit_sphere(hittable.object, ray, ray_t, hit);
        case HITTABLE_PLANE: return hittable_hit_plane(hittable.object, ray, ray_t, hit);
    }
}