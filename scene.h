#pragma once

#include <stdbool.h>

#include "ray.h"
#include "hittable.h"
#include "hit.h"
#include "interval.h"

typedef struct Scene {
    Hittable* hittables;
    int size;
} Scene;

bool scene_hit(Scene scene, Ray ray, Interval ray_t, Hit* hit) {

    Hit temp;
    bool hit_anything = false;
    double closest_so_far = ray_t.max;

    for (int i = 0; i < scene.size; i++) {
        if (hittable_hit(scene.hittables[i], ray, interval(ray_t.min, closest_so_far), &temp)) {
            hit_anything = true;
            closest_so_far = temp.t;
            *hit = temp;
        }
    }

    return hit_anything;
}

Scene scene_create(int size) {
    return (Scene) {
        .hittables = malloc(size * sizeof(Hittable)),
        .size = 0
    };
}

void scene_add(Scene* scene, Hittable object) {
    scene->hittables[scene->size++] = object;
}

void scene_destroy(Scene scene) {
    free(scene.hittables);
}