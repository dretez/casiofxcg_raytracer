#include "Engine/Tracing.h"
#include "Engine/Background.h"
#include "Engine/Intersection.h"
#include "Engine/Lighting.h"
#include "Engine/Shading.h"
#include "Vector/FloatingVector.h"

typedef struct TracingContext {
    HitRecord hit;
} TracingContext;

typedef struct {
    Vec3  normal;
    Vec3  refractOrigin;
    Vec3  reflectOrigin;
    float cosI;
    float eta;
} SurfaceInteraction;

Color traceRefraction(Ray incoming, const SurfaceInteraction* si, int depth, const Scene* scene);

Color traceReflection(Ray incoming, const SurfaceInteraction* si, int depth, const Scene* scene);

static inline SurfaceInteraction outsideHit(const HitRecord* hit, float rdoth) {
    return (SurfaceInteraction){
        .normal        = hit->normal,
        .refractOrigin = hit->innerOffset,
        .reflectOrigin = hit->outerOffset,
        .cosI          = -rdoth,
        .eta           = hit->object->material->invior,
    };
}

static inline SurfaceInteraction insideHit(const HitRecord* hit, float rdoth) {
    return (SurfaceInteraction){
        .normal        = Vec3_neg(hit->normal),
        .refractOrigin = hit->outerOffset,
        .reflectOrigin = hit->innerOffset,
        .cosI          = rdoth,
        .eta           = hit->object->material->ior,
    };
}

Color trace(Ray ray, int depth, const Scene* scene) {
    if (depth <= 0) return (Color){ 0 };

    HitRecord hit = intersectScene(ray, scene);
    if (!hit.object) return background(ray);

    Color local     = computeLighting(&hit, ray, scene);
    Color reflected = (Color){ 0 };
    Color refracted = (Color){ 0 };

    const Material* material = hit.object->material;
    if (material->reflectivity || material->transparency) {
        float              rdotn = hit.rdotn;
        SurfaceInteraction si    = rdotn > 0 ? insideHit(&hit, rdotn) : outsideHit(&hit, rdotn);
        if (material->reflectivity) reflected = traceReflection(ray, &si, depth, scene);
        if (material->transparency) refracted = traceRefraction(ray, &si, depth, scene);
    }

    return combineLighting(local, reflected, refracted, material);
}

Color traceRefraction(Ray incoming, const SurfaceInteraction* si, int depth, const Scene* scene) {
    Vec3 direction;
    if (!Vec3_refract(incoming.direction, si->normal, si->cosI, si->eta, &direction))
        return (Color){ 0 };

    Ray ray = (Ray){
        .origin    = si->refractOrigin,
        .direction = direction,
    };
    return trace(ray, depth - 1, scene);
}

Color traceReflection(Ray incoming, const SurfaceInteraction* si, int depth, const Scene* scene) {
    Vec3 reflectionDir = Vec3_add(incoming.direction, Vec3_scale(si->normal, 2.0 * si->cosI));
    Ray  reflectionRay = (Ray){
         .origin    = si->reflectOrigin,
         .direction = reflectionDir,
    };
    return trace(reflectionRay, depth - 1, scene);
}
