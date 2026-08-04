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

#define DEFINE_MATERIAL(                                                                           \
    _color, _ambient, _diffuse, _specular, _reflectivity, _transparency, _ior, _shininess)         \
    (Material) {                                                                                   \
        .color       = _color,                                                                     \
        .localWeight = (u32)_reflectivity + _transparency > (u32)COLOR_ONE                         \
                           ? 0                                                                     \
                           : COLOR_ONE - _reflectivity - _transparency,                            \
        .ambient = _ambient, .diffuse = _diffuse, .specular = _specular,                           \
        .reflectivity = _reflectivity, .transparency = _transparency, .ior = GEO_FROM_FLOAT(_ior), \
        .invior = GEO_FROM_FLOAT(1.0f / _ior), .shininess = _shininess,                            \
        .traceFlags = (((_ambient || _diffuse || _specular) ? MATTF_LOCAL : 0) |                   \
                       ((_reflectivity) ? MATTF_REFLE : 0) | ((_transparency) ? MATTF_REFRA : 0))  \
    }

#endif /* ifndef INCLUDE_SCENE_OBJECTS_MATERIAL_H */
