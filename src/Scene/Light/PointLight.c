#include "Scene/Light/PointLight.h"

static void PointLight_initSampler(Light* self, Vec3 shadingPoint);
static Vec3 PointLight_sample(const Light* self, u16 sample);

const LightVTable pointlight_vtable = {
    .initSampler = PointLight_initSampler,
    .sample      = PointLight_sample,
};

static void PointLight_initSampler(Light* self, Vec3 shadingPoint) {
    (void)self;         // unused
    (void)shadingPoint; // unused
}

static Vec3 PointLight_sample(const Light* self, u16 sample) {
    (void)sample; // unused
    return self->pos;
}
