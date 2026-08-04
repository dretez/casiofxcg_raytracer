#include "Engine/Lighting.h"

typedef struct LightingContext {
    const Scene*      scene;
    ColorAccumulator* colAcc;

    Vec3 viewDir;
    Vec3 shadowOrigin;

    Color baseColor;
    geo_t ndotv;
} LightingContext;

i64 shadowTests = 0;
i64 shadowHits  = 0;

color_t shadowTransmission(const HitRecord* hit, Vec3 lightDir, Vec3 shadowOrigin, geo_t lightDist, const Scene* scene);

color_t phongSpecular(geo_t ndotl, geo_t ndotv, Vec3 lightDir, Vec3 viewDir, const Material* material);

void computeSample(const LightingContext* ctx, const Light* light, const Vec3 sample);

Color computeLighting(const HitRecord* hit, const Ray* ray, const Scene* scene) {
    const Material* material = hit->object->material;

    Color           result  = Color_scale(material->color, material->ambient);
    Vec3            viewDir = Vec3_neg(ray->direction);
    LightingContext ctx     = (LightingContext){
        .scene = scene,

        .viewDir      = viewDir,
        .shadowOrigin = hit->outerOffset,

        .ndotv = -hit->rdotn,
    };
    Color baseColor = Color_scale(material->color, material->diffuse);

    for (int i = 0; i < scene->lightCount; i++) {
        Light* light = scene->lights[i];
        light->vtable->initSampler(light, hit->point);
        ColorAccumulator lightContribution = { 0 };

        ctx.baseColor = Color_mul(light->color, baseColor);
        ctx.colAcc    = &lightContribution;

        for (int s = 0; s < light->samplec; s++)
            computeSample(hit, material, &ctx, light, light->vtable->sample(light, s));

        ColorAccumulator_scale(&lightContribution, light->invsamplec);
        result = Color_add(result, ColorAccumulator_toColor(&lightContribution));
    }

    return result;
}

void computeSample(const HitRecord*       hit,
                   const Material*        material,
                   const LightingContext* ctx,
                   const Light*           light,
                   const Vec3             sample) {
    Vec3  offset = Vec3_sub(sample, ctx->hit->point);
    geo_t ndoto  = Vec3_dot(ctx->hit->normal, offset);
    if (ndoto <= GEO_ZERO) return;

    geo_t   distance2 = Vec3_dot(offset, offset);
    color_t shadow = shadowTransmission(hit, offset, ctx->shadowOrigin, distance2, ctx->scene);
    if (!shadow) return;

    geo_t invDistance = geo_rsqrt(distance2);

    geo_t ndotl = geo_mul(ndoto, invDistance);

    color_t diffuse = geo_to_color(geo_mul(color_to_geo(shadow), ndotl));
    ColorAccumulator_addScaled(ctx->colAcc, ctx->baseColor, diffuse);

    if (material->specular) {
        Vec3    lightDir       = Vec3_scale(offset, invDistance);
        color_t specular       = phongSpecular(ndotl, ctx->ndotv, lightDir, ctx->viewDir, material);
        color_t specularWeight = colorMul(specular, shadow);
        ColorAccumulator_addScaled(ctx->colAcc, light->color, specularWeight);
    }
}

color_t shadowTransmission(const HitRecord* hit, Vec3 offset, Vec3 shadowOrigin, geo_t lightDist2, const Scene* scene) {
    const Object* self      = hit->object;
    Ray           shadowRay = (Ray){
                  .origin    = shadowOrigin,
                  .direction = offset,
    };

    color_t transmission = COLOR_ONE;
    for (int i = 0; i < scene->objectCount; i++) {
        const Object* obj = &scene->objects[i];
        if (obj == self) continue;
        shadowTests++;
        if (!obj->vtable->intersect(obj->data, &shadowRay, lightDist2)) continue;
        shadowHits++;
        color_t transparency = obj->material->transparency;
        if (!transparency) return 0;
        transmission = colorMul(transmission, transparency);
        if (transmission < COLOR_FROM_FLOAT(1e-3)) return 0;
    }
    return transmission;
}

color_t phongSpecular(geo_t ndotl, geo_t ndotv, Vec3 lightDir, Vec3 viewDir, const Material* material) {
    geo_t ldotv = Vec3_dot(lightDir, viewDir);
    geo_t rdotv = geo_sub(geo_mul(geo_mul(GEO_TWO, ndotl), ndotv), ldotv);
    if (rdotv <= 0) return 0;

    geo_t baseSpecular = rdotv;
    geo_t specular     = rdotv;
    u16   exp          = material->shininess;
    switch (exp) {
    case 128:
        specular = geo_mul(specular, specular);
        specular = geo_mul(specular, specular);
        __attribute__((fallthrough));
    case 32:
        specular = geo_mul(specular, specular);
        __attribute__((fallthrough));
    case 16:
        specular = geo_mul(specular, specular);
        specular = geo_mul(specular, specular);
        specular = geo_mul(specular, specular);
        specular = geo_mul(specular, specular);
        __attribute__((fallthrough));
    case 1:
        break;
    default:
        exp--;
        while (exp) {
            if (exp & 1) specular = geo_mul(specular, baseSpecular);
            baseSpecular = geo_mul(baseSpecular, baseSpecular);
            exp >>= 1;
        }
        break;
    }

    return colorMul(material->specular, geo_to_color(specular));
}
