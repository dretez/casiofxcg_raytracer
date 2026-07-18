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
    { 0.34f, 0.21f, 0.11f },   { -0.28f, 0.17f, -0.31f },
    { 0.15f, -0.42f, 0.24f },  { -0.37f, -0.19f, 0.18f },

    { 0.07f, 0.46f, -0.26f },  { -0.11f, -0.33f, -0.41f },
    { 0.42f, -0.08f, -0.17f }, { -0.46f, 0.02f, 0.09f },

    { 0.23f, 0.31f, -0.38f },  { -0.18f, 0.41f, 0.27f },
    { 0.39f, -0.27f, 0.04f },  { -0.24f, -0.11f, 0.43f },

    { 0.02f, -0.48f, -0.09f }, { -0.41f, 0.29f, -0.12f },
    { 0.30f, 0.05f, 0.40f },   { -0.05f, -0.23f, -0.47f }
};

void VolumeLight_init(VolumeLight* light, Vec3 position, Color color, float intensity, float radius) {
    Vec3* samples = malloc(sizeof(Vec3) * VOLUME_SAMPLE_COUNT);
    if (!samples) return;
    Light_init((Light*)light, &volumelight_vtable, LIGHT_VOLUME, position, color, intensity, VOLUME_SAMPLE_COUNT);
    light->samples = samples;
    for (int i = 0; i < VOLUME_SAMPLE_COUNT; i++)
        light->samples[i] = Vec3_add(position, Vec3_scale(volumeSamples[i], radius));
}

static void VolumeLight_initSampler(Light* self, Vec3 shadingPoint) {
    (void)self;         // unused
    (void)shadingPoint; // unused
}

static Vec3 VolumeLight_sample(const Light* self, u16 sample) {
    return ((VolumeLight*)self)->samples[sample];
}
