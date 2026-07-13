#include "Engine/Lighting.h"

#include "Color.h"
#include "Fixed.h"
#include "HitRecord.h"
#include "Scene/Light/Light.h"
#include "Scene/Light/LightSampler.h"
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

    Color baseColor;
    uq0_16 ndotv;
} LightingContext;

color_t shadowTransmission( const HitRecord* hit, Vec3 lightDir, Vec3 shadowOrigin, float lightDist, const Scene* scene);

color_t phongSpecular(uq0_16 ndotl, uq0_16 ndotv, Vec3 lightDir, Vec3 viewDir, const Material* material);

void computeSample(const LightingContext* ctx,
                   const Light*          light,
                   LightSampler*         lsampler);

Color computeLighting(const HitRecord* hit, Ray ray, const Scene* scene) {
    const Material* material = hit->object->material;

    Color          result  = Color_scale(material->color, material->ambient);
    Vec3           viewDir = Vec3_neg(ray.direction);
    LightingContext ctx     = (LightingContext){
        .hit      = hit,
        .material = hit->object->material,
        .scene    = scene,

        .viewDir      = viewDir,
        .shadowOrigin = hit->outerOffset,

        .ndotv = -uq0_16_from_unitfloat(hit->rdotn),
    };
    Color baseColor = Color_scale(material->color, material->diffuse);

    for (int i = 0; i < scene->lightCount; i++) {
        Light*       light = scene->lights[i];
        LightSampler lsampler;
        LightSampler_init(light, hit->point, &lsampler);
        ColorAccumulator lightContribution = { 0 };

        ctx.baseColor = Color_mul(light->color, baseColor);
        ctx.colAcc    = &lightContribution;

        for (int s = 0; s < light->samplec; s++) computeSample(&ctx, light, &lsampler);

        ColorAccumulator_scale(&lightContribution, light->invsamplec);
        result = Color_add(result, ColorAccumulator_toColor(&lightContribution));
    }

    return result;
}

color_t shadowTransmission(const HitRecord* hit, Vec3 lightDir, Vec3 shadowOrigin, float lightDist, const Scene* scene) {
    const Object* self = hit->object;
    Ray shadowRay = (Ray){
        .origin    = shadowOrigin,
        .direction = lightDir,
    };

    color_t transmission = COLOR_ONE;
    for (int i = 0; i < scene->objectCount; i++) {
        const Object* obj = scene->objects[i];
        if (obj == self || !obj->vtable->intersect(obj, &shadowRay, lightDist)) continue;
        color_t transparency = obj->material->transparency;
        if (!transparency) return 0;
        transmission = colorMul(transmission, transparency);
        if (transmission < COLOR_FROM_FLOAT(1e-3)) return 0;
    }
    return transmission;
}

color_t phongSpecular(uq0_16 ndotl, uq0_16 ndotv, Vec3 lightDir, Vec3 viewDir, const Material* material) {
    float ldotv = Vec3_dot(lightDir, viewDir);
    uq0_16 rdotv = (uq0_16_mul(ndotl, ndotv) - uq0_16_from_float(ldotv)) << 1;
    if (rdotv <= 0) return 0;

    uq0_16 baseSpecular = rdotv;
    uq0_16 specular = baseSpecular;
    u16   exp      = material->shininess;
    switch (exp) {
    case 128:
        specular = uq0_16_mul(specular, specular);
        specular = uq0_16_mul(specular, specular);
        __attribute__((fallthrough));
    case 32:
        specular = uq0_16_mul(specular, specular);
        __attribute__((fallthrough));
    case 16:
        specular = uq0_16_mul(specular, specular);
        specular = uq0_16_mul(specular, specular);
        specular = uq0_16_mul(specular, specular);
        specular = uq0_16_mul(specular, specular);
        break;
    default:
        exp--;
        while (exp) {
            if (exp & 1) specular = uq0_16_mul(specular, baseSpecular);
            baseSpecular = uq0_16_mul(baseSpecular, baseSpecular);
            exp >>= 1;
        }
        break;
    }

    return colorMul(material->specular, specular);
}

void computeSample(const LightingContext* ctx,
                   const Light*          light,
                   LightSampler*         lsampler) {
    Vec3 sample = LightSampler_sample(light, lsampler);

    Vec3  offset = Vec3_sub(sample, ctx->hit->point);
    float ndoto  = Vec3_dot(ctx->hit->normal, offset);
    if (ndoto <= 0) return;

    float distance2   = Vec3_dot(offset, offset);
    float invDistance = 1.0f / sqrtf(distance2);
    float distance    = distance2 * invDistance;

    Vec3 lightDir = Vec3_scale(offset, invDistance);

    color_t shadow = shadowTransmission(ctx->hit, lightDir, ctx->shadowOrigin, distance, ctx->scene);
    if (!shadow) return;

    float   ndotl    = uq0_16_from_unitfloat(ndoto * invDistance);
    color_t diffuse  = ndotl;
    color_t specular = 0;
    if (ctx->material->specular)
        specular = phongSpecular(ndotl, ctx->ndotv, lightDir, ctx->viewDir, ctx->material);

    color_t diffuseWeight  = colorMul(diffuse, shadow);
    color_t specularWeight = colorMul(specular, shadow);
    ColorAccumulator_addScaled(ctx->colAcc, ctx->baseColor, diffuseWeight);
    ColorAccumulator_addScaled(ctx->colAcc, light->color, specularWeight);
}
