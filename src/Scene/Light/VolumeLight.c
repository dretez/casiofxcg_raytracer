#include "Scene/Light/VolumeLight.h"

#include "./Light_protected.h"

#include <stdlib.h>

static void VolumeLight_initSampler(Light* self, Vec3 shadingPoint);
static Vec3 VolumeLight_sample(const Light* self, u16 sample);

const LightVTable volumelight_vtable = {
    .initSampler = VolumeLight_initSampler,
    .sample      = VolumeLight_sample,
};

#define VOLUME_SAMPLE_COUNT 16

static const Vec3 volumeSamples[VOLUME_SAMPLE_COUNT] = {
    VEC3_INIT(0.34f, 0.21f, 0.11f),   VEC3_INIT(-0.28f, 0.17f, -0.31f),
    VEC3_INIT(0.15f, -0.42f, 0.24f),  VEC3_INIT(-0.37f, -0.19f, 0.18f),

    VEC3_INIT(0.07f, 0.46f, -0.26f),  VEC3_INIT(-0.11f, -0.33f, -0.41f),
    VEC3_INIT(0.42f, -0.08f, -0.17f), VEC3_INIT(-0.46f, 0.02f, 0.09f),

    VEC3_INIT(0.23f, 0.31f, -0.38f),  VEC3_INIT(-0.18f, 0.41f, 0.27f),
    VEC3_INIT(0.39f, -0.27f, 0.04f),  VEC3_INIT(-0.24f, -0.11f, 0.43f),

    VEC3_INIT(0.02f, -0.48f, -0.09f), VEC3_INIT(-0.41f, 0.29f, -0.12f),
    VEC3_INIT(0.30f, 0.05f, 0.40f),   VEC3_INIT(-0.05f, -0.23f, -0.47f)
};

void VolumeLight_init(VolumeLight* light, Vec3 position, Color color, float intensity, float radius) {
    Vec3* samples = malloc(sizeof(Vec3) * VOLUME_SAMPLE_COUNT);
    if (!samples) return;
    Light_init((Light*)light, &volumelight_vtable, position, color, intensity, VOLUME_SAMPLE_COUNT);
    light->samples = samples;
    geo_t rad      = geo_from_float(radius);
    for (int i = 0; i < VOLUME_SAMPLE_COUNT; i++)
        light->samples[i] = Vec3_addScaled(position, volumeSamples[i], rad);
}

static void VolumeLight_initSampler(Light* self, Vec3 shadingPoint) {
    (void)self;         // unused
    (void)shadingPoint; // unused
}

static Vec3 VolumeLight_sample(const Light* self, u16 sample) {
    return ((VolumeLight*)self)->samples[sample];
}
