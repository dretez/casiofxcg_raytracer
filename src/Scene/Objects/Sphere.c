#include "Scene/Objects/Sphere.h"
#include "Scene/Ray.h"

#include <math.h>

static int  Sphere_intersect(const Object* self, const Ray* ray, float maxDist);
static int  Sphere_intersectAt(const Object* self, const Ray* ray, float maxDist, float* t);
static Vec3 Sphere_normal(const Object* sphere, Vec3 hitPoint);

const ObjectVTable sphere_vtable = (ObjectVTable){
    .intersect   = Sphere_intersect,
    .intersectAt = Sphere_intersectAt,
    .normal      = Sphere_normal,
};

void Sphere_init(Sphere* sphere, const Material* material, Vec3 center, float radius) {
    *sphere = (Sphere){
        .super =
            (Object){
                .material = material,
                .vtable   = &sphere_vtable,
            },
        .center    = center,
        .radius    = radius,
        .radius2   = radius * radius,
        .invradius = 1.0f / radius,
    };
}

static int Sphere_intersect(const Object* self, const Ray* ray, float maxDist2) {
    const Sphere* sphere = (Sphere*)self;
    Vec3          oc     = Vec3_sub(ray->origin, sphere->center);

    float b = Vec3_dot(oc, ray->direction);
    if (b >= 0.0f) return 0;
    float c = Vec3_dot(oc, oc) - sphere->radius2;
    if (c < 0.0f) return 1;

    float disc = b * b - maxDist2 * c;
    if (disc < 0.0f) return 0;

    float limit = -b - maxDist2;
    return limit <= 0.0f || disc > limit * limit;
}

static int Sphere_intersectAt(const Object* self, const Ray* ray, float maxDist, float* t) {
    const Sphere* sphere = (Sphere*)self;
    Vec3          oc     = Vec3_sub(ray->origin, sphere->center);

    float b = Vec3_dot(oc, ray->direction);
    float c = Vec3_dot(oc, oc) - sphere->radius2;
    if (c >= 0.0f && b > 0.0f) return 0;

    float discriminant = b * b - c;
    if (discriminant < 0.0f) return 0;
    float dLimit = -(maxDist + b);
    if (dLimit > 0.0f && discriminant <= dLimit * dLimit) return 0;

    float d      = sqrtf(discriminant);
    float minusB = -b;
    if (minusB >= d && minusB < d + maxDist) {
        *t = minusB - d;
        return 1;
    }
    *t = minusB + d;
    return *t >= 0.0f && *t < maxDist;
}

static Vec3 Sphere_normal(const Object* obj, Vec3 hitPoint) {
    const Sphere* sphere = (const Sphere*)obj;
    return Vec3_scale(Vec3_sub(hitPoint, sphere->center), sphere->invradius);
}
