#pragma once

#include <stdio.h>

#include "util.h"
#include "color.h"
#include "hittable.h"
#include "material.h"
#include "scene.h"

typedef struct Camera {

    double aspect_ratio;
    int image_width;
    int samples_per_pixel;
    int max_depth;
    double vfov;
    vec3 lookfrom;
    vec3 lookat;
    vec3 vup;
    double defocus_angle;
    double focus_dist;   

    int image_height;    
    vec3 center;         
    vec3 pixel00_loc;    
    vec3 pixel_delta_u;  
    vec3 pixel_delta_v;  
    vec3 u, v, w;        
    vec3 defocus_disk_u;
    vec3 defocus_disk_v;

} Camera;

Camera camera_default(void) {
    return (Camera) {
        .aspect_ratio = 1.0,
        .image_width = 100,
        .samples_per_pixel = 10,
        .max_depth = 10,
        .vfov = 90.0,
        .lookfrom = (vec3) { 0.0, 0.0, -1.0 },
        .lookat = (vec3) { 0.0, 0.0, 0.0 },
        .vup = (vec3) { 0.0, 1.0, 0.0 },
        .defocus_angle = 0.0,
        .focus_dist = 10.0
    };
}

void camera_init(Camera* cam) {

    cam->image_height = (cam->image_width / cam->aspect_ratio);
    cam->image_height = (cam->image_height < 1) ? 1 : cam->image_height;

    cam->center = cam->lookfrom;

    // Determine viewport dimensions.
    double theta = cam->vfov * DEG2RAD;
    double h = tan(theta / 2.0);
    double viewport_height = 2.0 * h * cam->focus_dist;
    double viewport_width = viewport_height * (cam->image_width / (double)cam->image_height);

    // Calculate the u,v,w unit basis vectors for the camera coordinate frame.
    cam->w = vec3_norm(vec3_sub(cam->lookfrom, cam->lookat));
    cam->u = vec3_norm(vec3_cross(cam->vup, cam->w));
    cam->v = vec3_cross(cam->w, cam->u);

    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    vec3 viewport_u = vec3_scale(cam->u, viewport_width);    // Vector across viewport horizontal edge
    vec3 viewport_v = vec3_scale(cam->v, -viewport_height);  // Vector down viewport vertical edge

    // Calculate the horizontal and vertical delta vectors to the next pixel.
    cam->pixel_delta_u = vec3_scale(viewport_u, 1.0 / cam->image_width);
    cam->pixel_delta_v = vec3_scale(viewport_v, 1.0 / cam->image_height);

    // Calculate the location of the upper left pixel.
    vec3 cf = vec3_scale(cam->w, cam->focus_dist);
    vec3 c00 = vec3_scale(vec3_add(viewport_u, viewport_v), 0.5);
    vec3 viewport_upper_left = vec3_sub(cam->center, vec3_add(cf, c00));
    cam->pixel00_loc = vec3_add(viewport_upper_left, vec3_scale(vec3_add(cam->pixel_delta_u, cam->pixel_delta_v), 0.5));

    // Calculate the camera defocus disk basis vectors.
    double defocus_radius = cam->focus_dist * tan(DEG2RAD * cam->defocus_angle * 0.5);
    cam->defocus_disk_u = vec3_scale(cam->u, defocus_radius);
    cam->defocus_disk_v = vec3_scale(cam->v, defocus_radius);
}

vec3 pixel_sample_square(const Camera* cam) {
    double px = -0.5 + frand();
    double py = -0.5 + frand();
    return vec3_add(vec3_scale(cam->pixel_delta_u, px), vec3_scale(cam->pixel_delta_v, py));
}

vec3 pixel_sample_disk(const Camera* cam, double radius) {
    vec3 p = vec3_scale(vec3_rand_disk(), radius);
    double px = p.x;
    double py = p.y;
    return vec3_add(vec3_scale(cam->pixel_delta_u, px), vec3_scale(cam->pixel_delta_v, py));
}

vec3 defocus_disk_sample(const Camera* cam) {
    vec3 p = vec3_rand_disk();
    return vec3_add(cam->center, vec3_add(vec3_scale(cam->defocus_disk_u, p.x), vec3_scale(cam->defocus_disk_v, p.y)));
}

Ray get_ray(const Camera* cam, int i, int j) {

    vec3 u = vec3_scale(cam->pixel_delta_u, i);
    vec3 v = vec3_scale(cam->pixel_delta_v, j);

    vec3 pixel_center = vec3_add(cam->pixel00_loc, vec3_add(u, v));
    vec3 pixel_sample = vec3_add(pixel_center, pixel_sample_square(cam));

    vec3 ray_origin = (cam->defocus_angle <= 0) ? cam->center : defocus_disk_sample(cam);
    vec3 ray_direction = vec3_sub(pixel_sample, ray_origin);

    return (Ray) {
        .origin = ray_origin,
        .direction = ray_direction
    };
}

vec3 ray_color(Ray ray, int depth, Scene scene) {

    if (depth <= 0) {
        vec3_all(0.0);
    }

    Hit hit;
    
    if (scene_hit(scene, ray, interval(0.001, INFINITY), &hit)) {
        vec3 scattered;
        vec3 attenuation;
        if (material_scatter(*hit.mat, ray.direction, &hit, &attenuation, &scattered)) {
            ray.origin = hit.p;
            ray.direction = scattered;
            vec3_mul(attenuation, ray_color(ray, depth - 1, scene));
        }
        return vec3_all(0.0);
    }

    vec3 unit_direction = vec3_norm(ray.direction);
    double a = 0.5 * (unit_direction.y + 1.0);
    static const vec3 background = { 0.5, 0.7, 1.0 };
    
    return vec3_add(vec3_all(1.0 - a), vec3_scale(background, a));
}

void camera_render(Camera* cam, Scene scene) {
    
    camera_init(cam);

    printf("P3\n%d %d\n255\n", cam->image_width, cam->image_height);

    for (int j = 0; j < cam->image_height; ++j) {
        for (int i = 0; i < cam->image_width; ++i) {
            vec3 pixel_color = vec3_all(0.0);
            for (int sample = 0; sample < cam->samples_per_pixel; ++sample) {
                Ray r = get_ray(cam, i, j);
                pixel_color = vec3_add(pixel_color, ray_color(r, cam->max_depth, scene));
            }
            print_color(stdout, pixel_color, cam->samples_per_pixel);
        }
    }

    fprintf(stderr, "Done\n");
}