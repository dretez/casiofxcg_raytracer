#include "Engine/Tracing.h"
#include "Engine/Background.h"
#include "Engine/Intersection.h"
#include "Engine/Lighting.h"
#include "Engine/Shading.h"

u32 raycount = 0;

typedef struct {
    Vec3  normal;
    Vec3  refractOrigin;
    Vec3  reflectOrigin;
    geo_t cosI;
    geo_t eta;
} SurfaceInteraction;

Color traceRefraction(const Ray*                incoming,
                      const SurfaceInteraction* si,
                      int                       depth,
                      const Scene*              scene);

Color traceReflection(const Ray*                incoming,
                      const SurfaceInteraction* si,
                      int                       depth,
                      const Scene*              scene);

static inline SurfaceInteraction outsideHit(const HitRecord* hit, geo_t rdoth) {
    return (SurfaceInteraction){
        .normal        = hit->normal,
        .refractOrigin = hit->innerOffset,
        .reflectOrigin = hit->outerOffset,
        .cosI          = -rdoth,
        .eta           = hit->object->material->invior,
    };
}

static inline SurfaceInteraction insideHit(const HitRecord* hit, geo_t rdoth) {
    return (SurfaceInteraction){
        .normal        = Vec3_neg(hit->normal),
        .refractOrigin = hit->outerOffset,
        .reflectOrigin = hit->innerOffset,
        .cosI          = rdoth,
        .eta           = hit->object->material->ior,
    };
}

Color trace(const Ray* restrict ray, const int depth, const Scene* restrict scene) {
    raycount++;
    if (depth <= 0) return (Color){ 0 };

    HitRecord hit = intersectScene(ray, scene);
    if (!hit.object) return background(ray);

    Color local = (Color){ 0 };

    const Material* material = hit.object->material;
    if (material->traceFlags & MATTF_LOCAL) local = computeLighting(&hit, ray, scene);
    Color reflected = (Color){ 0 };
    Color refracted = (Color){ 0 };
    if (material->traceFlags & (MATTF_REFLE | MATTF_REFRA)) {
        SurfaceInteraction si =
            hit.rdotn > GEO_ZERO ? insideHit(&hit, hit.rdotn) : outsideHit(&hit, hit.rdotn);
        if (material->reflectivity) reflected = traceReflection(ray, &si, depth, scene);
        if (material->transparency) refracted = traceRefraction(ray, &si, depth, scene);
    }

    return combineLighting(local, reflected, refracted, material);
}

Color traceRefraction(const Ray* restrict incoming,
                      const SurfaceInteraction* restrict si,
                      const int                 depth,
                      const Scene* restrict scene) {
    Vec3 direction;
    if (!Vec3_refract(incoming->direction, si->normal, si->cosI, si->eta, &direction))
        return (Color){ 0 };

    Ray ray = (Ray){
        .origin    = si->refractOrigin,
        .direction = direction,
    };
    return trace(&ray, depth, scene);
}

Color traceReflection(const Ray* restrict incoming,
                      const SurfaceInteraction* restrict si,
                      const int                 depth,
                      const Scene* restrict scene) {
    Vec3 reflectionDir = Vec3_reflect(incoming->direction, si->normal, si->cosI);
    Ray reflectionRay = (Ray){
        .origin    = si->reflectOrigin,
        .direction = reflectionDir,
    };
    return trace(&reflectionRay, depth - 1, scene);
}
