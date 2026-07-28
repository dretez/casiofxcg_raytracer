#include "Scene/Objects/Triangle.h"
#include "Scene/Ray.h"
#include "Vector/FloatingVector.h"
#include <math.h>

static int  Triangle_intersect(const Object* self, const Ray* ray, float maxDist2);
static int  Triangle_intersectAt(const Object* self, const Ray* ray, float maxDist, float* t);
static Vec3 Triangle_normal(const Object* self, Vec3 hitPoint);

const ObjectVTable triangle_vtable = {
    .intersect   = Triangle_intersect,
    .intersectAt = Triangle_intersectAt,
    .normal      = Triangle_normal,
};

void Triangle_init(Triangle* tri, const Material* material, Vec3 a, Vec3 b, Vec3 c) {
    Vec3 e0 = Vec3_sub(b, a);
    Vec3 e1 = Vec3_sub(c, b);
    Vec3 e2 = Vec3_sub(a, c);

    Vec3  normal = Vec3_normalize(Vec3_cross(e0, Vec3_sub(c, a)));
    float d      = Vec3_dot(normal, a);

    tri->super = (Object){
        .material = material,
        .vtable   = &triangle_vtable,
    };
    tri->v0 = a;
    tri->v1 = b;
    tri->v2 = c;

    tri->normal = normal;
    tri->planeD = d;

    tri->edgeNormal0 = Vec3_cross(normal, e0);
    tri->edgeNormal1 = Vec3_cross(normal, e1);
    tri->edgeNormal2 = Vec3_cross(normal, e2);

    tri->e0D = Vec3_dot(tri->edgeNormal0, tri->v0);
    tri->e1D = Vec3_dot(tri->edgeNormal1, tri->v1);
    tri->e2D = Vec3_dot(tri->edgeNormal2, tri->v2);
}

static int Triangle_intersect(const Object* self, const Ray* ray, float maxDist2) {
    (void)maxDist2; // silence unused argument warning
    const Triangle* tri   = (const Triangle*)self;
    float           denom = Vec3_dot(tri->normal, ray->direction);
    if (fabsf(denom) < RAY_EPSILON) return 0;
    float numer = tri->planeD - Vec3_dot(tri->normal, ray->origin);
    if (denom > 0.0f) {
        if (numer < 0.0f || numer > denom) return 0;
    } else {
        if (numer > 0.0f || numer < denom) return 0;
    }
    float inv = 1.0f / denom;
    float t   = numer * inv;
    Vec3  p   = Vec3_add(ray->origin, Vec3_scale(ray->direction, t));

    return Vec3_dot(tri->edgeNormal0, p) >= tri->e0D && Vec3_dot(tri->edgeNormal1, p) >= tri->e1D &&
           Vec3_dot(tri->edgeNormal2, p) >= tri->e2D;
}

static int Triangle_intersectAt(const Object* self, const Ray* ray, float maxDist, float* t) {
    const Triangle* tri = (const Triangle*)self;

    float denom = Vec3_dot(tri->normal, ray->direction);
    if (fabsf(denom) < RAY_EPSILON) return 0;

    float numer = tri->planeD - Vec3_dot(tri->normal, ray->origin);
    float dist  = numer / denom;
    if (dist < 0.0f || dist >= maxDist) return 0;

    Vec3 p = Vec3_add(ray->origin, Vec3_scale(ray->direction, dist));
    if (Vec3_dot(tri->edgeNormal0, p) < tri->e0D) return 0;
    if (Vec3_dot(tri->edgeNormal1, p) < tri->e1D) return 0;
    if (Vec3_dot(tri->edgeNormal2, p) < tri->e2D) return 0;
    *t = dist;
    return 1;
}

static Vec3 Triangle_normal(const Object* obj, Vec3 hitPoint) {
    (void)hitPoint; // silence unused argument warning
    return ((Triangle*)obj)->normal;
}
