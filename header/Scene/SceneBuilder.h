#ifndef INCLUDE_SCENEBUILDER_H
#define INCLUDE_SCENEBUILDER_H

#include "Color.h"
#include "Scene/Camera.h"
#include "Scene/Objects/Material.h"
#include "Scene/Objects/Sphere.h"
#include "Scene/Scene.h"
#include "utils.h"

DEFINE_LIST_STRUCT(Sphere, SphereList);
DEFINE_LIST_STRUCT(Light*, LightList);

typedef struct {
    SphereList spheres;
    LightList  lights;
    Camera     camera;
} SceneBuilder;

void SceneBuilder_init(SceneBuilder* builder);
void SceneBuilder_destroy(SceneBuilder* builder);

int SceneBuilder_setCamera(SceneBuilder* builder, Camera camera);
int SceneBuilder_addSphere(SceneBuilder* builder, Vec3 center, float radius, const Material *material);
int SceneBuilder_addPointLight(SceneBuilder* builder, Vec3 posisition, Color color, float intensity);
int SceneBuilder_addAreaLight(SceneBuilder* builder, Vec3 posisition, Color color, float intensity, float radius);

Scene SceneBuilder_build(SceneBuilder* builder);

#endif /* ifndef INCLUDE_SCENEBUILDER_H */
