#include "Scene/Objects/Sphere.h"
#include "Vector.h"

#include <math.h>

#define calc_b_d                                                                                   \
    Vec3 oc = Vec3_sub(ray->origin, sphere->center);                                               \
                                                                                                   \
    float b = Vec3_dot(oc, ray->direction);                                                        \
    float c = Vec3_dot(oc, oc) - sphere->radius2;                                                  \
                                                                                                   \
    float discriminant = b * b - c;                                                                \
    if (discriminant < 0.0f) return 0;                                                             \
    float dLimit = -(maxDist + b);                                                                 \
    if (dLimit > 0.0f && discriminant < dLimit * dLimit) return 0;                                 \
                                                                                                   \
    float d = sqrtf(discriminant);

int intersectSphere(const Ray* ray, const Sphere* sphere, float maxDist) {
    calc_b_d;
    if (b < d) return 1;
    return d >= b && d - b < maxDist;
}

int intersectSphereAt(const Ray* ray, const Sphere* sphere, float maxDist, float* t) {
    calc_b_d;
    if (-b >= d && -b < d + maxDist) {
        *t = -b - d;
        return 1;
    }
    *t = -b + d;
    return *t >= 0.0f && *t < maxDist;
}
