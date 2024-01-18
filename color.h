#pragma once

#include <math.h>

#include "vec3.h"

vec3 linear_to_gamma(vec3 color) {
    return (vec3) {
        .x = sqrt(color.x),
        .y = sqrt(color.y),
        .z = sqrt(color.z)
    };
}

void print_color(FILE* out, vec3 color, int samples) {

    vec3_scale(color, 1.0 / samples);

    color = linear_to_gamma(color);

    color = vec3_clamp_all(color, 0.000, 0.999);
    color = vec3_scale(color, 256.0);

    int r = color.x;
    int g = color.y;
    int b = color.z;

    fprintf(out, "%d %d %d\n", r, g, b);
}
