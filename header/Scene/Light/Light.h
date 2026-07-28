#ifndef INCLUDE_LIGHT_LIGHT_H
#define INCLUDE_LIGHT_LIGHT_H

#include "Color.h"
#include "Fixed.h"
#include "Vector/Vector.h"

typedef struct Light Light;

typedef void (*Light_initSampler)(Light* self, Vec3 shadingPoint);
typedef Vec3 (*Light_sample)(const Light* self, u16 sampleNum);

typedef struct LightVTable {
    const Light_initSampler initSampler;
    const Light_sample sample;
} LightVTable;

typedef struct Light {
    const LightVTable* vtable;
    Vec3 pos;
    Color color;

    uq0_16 invsamplec;
    u16 samplec;
} Light;

#endif /* ifndef INCLUDE_LIGHT_LIGHT_H */
