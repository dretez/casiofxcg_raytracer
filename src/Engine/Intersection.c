#include "Engine/Intersection.h"
#include "Scene/Objects/Object.h"

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
    const Object* hit     = NULL;

    for (int i = 0; i < scene->objectCount; i++) {
        Object *obj = scene->objects[i];
        float t;
        if (!obj->vtable->intersectAt(obj, &ray, nearest, &t)) continue;
        nearest = t;
        hit     = obj;
    }

    if (!hit) return noHit;
    Vec3 hitPoint = Vec3_add(ray.origin, Vec3_scale(ray.direction, nearest));
    Vec3 normal   = hit->vtable->normal(hit, hitPoint);

    return (HitRecord){
        .normal =
            (Ray){
                .origin    = hitPoint,
                .direction = normal,
            },
        .object = hit,
    };
}
