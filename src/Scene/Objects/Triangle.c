#include "Scene/Objects/Triangle.h"

static int  Triangle_intersect(const ObjectData* self, const Ray* ray, geo_t maxDist2);
static int  Triangle_intersectAt(const ObjectData* self, const Ray* ray, geo_t maxDist, geo_t* t);
static Vec3 Triangle_normal(const ObjectData* self, Vec3 hitPoint);

const ObjectVTable triangle_vtable = {
    .intersect   = Triangle_intersect,
    .intersectAt = Triangle_intersectAt,
    .normal      = Triangle_normal,
};

void Triangle_init(Triangle* tri, Vec3 a, Vec3 b, Vec3 c) {
    Vec3 e0 = Vec3_sub(b, a);
    Vec3 e1 = Vec3_sub(c, b);
    Vec3 e2 = Vec3_sub(a, c);

    Vec3  e0N    = Vec3_normalize(e0);
    Vec3  e1N    = Vec3_normalize(Vec3_sub(c, a));
    Vec3  normal = Vec3_normalize(Vec3_cross(e0N, e1N));
    geo_t d      = Vec3_dot(normal, a);

    tri->normal = normal;
    tri->planeD = d;

    tri->edgeNormal0 = Vec3_cross(normal, e0);
    tri->e0D         = Vec3_dot(tri->edgeNormal0, a);
    tri->edgeNormal1 = Vec3_cross(normal, e1);
    tri->e1D         = Vec3_dot(tri->edgeNormal1, b);
    tri->edgeNormal2 = Vec3_cross(normal, e2);
    tri->e2D         = Vec3_dot(tri->edgeNormal2, c);
}

static int Triangle_intersect(const ObjectData* self, const Ray* ray, geo_t maxDist2) {
    (void)maxDist2; // silence unused argument warning
    const Triangle* tri   = (const Triangle*)self;
    geo_t           denom = Vec3_dot(tri->normal, ray->direction);
    if (geo_abs(denom) < RAY_EPSILON) return 0;
    geo_t numer = geo_sub(tri->planeD, Vec3_dot(tri->normal, ray->origin));
    if (denom > GEO_ZERO) {
        if (numer < GEO_ZERO || numer > denom) return 0;
    } else {
        if (numer > GEO_ZERO || numer < denom) return 0;
    }
    geo_t inv = geo_inv(denom);
    geo_t t   = geo_mul(numer, inv);
    Vec3  p   = Vec3_add(ray->origin, Vec3_scale(ray->direction, t));

    return Vec3_dot(tri->edgeNormal0, p) >= tri->e0D && Vec3_dot(tri->edgeNormal1, p) >= tri->e1D &&
           Vec3_dot(tri->edgeNormal2, p) >= tri->e2D;
}

static int Triangle_intersectAt(const ObjectData* self, const Ray* ray, geo_t maxDist, geo_t* t) {
    const Triangle* tri = (const Triangle*)self;

    geo_t denom = Vec3_dot(tri->normal, ray->direction);
    if (geo_abs(denom) < RAY_EPSILON) return 0;

    geo_t numer = geo_sub(tri->planeD, Vec3_dot(tri->normal, ray->origin));
    geo_t dist  = geo_div(numer, denom);
    if (dist < GEO_ZERO || dist >= maxDist) return 0;

    Vec3 p = Vec3_add(ray->origin, Vec3_scale(ray->direction, dist));
    if (Vec3_dot(tri->edgeNormal0, p) < tri->e0D) return 0;
    if (Vec3_dot(tri->edgeNormal1, p) < tri->e1D) return 0;
    if (Vec3_dot(tri->edgeNormal2, p) < tri->e2D) return 0;
    *t = dist;
    return 1;
}

static Vec3 Triangle_normal(const ObjectData* self, Vec3 hitPoint) {
    (void)hitPoint; // silence unused argument warning
    return ((Triangle*)self)->normal;
}
