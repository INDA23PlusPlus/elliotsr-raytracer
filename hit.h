#pragma once

#include <stdbool.h>

#include "vec3.h"

typedef struct Material Material;
struct Hit {
    vec3 p;
    vec3 normal;
    const Material* mat;
    double t;
    bool front_face;
};
#include "material.h"

void set_face_normal(Hit* hit, vec3 dir, vec3 out_normal) {
    hit->front_face = vec3_dot(dir, out_normal) < 0.0;
    hit->normal = hit->front_face ? out_normal : vec3_flip(out_normal);
}