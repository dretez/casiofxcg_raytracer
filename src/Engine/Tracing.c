#include "Engine/Tracing.h"
#include "Engine/Background.h"
#include "Engine/Intersection.h"
#include "Engine/Lighting.h"
#include "Engine/Shading.h"

Color trace(Ray ray, int depth, const Scene* scene) {
    if (depth <= 0) return (Color){ 0 };

    HitRecord hit = intersectScene(ray, scene);
    if (!hit.object) return background(ray);

    Color local     = computeLighting(&hit, ray, scene);
    Color reflected = (Color){ 0 };
    if (hit.object->material->reflectivity > 0) reflected = traceReflection(ray, hit, depth, scene);
    Color refracted = (Color){ 0 };
    if (hit.object->material->transparency > 0) refracted = traceRefraction(ray, hit, depth, scene);
    return combineLighting(local, reflected, refracted, hit.object->material);
}

Color traceRefraction(Ray incoming, HitRecord hit, int depth, const Scene* scene) {
    Ray  ray;
    Vec3 normal = hit.normal.direction;

    float eta = hit.object->material->invior;
    if (Vec3_dot(incoming.direction, normal) > 0) {
        normal = Vec3_neg(normal);
        eta    = hit.object->material->ior;
    }

    Vec3 direction;
    if (!Vec3_refract(incoming.direction, normal, eta, &direction)) return (Color){ 0 };

    ray = (Ray){
        .origin    = Vec3_sub(hit.normal.origin, Vec3_scale(normal, RAY_EPSILON)),
        .direction = direction,
    };
    return trace(ray, depth - 1, scene);
}

Color traceReflection(Ray incoming, HitRecord hit, int depth, const Scene* scene) {
    Vec3 normal = hit.normal.direction;

    if (Vec3_dot(incoming.direction, normal) > 0) normal = Vec3_neg(normal);

    Vec3 reflectionDir = Vec3_reflect(incoming.direction, hit.normal.direction);
    Ray  reflectionRay = (Ray){
         .origin    = Vec3_add(hit.normal.origin, Vec3_scale(normal, RAY_EPSILON)),
         .direction = reflectionDir,
    };
    return trace(reflectionRay, depth - 1, scene);
}
