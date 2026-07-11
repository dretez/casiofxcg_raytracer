#include "Scene/Objects/Sphere.h"
#include "Scene/Ray.h"

#include <math.h>

int  Sphere_intersect(const Object* self, const Ray* ray, float maxDist);
int  Sphere_intersectAt(const Object* self, const Ray* ray, float maxDist, float* t);
Vec3 Sphere_normal(const Object* sphere, Vec3 hitPoint);

const ObjectVTable sphere_vtable = (ObjectVTable){
    .intersect   = Sphere_intersect,
    .intersectAt = Sphere_intersectAt,
    .normal      = Sphere_normal,
};

void Sphere_init(Sphere* sphere, const Material* material, Vec3 center, float radius) {
    *sphere = (Sphere){
        .super =
            (Object){
                .type     = SPHERE,
                .material = material,
                .vtable   = &sphere_vtable,
            },
        .center    = center,
        .radius    = radius,
        .radius2   = radius * radius,
        .invradius = 1.0f / radius,
    };
}

static inline int sphereQuadratic(const Sphere* sphere, const Ray* ray, float maxDist, float* b, float* d) {
    Vec3 oc = Vec3_sub(ray->origin, sphere->center);

    float bb = Vec3_dot(oc, ray->direction);
    float c = Vec3_dot(oc, oc) - sphere->radius2;

    float discriminant = bb * bb - c;
    if (discriminant < 0.0f) return 1;
    float dLimit = -(maxDist + bb);
    if (dLimit > 0.0f && discriminant < dLimit * dLimit) return 1;

    *b = bb;
    *d = sqrtf(discriminant);
    return 0;
}

int Sphere_intersect(const Object* self, const Ray* ray, float maxDist) {
    float b, d;
    if(sphereQuadratic((Sphere*)self, ray, maxDist, &b, &d)) return 0;
    if (b < d) return 1;
    return d >= b && d - b < maxDist;
}

int Sphere_intersectAt(const Object* self, const Ray* ray, float maxDist, float* t) {
    float b, d;
    if(sphereQuadratic((Sphere*)self, ray, maxDist, &b, &d)) return 0;
    float minusB = -b;
    if (minusB >= d && minusB < d + maxDist) {
        *t = minusB - d;
        return 1;
    }
    *t = minusB + d;
    return *t >= 0.0f && *t < maxDist;
}

Vec3 Sphere_normal(const Object* obj, Vec3 hitPoint) {
    const Sphere* sphere = (const Sphere*)obj;
    return Vec3_scale(Vec3_sub(hitPoint, sphere->center), sphere->invradius);
}
