#pragma once

#include "vec3.h"

typedef struct Ray {
    vec3 origin;
    vec3 direction;
} Ray;

Ray ray_between(vec3 a, vec3 b) {
    return (Ray) {
        .origin = a,
        .direction = vec3_sub(b, a)
    };
}

vec3 ray_at(Ray r, double t) {
    return vec3_add(r.origin, vec3_scale(r.direction, t));
}