#include "Scene/Scene.h"
#include "Scene/Camera.h"

#include <stdlib.h>

void Scene_free(Scene* scene) {
    free(scene->spheres);
    for (int i = 0; i < scene->lightCount; i++) free(scene->lights[i]);
    free(scene->lights);
    Camera_free(&scene->camera);
}
