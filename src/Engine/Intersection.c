#include "Engine/Intersection.h"

static const HitRecord noHit = (HitRecord){
    .normal =
        (Ray){
            .origin    = (Vec3){ 0, 0, 0 },
            .direction = (Vec3){ 0, 0, 0 },
        },
    .object = NULL,
};

HitRecord intersectScene(Ray ray, const Scene* scene) {
    float   nearest = 1e30;
    Sphere* hit     = NULL;

    for (int i = 0; i < scene->sphereCount; i++) {
        float t;
        if (!intersectSphereAt(&ray, &scene->spheres[i], nearest, &t)) continue;
        nearest = t;
        hit     = &scene->spheres[i];
    }

    if (!hit) return noHit;
    Vec3 hitPoint = Vec3_add(ray.origin, Vec3_scale(ray.direction, nearest));
    Vec3 normal   = Vec3_normalize(Vec3_sub(hitPoint, hit->center));

    return (HitRecord){
        .normal =
            (Ray){
                .origin    = hitPoint,
                .direction = normal,
            },
        .object = hit,
    };
}
