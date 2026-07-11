#include "Scene/Light/Light.h"
#include "Color.h"
#include "Fixed.h"
#include "Scene/Light/AreaLight.h"
#include "Scene/Light/PointLight.h"

#include "Scene/Light/LightSampler.h"
#include "Vector/Vector.h"

#define DISK_AREA_SAMPLE_COUNT 16

static const Vec2 diskSamples[DISK_AREA_SAMPLE_COUNT] = {
    { 0.13f, 0.07f },  { -0.42f, 0.21f },  { 0.34f, -0.31f }, { -0.15f, -0.44f },
    { 0.48f, 0.12f },  { -0.26f, 0.46f },  { 0.06f, -0.18f }, { -0.49f, -0.08f },
    { 0.29f, 0.40f },  { -0.38f, -0.29f }, { 0.18f, -0.47f }, { -0.04f, 0.33f },
    { 0.43f, -0.05f }, { -0.22f, -0.37f }, { 0.01f, 0.49f },  { -0.31f, 0.15f }
};

void Light_init( Light* light, LightType type, Vec3 posisition, Color color, float intensity, int samples) {
    light->type       = type;
    light->pos        = posisition;
    light->color      = Color_scale(color, float2color(intensity));
    light->samples    = samples;
    light->invsamples = uq0_16_from_float(1.0f / samples);
}

void PointLight_init(PointLight* light, Vec3 posisition, Color color, float intensity) {
    Light_init((Light*)light, LIGHT_POINT, posisition, color, intensity, 1);
}

void AreaLight_init(AreaLight* light, Vec3 posisition, Color color, float intensity, float radius) {
    Light_init((Light*)light, LIGHT_AREA, posisition, color, intensity, DISK_AREA_SAMPLE_COUNT);
    light->radius = radius;
}

Vec3 AreaLight_sample(int sampleIndex, const LightSampler* sampler) {
    float rx = diskSamples[sampleIndex].x;
    float ry = diskSamples[sampleIndex].y;
    return Vec3_add(sampler->center, Vec3_add(Vec3_scale(sampler->u, rx), Vec3_scale(sampler->v, ry)));
}
