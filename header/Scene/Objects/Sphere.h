#ifndef INCLUDE_SCENE_OBJECTS_SPHERE_H
#define INCLUDE_SCENE_OBJECTS_SPHERE_H

#include "Scene/Objects/Object.h"

typedef struct {
    Vec3  center;
    geo_t radius;
    geo_t radius2; // radius squared
    geo_t invradius;
} Sphere;

extern const ObjectVTable sphere_vtable;

#endif /* ifndef INCLUDE_SCENE_OBJECTS_SPHERE_H */
