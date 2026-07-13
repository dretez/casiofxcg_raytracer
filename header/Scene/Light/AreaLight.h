#ifndef INCLUDE_LIGHT_AREALIGHT_H
#define INCLUDE_LIGHT_AREALIGHT_H

#include "Scene/Light/Light.h"

typedef struct AreaLight {
    Light light;
    float radius;
} AreaLight;

void AreaLight_init(AreaLight *light, Vec3 posisition, Color color, float intensity, float radius);
// Vec3 AreaLight_sample(int sampleIndex, const LightSampler* sampler);

#endif /* ifndef INCLUDE_LIGHT_AREALIGHT_H */
