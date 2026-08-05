#include "Engine/Intersection.h"

static const HitRecord noHit = (HitRecord){
    .point  = (Vec3){ 0 },
    .normal = (Vec3){ 0 },

    .innerOffset = (Vec3){ 0 },
    .outerOffset = (Vec3){ 0 },
    .rdotn = 0,

    .object = NULL,
};

HitRecord intersectScene(const Ray* ray, const Scene* scene) {
    geo_t         nearest = GEO_MAX;
    const Object* hit     = NULL;
    Object* objects = scene->objects;

    for (int i = 0, count = scene->objectCount; i < count; i++) {
        const Object* obj = &objects[i];
        geo_t   t;
        if (!obj->vtable->intersectAt(obj->data, ray, nearest, &t)) continue;
        nearest = t;
        hit     = obj;
    }

    if (!hit) return noHit;
    Vec3 hitPoint = Vec3_addScaled(ray->origin, ray->direction, nearest);
    Vec3 normal   = hit->vtable->normal(hit->data, hitPoint);
    Vec3 offset   = Vec3_scale(normal, RAY_EPSILON);

    return (HitRecord){
        .point  = hitPoint,
        .normal = normal,

        .innerOffset = Vec3_sub(hitPoint, offset),
        .outerOffset = Vec3_add(hitPoint, offset),
        .rdotn = Vec3_dot(ray->direction, normal),

        .object = hit,
    };
}
