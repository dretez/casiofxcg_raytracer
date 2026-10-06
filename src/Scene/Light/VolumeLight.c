#include "Scene/Light/VolumeLight.h"

static void VolumeLight_initSampler(Light* self, Vec3 shadingPoint);
static Vec3 VolumeLight_sample(const Light* self, u16 sample);

const LightVTable volumelight_vtable = {
    .initSampler = VolumeLight_initSampler,
    .sample      = VolumeLight_sample,
};

static void VolumeLight_initSampler(Light* self, Vec3 shadingPoint) {
    (void)self;         // unused
    (void)shadingPoint; // unused
}

static Vec3 VolumeLight_sample(const Light* self, u16 sample) {
    return ((VolumeLight*)self)->samples[sample];
}
