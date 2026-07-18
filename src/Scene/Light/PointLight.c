#include "Scene/Light/PointLight.h"

#include "./Light_protected.h"

static void PointLight_initSampler(Light* self, Vec3 shadingPoint);
static Vec3 PointLight_sample(const Light* self, u16 sample);

const LightVTable pointlight_vtable = {
    .initSampler = PointLight_initSampler,
    .sample      = PointLight_sample,
};

void PointLight_init(PointLight* light, Vec3 posisition, Color color, float intensity) {
    Light_init((Light*)light, &pointlight_vtable, LIGHT_POINT, posisition, color, intensity, 1);
}

static void PointLight_initSampler(Light* self, Vec3 shadingPoint) {
    (void)self;         // unused
    (void)shadingPoint; // unused
}

static Vec3 PointLight_sample(const Light* self, u16 sample) {
    (void)sample; // unused
    return self->pos;
}
