#ifndef INCLUDE_SCENE_OBJECTS_SPHERE_H
#define INCLUDE_SCENE_OBJECTS_SPHERE_H

#include "Scene/Objects/Object.h"
#include "Vector/Vector.h"

typedef struct {
    Object super;
    Vec3   center;
    float  radius;
    float  radius2; // radius squared
    float  invradius;
} Sphere;

void Sphere_init(Sphere* sphere, const Material* material, Vec3 center, float radius);

extern const ObjectVTable sphere_vtable;

#endif /* ifndef INCLUDE_SCENE_OBJECTS_SPHERE_H */
