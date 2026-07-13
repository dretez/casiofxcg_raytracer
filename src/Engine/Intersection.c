#include "Engine/Intersection.h"
#include "Scene/Objects/Object.h"
#include "Scene/Ray.h"
#include "Vector/FloatingVector.h"
#include <float.h>

static const HitRecord noHit = (HitRecord){
    .point  = (Vec3){ 0 },
    .normal = (Vec3){ 0 },

    .offset = (Vec3){ 0 },
    .innerOffset = (Vec3){ 0 },
    .outerOffset = (Vec3){ 0 },
    .rdotn = 0,

    .object = NULL,
};

HitRecord intersectScene(Ray ray, const Scene* scene) {
    float         nearest = FLT_MAX;
    const Object* hit     = NULL;

    for (int i = 0; i < scene->objectCount; i++) {
        const Object* obj = scene->objects[i];
        float   t;
        if (!obj->vtable->intersectAt(obj, &ray, nearest, &t)) continue;
        nearest = t;
        hit     = obj;
    }

    if (!hit) return noHit;
    Vec3 hitPoint = Vec3_add(ray.origin, Vec3_scale(ray.direction, nearest));
    Vec3 normal   = hit->vtable->normal(hit, hitPoint);
    Vec3 offset   = Vec3_scale(normal, RAY_EPSILON);

    return (HitRecord){
        .point  = hitPoint,
        .normal = normal,

        .offset = offset,
        .innerOffset = Vec3_sub(hitPoint, offset),
        .outerOffset = Vec3_add(hitPoint, offset),
        .rdotn = Vec3_dot(ray.direction, normal),

        .object = hit,
    };
}
