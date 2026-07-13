#include "Scene/Light/Light.h"
#include "Color.h"
#include "Fixed.h"
#include "Scene/Light/AreaLight.h"
#include "Scene/Light/PointLight.h"

#include "Scene/Light/LightSampler.h"
#include "Vector/FixedVector.h"
#include "Vector/FloatingVector.h"
#include "Vector/Vector.h"
#include <stdlib.h>

#define DISK_AREA_SAMPLE_COUNT 16

static const Vec3 volumeSamples[DISK_AREA_SAMPLE_COUNT] = {
    { 0.34f, 0.21f, 0.11f },   { -0.28f, 0.17f, -0.31f },
    { 0.15f, -0.42f, 0.24f },  { -0.37f, -0.19f, 0.18f },

    { 0.07f, 0.46f, -0.26f },  { -0.11f, -0.33f, -0.41f },
    { 0.42f, -0.08f, -0.17f }, { -0.46f, 0.02f, 0.09f },

    { 0.23f, 0.31f, -0.38f },  { -0.18f, 0.41f, 0.27f },
    { 0.39f, -0.27f, 0.04f },  { -0.24f, -0.11f, 0.43f },

    { 0.02f, -0.48f, -0.09f }, { -0.41f, 0.29f, -0.12f },
    { 0.30f, 0.05f, 0.40f },   { -0.05f, -0.23f, -0.47f }
};

void Light_init( Light* light, LightType type, Vec3 posisition, Color color, float intensity, int samplec) {
    light->type       = type;
    light->pos        = posisition;
    light->color      = Color_scale(color, float2color(intensity));
    light->samplec    = samplec;
    light->invsamplec = uq0_16_from_float(1.0f / samplec);
}

void PointLight_init(PointLight* light, Vec3 posisition, Color color, float intensity) {
    Light_init((Light*)light, LIGHT_POINT, posisition, color, intensity, 1);
}

void AreaLight_init(AreaLight* light, Vec3 position, Color color, float intensity, float radius) {
    Vec3* samples = malloc(sizeof(Vec3) * DISK_AREA_SAMPLE_COUNT);
    if (!samples) return ;
    Light_init((Light*)light, LIGHT_AREA, position, color, intensity, DISK_AREA_SAMPLE_COUNT);
    ((Light*)light)->samples = samples;
    for (int i = 0; i < DISK_AREA_SAMPLE_COUNT; i++)
        ((Light*)light)->samples[i] =
            Vec3_add(position, Vec3_scale(volumeSamples[i], radius));
}
