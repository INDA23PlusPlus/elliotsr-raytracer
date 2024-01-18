#pragma once

#include <stdlib.h>
#include <math.h>

#define PI 3.1415926535897932385

static const double DEG2RAD = PI / 180.0;
static const double RAD2DEG = 180.0 / PI;

double frand(void) {
    return rand() / (RAND_MAX + 1.0);
}

double clamp(double x, double a, double b) {
    return fmax(fmin(x, b), a);
}
double clamp01(double x) {
    return clamp(x, 0.0, 1.0);
}

double lerp(double a, double b, double t) {
    return a + (b - a) * t;
}

double unlerp(double x, double a, double b) {
    return (x - a) / (b - a);
}

double remap(double x, double a1, double b1, double a2, double b2) {
    return lerp(a2, b2, unlerp(x, a1, b1));
}