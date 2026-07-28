#include "Scene/Objects/Sphere.h"
#include "Scene/Ray.h"
#include "Vector/Geometry.h"

#include <math.h>

static int  Sphere_intersect(const Object* self, const Ray* ray, geo_t maxDist);
static int  Sphere_intersectAt(const Object* self, const Ray* ray, geo_t maxDist, geo_t* t);
static Vec3 Sphere_normal(const Object* sphere, Vec3 hitPoint);

const ObjectVTable sphere_vtable = (ObjectVTable){
    .intersect   = Sphere_intersect,
    .intersectAt = Sphere_intersectAt,
    .normal      = Sphere_normal,
};

void Sphere_init(Sphere* sphere, const Material* material, Vec3 center, float radius) {
    sphere->super = (Object){
        .material = material,
        .vtable   = &sphere_vtable,
    };
    sphere->center    = center;
    sphere->radius    = geo_from_float(radius);
    sphere->radius2   = geo_mul(sphere->radius, sphere->radius);
    sphere->invradius = geo_inv(sphere->radius);
}

static int Sphere_intersect(const Object* self, const Ray* ray, geo_t maxDist2) {
    const Sphere* sphere = (Sphere*)self;
    Vec3          oc     = Vec3_sub(ray->origin, sphere->center);

    geo_t b = Vec3_dot(oc, ray->direction);
    if (b >= GEO_ZERO) return 0;
    float c = geo_sub(Vec3_dot(oc, oc), sphere->radius2);
    if (c < GEO_ZERO) return 1;

    geo_t disc = geo_sub(geo_mul(b, b), geo_mul(maxDist2, c));
    if (disc < GEO_ZERO) return 0;

    geo_t limit = geo_sub(-b, maxDist2);
    return limit <= GEO_ZERO || disc > geo_mul(limit, limit);
}

static int Sphere_intersectAt(const Object* self, const Ray* ray, geo_t maxDist, geo_t* t) {
    const Sphere* sphere = (Sphere*)self;
    Vec3          oc     = Vec3_sub(ray->origin, sphere->center);

    geo_t b = Vec3_dot(oc, ray->direction);
    geo_t c = geo_sub(Vec3_dot(oc, oc), sphere->radius2);
    if (c >= GEO_ZERO && b > GEO_ZERO) return 0;

    geo_t discriminant = geo_sub(geo_mul(b, b), c);
    if (discriminant < GEO_ZERO) return 0;
    geo_t dLimit = -geo_add(maxDist, b);
    if (dLimit > GEO_ZERO && discriminant <= geo_mul(dLimit, dLimit)) return 0;

    geo_t d      = geo_sqrt(discriminant);
    geo_t minusB = -b;
    if (minusB >= d && minusB < geo_add(d, maxDist)) {
        *t = geo_sub(minusB, d);
        return 1;
    }
    *t = geo_add(minusB, d);
    return *t >= GEO_ZERO && *t < maxDist;
}

static Vec3 Sphere_normal(const Object* obj, Vec3 hitPoint) {
    const Sphere* sphere = (const Sphere*)obj;
    return Vec3_scale(Vec3_sub(hitPoint, sphere->center), sphere->invradius);
}
