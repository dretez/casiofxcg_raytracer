#include "Engine/Lighting.h"

#include "Scene/Light/Light.h"
#include "Scene/Light/LightSampler.h"
#include "Scene/Objects/Object.h"

void computeSample(ColorAccumulator* lightContribution,
                   const HitRecord*  hit,
                   const Light*      light,
                   LightSampler*     lsampler,
                   Vec3              viewDir,
                   const Material*   material,
                   Color             baseColor,
                   const Scene*      scene) {
    Vec3 sample = LightSampler_sample(light, lsampler);

    Vec3  offset = Vec3_sub(sample, hit->normal.origin);
    float ndoto  = Vec3_dot(hit->normal.direction, offset);
    if (ndoto <= 0) return;

    float distance2   = Vec3_dot(offset, offset);
    float invDistance = 1.0f / sqrtf(distance2);
    float distance    = distance2 * invDistance;

    Vec3 lightDir = Vec3_scale(offset, invDistance);

    color_t shadow = shadowTransmission(hit, lightDir, distance, scene);
    if (!shadow) return;

    color_t diffuse  = float2color(ndoto * invDistance);
    color_t specular = phongSpecular(hit->normal.direction, lightDir, viewDir, material);

    color_t diffuseWeight  = colorMul(material->diffuse, colorMul(diffuse, shadow));
    color_t specularWeight = colorMul(specular, shadow);
    ColorAccumulator_addScaled(lightContribution, baseColor, diffuseWeight);
    ColorAccumulator_addScaled(lightContribution, light->color, specularWeight);
}

Color computeLighting(const HitRecord* hit, Ray ray, const Scene* scene) {
    const Material* material = hit->object->material;

    Vec3  viewDir = Vec3_neg(ray.direction);
    Color result  = Color_scale(material->color, material->ambient);

    for (int i = 0; i < scene->lightCount; i++) {
        Light*       light = scene->lights[i];
        LightSampler lsampler;
        LightSampler_init(light, hit->normal.origin, &lsampler);
        ColorAccumulator lightContribution = { 0 };

        Color lmColor = Color_mul(material->color, light->color);

        for (int s = 0; s < light->samples; s++)
            computeSample(&lightContribution, hit, light, &lsampler, viewDir, material, lmColor, scene);

        ColorAccumulator_scale(&lightContribution, light->invsamples);
        result = Color_add(result, ColorAccumulator_toColor(&lightContribution));
    }

    return result;
}

color_t shadowTransmission(const HitRecord* hit, Vec3 lightDir, float lightDist, const Scene* scene) {
    Vec3 shadowOrigin = Vec3_add(hit->normal.origin, Vec3_scale(hit->normal.direction, RAY_EPSILON));
    Ray shadowRay = (Ray){
        .origin    = shadowOrigin,
        .direction = lightDir,
    };

    color_t transmission = COLOR_ONE;
    for (int i = 0; i < scene->objectCount; i++) {
        const Object* obj = scene->objects[i];
        if (obj == hit->object || !obj->vtable->intersect(obj, &shadowRay, lightDist)) continue;
        if (!obj->material->transparency) return 0;
        transmission = colorMul(transmission, obj->material->transparency);
    }
    return transmission;
}

color_t phongSpecular(Vec3 normal, Vec3 lightDir, Vec3 viewDir, const Material* material) {
    if (!material->specular) return 0;
    Vec3  reflected = Vec3_reflect(Vec3_neg(lightDir), normal);
    float rdotv     = Vec3_dot(reflected, viewDir);
    if (rdotv <= 0) return 0;

    float specular = rdotv;
    u16   exp      = material->shininess;
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
        break;
    default:
        exp--;
        while (exp) {
            if (exp & 1) specular *= rdotv;
            rdotv *= rdotv;
            exp >>= 1;
        }
        break;
    }

    return colorMul(material->specular, float2color(specular));
}
