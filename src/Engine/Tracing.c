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
    Color refracted = (Color){ 0 };

    const Material* material = hit.object->material;
    if (material->reflectivity > 0 || material->transparency > 0) {
        float rdoth = Vec3_dot(ray.direction, hit.normal.direction);
        float eta = hit.object->material->invior;
        if (rdoth > 0) {
            hit.normal.direction = Vec3_neg(hit.normal.direction);
            eta = hit.object->material->ior;
        }
        if (hit.object->material->reflectivity > 0)
            reflected = traceReflection(ray, hit, depth, scene);
        if (hit.object->material->transparency > 0)
            refracted = traceRefraction(ray, hit, eta, depth, scene);
    }

    return combineLighting(local, reflected, refracted, material);
}

Color traceRefraction(Ray incoming, HitRecord hit, float eta, int depth, const Scene* scene) {
    Vec3 direction;
    if (!Vec3_refract(incoming.direction, hit.normal.direction, eta, &direction))
        return (Color){ 0 };

    Ray ray = (Ray){
        .origin    = Vec3_sub(hit.normal.origin, Vec3_scale(hit.normal.direction, RAY_EPSILON)),
        .direction = direction,
    };
    return trace(ray, depth - 1, scene);
}

Color traceReflection(Ray incoming, HitRecord hit, int depth, const Scene* scene) {
    Vec3 reflectionDir = Vec3_reflect(incoming.direction, hit.normal.direction);
    Ray  reflectionRay = (Ray){
         .origin    = Vec3_add(hit.normal.origin, Vec3_scale(hit.normal.direction, RAY_EPSILON)),
         .direction = reflectionDir,
    };
    return trace(reflectionRay, depth - 1, scene);
}
