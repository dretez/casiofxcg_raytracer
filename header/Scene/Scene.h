#ifndef INCLUDE_SCENE_H
#define INCLUDE_SCENE_H

#include "Scene/Camera.h"
#include "Scene/Light/Light.h"
#include "Scene/Objects/Object.h"

typedef struct {
    Object** objects;
    int      objectCount;

    Light** lights;
    int     lightCount;

    Camera camera;
} Scene;

void Scene_free(Scene* scene);

#endif /* ifndef INCLUDE_SCENE_H */
