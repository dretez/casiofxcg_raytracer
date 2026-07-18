#include "Scene/Light/DiskAreaLight.h"

#include "./Light_protected.h"

static void DiskAreaLight_initSampler(Light* self, Vec3 shadingPoint);
static Vec3 DiskAreaLight_sample(const Light* self, u16 sample);

const LightVTable diskarealight_vtable = {
    .initSampler = DiskAreaLight_initSampler,
    .sample      = DiskAreaLight_sample,
};

#define DISK_AREA_SAMPLE_COUNT 16

static const Vec2 diskSamples[DISK_AREA_SAMPLE_COUNT] = {
    { 0.13f, 0.07f },  { -0.42f, 0.21f },  { 0.34f, -0.31f }, { -0.15f, -0.44f },
    { 0.48f, 0.12f },  { -0.26f, 0.46f },  { 0.06f, -0.18f }, { -0.49f, -0.08f },
    { 0.29f, 0.40f },  { -0.38f, -0.29f }, { 0.18f, -0.47f }, { -0.04f, 0.33f },
    { 0.43f, -0.05f }, { -0.22f, -0.37f }, { 0.01f, 0.49f },  { -0.31f, 0.15f }
};

void DiskAreaLight_init(DiskAreaLight* light, Vec3 position, Color color, float intensity, float radius) {
    Light_init((Light*)light,
               &diskarealight_vtable,
               LIGHT_DISKAREA,
               position,
               color,
               intensity,
               DISK_AREA_SAMPLE_COUNT);
    light->radius = radius;
}

static void DiskAreaLight_initSampler(Light* self, Vec3 shadingPoint) {
    DiskAreaLight* light = (DiskAreaLight*)self;

    Vec3  w   = Vec3_normalize(Vec3_sub(self->pos, shadingPoint));
    Vec3  up  = w.y < 0.99f ? vec(0.0f, 1.0f, 0.0f) : vec(1.0f, 0.0f, 0.0f);
    float inv = 1.0f / sqrtf(1.0f - (up.y ? w.y * w.y : w.x * w.x));

    light->u = Vec3_scale(up.y ? vec(w.z, 0.0f, -w.x) : vec(0.0f, -w.z, w.y),
                          inv * ((DiskAreaLight*)light)->radius);
    light->v = Vec3_cross(w, light->u);
}

static Vec3 DiskAreaLight_sample(const Light* self, u16 sample) {
    const DiskAreaLight* light = (const DiskAreaLight*)self;
    float            rx    = diskSamples[sample].x;
    float            ry    = diskSamples[sample].y;
    return Vec3_add(self->pos, Vec3_add(Vec3_scale(light->u, rx), Vec3_scale(light->v, ry)));
}
