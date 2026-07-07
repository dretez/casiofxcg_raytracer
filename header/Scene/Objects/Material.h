#ifndef INCLUDE_SCENE_OBJECTS_MATERIAL_H
#define INCLUDE_SCENE_OBJECTS_MATERIAL_H

#include "Color.h"
#include "utils.h"

typedef struct {
    Color color;

    color_t ambient;
    color_t diffuse;

    color_t specular;
    color_t reflectivity;

    color_t transparency;
    float   ior;
    float   invior;

    u16 shininess;
} Material;

#endif /* ifndef INCLUDE_SCENE_OBJECTS_MATERIAL_H */
