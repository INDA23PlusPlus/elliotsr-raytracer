#pragma once

#include <stdio.h>
#include <math.h>
#include <stdbool.h>

#include "util.h"

typedef struct {
    double x, y, z;
} vec3;

vec3 vec3_add(vec3 u, vec3 v) {
    return (vec3) {
        .x = u.x + v.x,
        .y = u.y + v.y,
        .z = u.z + v.z,
    };
}

vec3 vec3_sub(vec3 u, vec3 v) {
    return (vec3) {
        .x = u.x - v.x,
        .y = u.y - v.y,
        .z = u.z - v.z,
    };
}

vec3 vec3_mul(vec3 u, vec3 v) {
    return (vec3) {
        u.x * v.x,
        u.y * v.y,
        u.z * v.z
    };
}

vec3 vec3_div(vec3 u, vec3 v) {
    return (vec3) {
        u.x / v.x,
        u.y / v.y,
        u.z / v.z
    };
}

vec3 vec3_scale(vec3 v, double s) {
    return (vec3) {
        .x = v.x * s,
        .y = v.y * s,
        .z = v.z * s
    };
}

vec3 vec3_flip(vec3 v) {
    return vec3_scale(v, -1.0);
}

double vec3_dot(vec3 u, vec3 v) {
    return u.x * v.x + u.y * v.y + u.z * v.z;
}

double vec3_sqrlen(vec3 v) {
    return vec3_dot(v, v);
}

double vec3_len(vec3 v) {
    return sqrt(vec3_sqrlen(v));
}

vec3 vec3_all(double t) {
    return (vec3) { .x = t, .y = t, .z = t };
}

bool vec3_nearzero(vec3 v) {
    static const double epsilon = 1e-8;
    return (fabs(v.x) < epsilon) && (fabs(v.y) < epsilon) && (fabs(v.z) < epsilon);
}

vec3 vec3_clamp(vec3 v, vec3 a, vec3 b) {
    return (vec3) {
        .x = clamp(v.x, a.x, b.x),
        .y = clamp(v.y, a.y, b.y),
        .z = clamp(v.z, a.z, b.z),
    };
}

vec3 vec3_clamp_all(vec3 v, float a, float b) {
    return vec3_clamp(v, vec3_all(a), vec3_all(b));
}

vec3 vec3_rand(void) {
    return (vec3) {
        .x = frand(),
        .y = frand(),
        .z = frand()
    };
}

vec3 vec3_lerp(vec3 a, vec3 b, vec3 t) {
    return (vec3) {
        .x = lerp(a.x, b.x, t.x),
        .y = lerp(a.y, b.y, t.y),
        .z = lerp(a.z, b.z, t.z)
    };
}

vec3 vec3_unlerp(vec3 v, vec3 a, vec3 b) {
    return (vec3) {
        .x = unlerp(v.x, a.x, b.x),
        .y = unlerp(v.y, a.y, b.y),
        .z = unlerp(v.z, a.z, b.z)
    };
}

vec3 vec3_lerp_all(vec3 a, vec3 b, double t) {
    return vec3_lerp(a, b, vec3_all(t));
}

double vec3_unlerp_all(vec3 v, vec3 a, vec3 b) {
    return vec3_unlerp(v, a, b).x;
}

vec3 vec3_cross(vec3 u, vec3 v) {
    return (vec3) {
        .x = u.y * v.z - u.z * v.y,
        .y = -(u.x * v.z - u.z * v.x),
        .z = u.x * v.y - u.y * v.x
    };
}

vec3 vec3_norm(vec3 v) {
    return vec3_scale(v, 1.0 / vec3_len(v));
}

vec3 vec3_rand_disk(void) {
    static const vec3 min = { .x = -1.0, .y = -1.0, .z = 0.0 };
    static const vec3 max = { .x = 1.0, .y = 1.0, .z = 0.0 };
    vec3 p;
    do {
        p = vec3_lerp(min, max, vec3_rand());
    } while(vec3_sqrlen(p) >= 1.0);
    return p;
}

vec3 vec3_rand_sphere(void) {
    static const vec3 min = { .x = -1.0, .y = -1.0, .z = -1.0 };
    static const vec3 max = { .x = 1.0, .y = 1.0, .z = 1.0 };
    vec3 p;
    do {
        p = vec3_lerp(min, max, vec3_rand());
    } while(vec3_sqrlen(p) >= 1.0);
    return p;
}

vec3 vec3_rand_unit(void) {
    return vec3_norm(vec3_rand_sphere());
}

vec3 vec3_rand_hemisphere(vec3 n) {
    vec3 v = vec3_rand_unit();
    return (vec3_dot(v, n) > 0.0) ? v : vec3_flip(v);
}

vec3 vec3_reflect(vec3 v, vec3 n) {
    return vec3_sub(v, vec3_scale(n, 2.0 * vec3_dot(v,n)));
}

vec3 vec3_refract(vec3 uv, vec3 n, double etai_over_etat) {
    double cos_theta = fmin(vec3_dot(vec3_flip(uv), n), 1.0);
    vec3 r_out_perp =  vec3_scale(vec3_add(uv, vec3_scale(n, cos_theta)), etai_over_etat);
    vec3 r_out_parallel = vec3_scale(n, -sqrt(fabs(1.0 - vec3_sqrlen(r_out_perp))));
    return vec3_add(r_out_perp, r_out_parallel);
}
