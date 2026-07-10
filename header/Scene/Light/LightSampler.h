#ifndef INCLUDE_LIGHT_LIGHTSAMPLER_H
#define INCLUDE_LIGHT_LIGHTSAMPLER_H

#include "Vector/Vector.h"

struct Light;

typedef struct LightSampler {
    Vec3 center;
    Vec3 u;
    Vec3 v;

    int curSample;
} LightSampler;

void LightSampler_init(const struct Light* light, Vec3 shadingPoint, LightSampler* sampler);
Vec3 LightSampler_sample(const struct Light* light, LightSampler* sampler);

#endif /* ifndef INCLUDE_LIGHT_LIGHTSAMPLER_H */
