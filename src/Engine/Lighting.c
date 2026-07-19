#include "Engine/Lighting.h"

#include "Color.h"
#include "Fixed.h"
#include "HitRecord.h"
#include "Scene/Light/Light.h"
#include "Scene/Objects/Material.h"
#include "Scene/Objects/Object.h"
#include "Vector/FloatingVector.h"

typedef struct LightingContext {
    const HitRecord*  hit;
    const Material*   material;
    const Scene*      scene;
    ColorAccumulator* colAcc;

    Vec3 viewDir;
    Vec3 shadowOrigin;

    Color  baseColor;
    float ndotv;
} LightingContext;

i64 shadowTests = 0;
i64 shadowHits  = 0;

color_t shadowTransmission(const HitRecord* hit, Vec3 lightDir, Vec3 shadowOrigin, float lightDist, const Scene* scene);

color_t phongSpecular(float ndotl, float ndotv, Vec3 lightDir, Vec3 viewDir, const Material* material);

void computeSample(const LightingContext* ctx, const Light* light, const Vec3 sample);

Color computeLighting(const HitRecord* hit, Ray ray, const Scene* scene) {
    const Material* material = hit->object->material;

    Color           result  = Color_scale(material->color, material->ambient);
    Vec3            viewDir = Vec3_neg(ray.direction);
    LightingContext ctx     = (LightingContext){
        .hit      = hit,
        .material = hit->object->material,
        .scene    = scene,

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
            computeSample(&ctx, light, light->vtable->sample(light, s));

        ColorAccumulator_scale(&lightContribution, light->invsamplec);
        result = Color_add(result, ColorAccumulator_toColor(&lightContribution));
    }

    return result;
}

void computeSample(const LightingContext* ctx, const Light* light, const Vec3 sample) {
    Vec3  offset = Vec3_sub(sample, ctx->hit->point);
    float ndoto  = Vec3_dot(ctx->hit->normal, offset);
    if (ndoto <= 0) return;

    float   distance2 = Vec3_dot(offset, offset);
    color_t shadow = shadowTransmission(ctx->hit, offset, ctx->shadowOrigin, distance2, ctx->scene);
    if (!shadow) return;

    float invDistance = 1.0f / sqrtf(distance2);

    float ndotl = ndoto * invDistance;

    color_t diffuse  = uq0_16_from_unitfloat(ndotl);
    color_t diffuseWeight  = colorMul(diffuse, shadow);
    ColorAccumulator_addScaled(ctx->colAcc, ctx->baseColor, diffuseWeight);

    if (ctx->material->specular) {
        Vec3 lightDir = Vec3_scale(offset, invDistance);
        color_t specular = phongSpecular(ndotl, ctx->ndotv, lightDir, ctx->viewDir, ctx->material);
        color_t specularWeight = colorMul(specular, shadow);
        ColorAccumulator_addScaled(ctx->colAcc, light->color, specularWeight);
    }
}

color_t shadowTransmission(const HitRecord* hit, Vec3 offset, Vec3 shadowOrigin, float lightDist2, const Scene* scene) {
    const Object* self      = hit->object;
    Ray           shadowRay = (Ray){
                  .origin    = shadowOrigin,
                  .direction = offset,
    };

    color_t transmission = COLOR_ONE;
    for (int i = 0; i < scene->objectCount; i++) {
        const Object* obj = scene->objects[i];
        if (obj == self) continue;
        shadowTests++;
        if (!obj->vtable->intersect(obj, &shadowRay, lightDist2)) continue;
        shadowHits++;
        color_t transparency = obj->material->transparency;
        if (!transparency) return 0;
        transmission = colorMul(transmission, transparency);
        if (transmission < COLOR_FROM_FLOAT(1e-3)) return 0;
    }
    return transmission;
}

color_t phongSpecular(float ndotl, float ndotv, Vec3 lightDir, Vec3 viewDir, const Material* material) {
    float ldotv = Vec3_dot(lightDir, viewDir);
    float rdotv = 2.0f * ndotl * ndotv - ldotv;
    if (rdotv <= 0) return 0;

    float baseSpecular = rdotv;
    float specular     = rdotv;
    u16   exp          = material->shininess;
    switch (exp) {
    case 128:
        specular *= specular;
        specular *= specular;
        __attribute__((fallthrough));
    case 32:
        specular *= specular;
        __attribute__((fallthrough));
    case 16:
        specular *= specular;
        specular *= specular;
        specular *= specular;
        specular *= specular;
        __attribute__((fallthrough));
    case 1:
        break;
    default:
        exp--;
        while (exp) {
            if (exp & 1) specular *= baseSpecular;
            baseSpecular *= baseSpecular;
            exp >>= 1;
        }
        break;
    }

    return colorMul(material->specular, float2color(specular));
}
