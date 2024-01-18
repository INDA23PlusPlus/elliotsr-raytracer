#include "util.h"

#include "color.h"
#include "util.h"
#include "vec3.h"
#include "hittable.h"
#include "ray.h"
#include "hit.h"
#include "scene.h"
#include "camera.h"

int main() {

    Scene world = scene_create(512);

    Material ground_material = {
        .type = MATERIAL_LAMBERTIAN,
        .object = &(MaterialLambertian) {
            .albedo = vec3_all(0.5)
        }
    };

    scene_add(&world, (Hittable) {
        .type = HITTABLE_SPHERE,
        .mat = ground_material,
        .object = &(Sphere) {
            .center = { 0.0, -1000.0, 0.0 },
            .radius = 1000.0,
        }
    });

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {

            double choose_mat = frand();
            vec3 center = { a + 0.9 * frand(), 0.2, b + 0.9 * frand() };

            if (vec3_len(vec3_sub(center, (vec3) { 4.0, 0.2, 0.0 })) > 0.9) {
                Material sphere_material;
                if (choose_mat < 0.8) {
                    vec3 albedo = vec3_mul(vec3_rand(), vec3_rand());
                    sphere_material = (Material) {
                        .type = MATERIAL_LAMBERTIAN,
                        .object = &(MaterialLambertian) {
                            .albedo = albedo
                        }
                    };
                } else if (choose_mat < 0.95) {
                    vec3 albedo = vec3_all(lerp(0.5, 1.0, frand()));
                    double fuzz = frand() * 0.5;
                    sphere_material = (Material) {
                        .type = MATERIAL_METAL,
                        .object = &(MaterialMetal) {
                            .albedo = albedo,
                            .fuzz = fuzz
                        }
                    };
                } else {
                    sphere_material = (Material) {
                        .type = MATERIAL_DIELECTRIC,
                        .object = &(MaterialDielectric) {
                            .ir = 1.5
                        }
                    };
                }
                scene_add(&world, (Hittable) {
                    .type = HITTABLE_SPHERE,
                    .mat = sphere_material,
                    .object = &(Sphere) {
                        .center = center, .radius = 0.2
                    }
                });
            }
        }
    }

    Material material1 = {
        .type = MATERIAL_DIELECTRIC,
        .object = &(MaterialDielectric) {
            .ir = 1.5
        }
    };

    Material material2 = {
        .type = MATERIAL_LAMBERTIAN,
        .object = &(MaterialLambertian) {
            .albedo = { 0.4, 0.2, 0.1 },
        }
    };

    Material material3 = {
        .type = MATERIAL_METAL,
        .object = &(MaterialMetal) {
            .albedo = { 0.7, 0.6, 0.5 },
            .fuzz = 0.0
        }
    };

    scene_add(&world, (Hittable) {
        .type = HITTABLE_SPHERE,
        .mat = material1,
        .object = &(Sphere) {
            .center = (vec3) { 0.0, 1.0, 0.0 },
            .radius = 1.0
        }
    });

    scene_add(&world, (Hittable) {
        .type = HITTABLE_SPHERE,
        .mat = material2,
        .object = &(Sphere) {
            .center = (vec3) { -4.0, 1.0, 0.0 },
            .radius = 1.0
        }
    });

    scene_add(&world, (Hittable) {
        .type = HITTABLE_SPHERE,
        .mat = material3,
        .object = &(Sphere) {
            .center = (vec3) { 4.0, 1.0, 0.0 },
            .radius = 1.0
        }
    });

    Camera cam = camera_default();

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 1200;
    cam.samples_per_pixel = 10;
    cam.max_depth         = 20;

    cam.vfov     = 20.0;
    cam.lookfrom = (vec3) { 13.0, 2.0, 3.0 };
    cam.lookat   = vec3_all(0.0);
    cam.vup      = (vec3) { 0.0, 1.0, 0.0 };

    cam.defocus_angle = 0.6;
    cam.focus_dist    = 10.0;

    camera_render(&cam, world);
}
