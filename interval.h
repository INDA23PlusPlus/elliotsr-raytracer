#pragma once

#include <stdbool.h>

#include "util.h"

typedef struct {
    double min, max;
} Interval;

static const Interval interval_empty = { .min = INFINITY, .max = -INFINITY };
static const Interval interval_universe = { .min = -INFINITY, .max = INFINITY };

static const Interval interval(double min, double max) {
    return (Interval) {
        .min = min,
        .max = max
    };
}

double interval_size(Interval interval) {
    return interval.max - interval.min;
}

Interval interval_expand(Interval interval, double delta) {
    double padding = delta / 2.0;
    return (Interval) {
        .min = interval.min - padding,
        .max = interval.max + padding
    };
}

bool interval_contains(Interval interval, double x) {
    return interval.min <= x && x <= interval.max;
}

bool interval_surrounds(Interval interval, double x) {
    return interval.min < x && x < interval.max;
}

double interval_clamp(Interval interval, double x) {
    return clamp(x, interval.min, interval.max);
}
