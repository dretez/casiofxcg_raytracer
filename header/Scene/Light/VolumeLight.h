#ifndef INCLUDE_SCENE_LIGHT_VOLUMELIGHT_H
#define INCLUDE_SCENE_LIGHT_VOLUMELIGHT_H

#include "Scene/Light/Light.h"

typedef struct VolumeLight {
    Light  light;
    Vec3*  samples;
    geo_t radius;
} VolumeLight;

void VolumeLight_init( VolumeLight* light, Vec3 position, Color color, float intensity, float radius);

#endif /* ifndef INCLUDE_SCENE_LIGHT_VOLUMELIGHT_H */
