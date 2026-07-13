#include "Scene/Light/LightSampler.h"

#include "Scene/Light/Light.h"
#include "Vector/Vector.h"

#include <math.h>

void LightSampler_init(const Light* light, Vec3 shadingPoint, LightSampler* sampler) {
    sampler->curSample = 0;
}

Vec3 LightSampler_sample(const Light* light, LightSampler* sampler) {
    switch (light->type) {
    case LIGHT_POINT:
        return light->pos;
    case LIGHT_AREA:
        return light->samples[sampler->curSample++];
    }
    return (Vec3){ 0 };
}
