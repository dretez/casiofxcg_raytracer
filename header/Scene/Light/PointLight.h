#ifndef INCLUDE_LIGHT_POINTLIGHT_H
#define INCLUDE_LIGHT_POINTLIGHT_H

#include "Scene/Light/Light.h"

typedef struct PointLight {
    Light light;
} PointLight;

void PointLight_init(PointLight *light, Vec3 posisition, Color color, float intensity);

#endif /* ifndef INCLUDE_LIGHT_POINTLIGHT_H */
