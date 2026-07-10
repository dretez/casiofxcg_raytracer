#ifndef INCLUDE_SCENE_H
#define INCLUDE_SCENE_H

#include "Color.h"
#include "Scene/Camera.h"
#include "Scene/Light/Light.h"
#include "Scene/Objects/Sphere.h"
#include "Scene/Ray.h"

typedef struct {
    Sphere* spheres;
    int sphereCount;

    Light** lights;
    int lightCount;

    Camera camera;
} Scene;

void Scene_free(Scene* scene);

Color trace(Ray ray, int depth, const Scene* scene);

#endif /* ifndef INCLUDE_SCENE_H */
