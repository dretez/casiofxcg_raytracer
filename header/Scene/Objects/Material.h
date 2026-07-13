#ifndef INCLUDE_SCENE_OBJECTS_MATERIAL_H
#define INCLUDE_SCENE_OBJECTS_MATERIAL_H

#include "Color.h"
#include "utils.h"

typedef struct {
    Color   color;
    color_t localWeight;

    color_t ambient;
    color_t diffuse;

    color_t specular;
    color_t reflectivity;

    color_t transparency;
    float   ior;
    float   invior;

    u16 shininess;
} Material;

#define DEFINE_MATERIAL(                                                                           \
    _color, _ambient, _diffuse, _specular, _reflectivity, _transparency, _ior, _shininess)         \
    (Material) {                                                                                   \
        .color       = _color,                                                                     \
        .localWeight = (u32)_reflectivity + _transparency > (u32)COLOR_ONE ? 0 : COLOR_ONE - _reflectivity - _transparency,                \
        .ambient = _ambient, .diffuse = _diffuse, .specular = _specular,                           \
        .reflectivity = _reflectivity, .transparency = _transparency, .ior = _ior,                 \
        .invior = 1.0f / _ior, .shininess = _shininess,                                            \
    }

#endif /* ifndef INCLUDE_SCENE_OBJECTS_MATERIAL_H */
