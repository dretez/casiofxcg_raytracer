#ifndef INCLUDE_LIGHT_LIGHT_H
#define INCLUDE_LIGHT_LIGHT_H

#include "Color.h"
#include "Vector.h"

typedef enum {
    LIGHT_POINT,
    LIGHT_AREA,
} LightType;

typedef struct Light {
    LightType type;
    Vec3 pos;
    Color color;

    float invsamples;
    int samples;
} Light;

#endif /* ifndef INCLUDE_LIGHT_LIGHT_H */
