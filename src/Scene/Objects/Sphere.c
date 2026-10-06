#include "Scene/Objects/Sphere.h"

static int  Sphere_intersect(const ObjectData* self, const Ray* ray, geo_t maxDist);
static int  Sphere_intersectAt(const ObjectData* self, const Ray* ray, geo_t maxDist, geo_t* t);
static Vec3 Sphere_normal(const ObjectData* sphere, Vec3 hitPoint);

const ObjectVTable sphere_vtable = (ObjectVTable){
    .intersect   = Sphere_intersect,
    .intersectAt = Sphere_intersectAt,
    .normal      = Sphere_normal,
};

static int Sphere_intersect(const ObjectData* self, const Ray* ray, geo_t maxDist2) {
    const Sphere* sphere = (const Sphere*)self;

    geo_t ocx = geo_sub_fast(ray->origin.x, sphere->center.x);
    geo_t ocy = geo_sub_fast(ray->origin.y, sphere->center.y);
    geo_t ocz = geo_sub_fast(ray->origin.z, sphere->center.z);

    const Vec3* dir = &ray->direction;
    if (geo_samesign(ocx, dir->x) && geo_samesign(ocy, dir->y) && geo_samesign(ocz, dir->z))
        return 0;
    if ((geo_abs(ocx) > sphere->radius && geo_samesign(ocx, dir->x)) ||
        (geo_abs(ocy) > sphere->radius && geo_samesign(ocy, dir->y)) ||
        (geo_abs(ocz) > sphere->radius && geo_samesign(ocz, dir->z)))
        return 0;

    geo_t b = geo_mul(ocx, dir->x);
    b       = geo_add_fast(b, geo_mul(ocy, dir->y));
    b       = geo_add_fast(b, geo_mul(ocz, dir->z));
    if (b >= GEO_ZERO) return 0;
    geo_t c = geo_mul(ocx, ocx);
    c       = geo_add_fast(c, geo_mul(ocy, ocy));
    c       = geo_add_fast(c, geo_mul(ocz, ocz));
    c       = geo_sub_fast(c, sphere->radius2);
    if (c < GEO_ZERO) return 1;

    geo_t disc = geo_sub_fast(geo_mul(b, b), geo_mul(maxDist2, c));
    if (disc < GEO_ZERO) return 0;

    geo_t limit = geo_sub_fast(-b, maxDist2);
    return limit <= GEO_ZERO || disc > geo_mul(limit, limit);
}

static int Sphere_intersectAt(const ObjectData* self, const Ray* ray, geo_t maxDist, geo_t* t) {
    const Sphere* sphere = (const Sphere*)self;
    const Vec3*   dir    = &ray->direction;

    geo_t ocx = geo_sub_fast(ray->origin.x, sphere->center.x);
    geo_t ocy = geo_sub_fast(ray->origin.y, sphere->center.y);
    geo_t ocz = geo_sub_fast(ray->origin.z, sphere->center.z);

    if (geo_samesign(ocx, dir->x) && geo_samesign(ocy, dir->y) && geo_samesign(ocz, dir->z))
        return 0;
    if ((geo_abs(ocx) > sphere->radius && geo_samesign(ocx, dir->x)) ||
        (geo_abs(ocy) > sphere->radius && geo_samesign(ocy, dir->y)) ||
        (geo_abs(ocz) > sphere->radius && geo_samesign(ocz, dir->z)))
        return 0;

    geo_t b = geo_mul(ocx, dir->x);
    b       = geo_add_fast(b, geo_mul(ocy, dir->y));
    b       = geo_add_fast(b, geo_mul(ocz, dir->z));
    geo_t c = geo_mul(ocx, ocx);
    c       = geo_add_fast(c, geo_mul(ocy, ocy));
    c       = geo_add_fast(c, geo_mul(ocz, ocz));
    c       = geo_sub_fast(c, sphere->radius2);
    if (c >= GEO_ZERO && b > GEO_ZERO) return 0;

    geo_t discriminant = geo_sub_fast(geo_mul(b, b), c);
    if (discriminant < GEO_ZERO) return 0;
    geo_t dLimit = -geo_add_fast(maxDist, b);
    if (dLimit > GEO_ZERO && discriminant <= geo_mul(dLimit, dLimit)) return 0;

    geo_t d      = geo_sqrt(discriminant);
    geo_t minusB = -b;
    if (minusB >= d && minusB < geo_add(d, maxDist)) {
        *t = geo_sub_fast(minusB, d);
        return 1;
    }
    *t = geo_add_fast(minusB, d);
    return *t >= GEO_ZERO && *t < maxDist;
}

static Vec3 Sphere_normal(const ObjectData* obj, Vec3 hitPoint) {
    const Sphere* sphere = (const Sphere*)obj;
    return Vec3_scale(Vec3_sub_fast(hitPoint, sphere->center), sphere->invradius);
}
