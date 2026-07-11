#ifndef INCLUDE_SCENE_OBJECTS_SPHERE_H
#define INCLUDE_SCENE_OBJECTS_SPHERE_H

#include "Scene/Objects/Material.h"
#include "Scene/Ray.h"
#include "Vector/Vector.h"

typedef struct {
    const Material* material;

    Vec3  center;
    float radius;
    float radius2; // radius squared
} Sphere;

int intersectSphere(const Ray* ray, const Sphere* sphere, float maxDist);
int intersectSphereAt(const Ray* ray, const Sphere* sphere, float maxDist, float* t);

#endif /* ifndef INCLUDE_SCENE_OBJECTS_SPHERE_H */
