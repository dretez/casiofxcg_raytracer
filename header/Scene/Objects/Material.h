#ifndef INCLUDE_SCENE_OBJECTS_MATERIAL_H
#define INCLUDE_SCENE_OBJECTS_MATERIAL_H

#include "Color.h"
#include "Vector/Geometry.h"
#include "utils.h"

typedef struct {
    geo_t   ior;
    geo_t   invior;
    Color   color;
    color_t localWeight;

    color_t ambient;
    color_t diffuse;

    color_t specular;
    color_t reflectivity;

    color_t transparency;

    u16 shininess;
    u8 traceFlags;
} Material;


/** Material Tracing flag - Local Lighting */
#define MATTF_LOCAL 0b001
/** Material Tracing flag - Reflection */
#define MATTF_REFLE 0b010
/** Material Tracing flag - Refraction */
#define MATTF_REFRA 0b100

#endif /* ifndef INCLUDE_SCENE_OBJECTS_MATERIAL_H */
