#ifndef INCLUDE_SCENE_OBJECTS_TRIANGLE_H
#define INCLUDE_SCENE_OBJECTS_TRIANGLE_H

#include "Scene/Objects/Object.h"
#include "Vector/FloatingVector.h"

typedef struct {
    Object super;

    Vec3 v0;
    Vec3 v1;
    Vec3 v2;

    Vec3 edgeNormal0;
    Vec3 edgeNormal1;
    Vec3 edgeNormal2;

    Vec3  normal;
    float planeD;

    float e0D;
    float e1D;
    float e2D;
} Triangle;

void Triangle_init(Triangle* triangle, const Material* material, Vec3 a, Vec3 b, Vec3 c);

extern const ObjectVTable triangle_vtable;

#endif
