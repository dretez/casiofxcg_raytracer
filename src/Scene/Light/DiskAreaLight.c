#include "Scene/Light/DiskAreaLight.h"

#include "./Light_protected.h"
#include "Vector/Geometry.h"

static void DiskAreaLight_initSampler(Light* self, Vec3 shadingPoint);
static Vec3 DiskAreaLight_sample(const Light* self, u16 sample);

const LightVTable diskarealight_vtable = {
    .initSampler = DiskAreaLight_initSampler,
    .sample      = DiskAreaLight_sample,
};

#define DISK_AREA_SAMPLE_COUNT 16

static const Vec2 diskSamples[DISK_AREA_SAMPLE_COUNT] = {
    VEC2_INIT(0.13f, 0.07f),   VEC2_INIT(-0.42f, 0.21f),  VEC2_INIT(0.34f, -0.31f),
    VEC2_INIT(-0.15f, -0.44f), VEC2_INIT(0.48f, 0.12f),   VEC2_INIT(-0.26f, 0.46f),
    VEC2_INIT(0.06f, -0.18f),  VEC2_INIT(-0.49f, -0.08f), VEC2_INIT(0.29f, 0.40f),
    VEC2_INIT(-0.38f, -0.29f), VEC2_INIT(0.18f, -0.47f),  VEC2_INIT(-0.04f, 0.33f),
    VEC2_INIT(0.43f, -0.05f),  VEC2_INIT(-0.22f, -0.37f), VEC2_INIT(0.01f, 0.49f),
    VEC2_INIT(-0.31f, 0.15f)
};

void DiskAreaLight_init(DiskAreaLight* light, Vec3 position, Color color, float intensity, float radius) {
    Light_init(
        (Light*)light, &diskarealight_vtable, position, color, intensity, DISK_AREA_SAMPLE_COUNT);
    light->radius = geo_from_float(radius);
}

static void DiskAreaLight_initSampler(Light* self, Vec3 shadingPoint) {
    DiskAreaLight* light = (DiskAreaLight*)self;

    Vec3  w   = Vec3_normalize(Vec3_sub(self->pos, shadingPoint));
    Vec3  up  = w.y < GEO_ONE ? vec(0, GEO_ONE, 0) : vec(GEO_ONE, 0, 0);
    geo_t inv = up.y ? w.y : w.x;
    inv       = geo_rsqrt(geo_sub(GEO_ONE, geo_mul(inv, inv)));

    light->u = Vec3_scale(up.y ? vec(w.z, 0, -w.x) : vec(0, -w.z, w.y),
                          geo_mul(inv, ((DiskAreaLight*)light)->radius));
    light->v = Vec3_cross(w, light->u);
}

static Vec3 DiskAreaLight_sample(const Light* self, u16 sample) {
    const DiskAreaLight* light = (const DiskAreaLight*)self;
    float                rx    = diskSamples[sample].x;
    float                ry    = diskSamples[sample].y;
    return Vec3_add(self->pos, Vec3_add(Vec3_scale(light->u, rx), Vec3_scale(light->v, ry)));
}
