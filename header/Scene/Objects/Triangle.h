#ifndef INCLUDE_SCENE_OBJECTS_TRIANGLE_H
#define INCLUDE_SCENE_OBJECTS_TRIANGLE_H

#include "Scene/Objects/Object.h"

typedef struct {
    Vec3  normal;
    geo_t planeD;

    Vec3  edgeNormal0;
    geo_t e0D;
    Vec3  edgeNormal1;
    geo_t e1D;
    Vec3  edgeNormal2;
    geo_t e2D;
} Triangle;

void Triangle_init(Triangle* triangle, Vec3 a, Vec3 b, Vec3 c);

extern const ObjectVTable triangle_vtable;

#endif
