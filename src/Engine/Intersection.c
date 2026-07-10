#include "Engine/Intersection.h"

HitRecord intersectScene(Ray ray, const Scene* scene) {
    float   nearest = 1e30;
    Sphere* hit     = NULL;
    float   t;

    for (int i = 0; i < scene->sphereCount; i++) {
        if (!intersectSphereAt(&ray, &scene->spheres[i], nearest, &t)) continue;
        nearest = t;
        hit     = &scene->spheres[i];
    }

    Vec3 hitPoint =
        hit ? Vec3_add(ray.origin, Vec3_scale(ray.direction, nearest)) : (Vec3){ 0, 0, 0 };
    Vec3 normal = hit ? Vec3_sub(hitPoint, hit->center) : (Vec3){ 0, 0, 0 };

    return (HitRecord){
        .normal = (Ray){ .origin = hitPoint, .direction = normal },
        .object = hit,
    };
}
