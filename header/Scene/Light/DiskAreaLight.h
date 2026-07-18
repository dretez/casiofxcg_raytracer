#ifndef INCLUDE_SCENE_LIGHT_AREALIGHT_H
#define INCLUDE_SCENE_LIGHT_AREALIGHT_H

#include "Scene/Light/Light.h"

typedef struct DiskAreaLight {
    Light light;
    Vec3 u;
    Vec3 v;
    float radius;
} DiskAreaLight;

void DiskAreaLight_init(DiskAreaLight *light, Vec3 position, Color color, float intensity, float radius);

#endif /* ifndef INCLUDE_SCENE_LIGHT_AREALIGHT_H */
