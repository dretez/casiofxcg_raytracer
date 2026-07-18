#include "Scene/SceneBuilder.h"

#include <stdlib.h>

#include "Scene/Camera.h"
#include "Scene/Light/DiskAreaLight.h"
#include "Scene/Light/Light.h"
#include "Scene/Light/PointLight.h"
#include "Scene/Light/VolumeLight.h"
#include "Scene/Objects/Object.h"
#include "Scene/Objects/Sphere.h"
#include "Scene/Objects/Triangle.h"
#include "utils.h"

void SceneBuilder_init(SceneBuilder* builder) {
    builder->objects = LIST_INITIALIZE(ObjectList);
    builder->lights  = LIST_INITIALIZE(LightList);
}

void SceneBuilder_destroy(SceneBuilder* builder) {
    list_free(builder->objects, ObjectList);
    list_free(builder->lights, LightList);
}

int SceneBuilder_setCamera(SceneBuilder* builder, Camera camera) {
    builder->camera = camera;
    return 0;
}

int SceneBuilder_addSphere(SceneBuilder*   builder,
                           Vec3            center,
                           float           radius,
                           const Material* material) {
    Sphere* sphere = malloc(sizeof(Sphere));
    if (!sphere) return 1;
    Sphere_init(sphere, material, center, radius);
    list_add(builder->objects, Object*, (Object*)sphere, 1);
    return 0;
}

int SceneBuilder_addTriangle(SceneBuilder* builder, const Material* material, Vec3 a, Vec3 b, Vec3 c) {
    Triangle* tri = malloc(sizeof(Triangle));
    if (!tri) return 1;
    Triangle_init(tri, material, a, b, c);
    list_add(builder->objects, Object*, (Object*)tri, 1);
    return 0;
}

int SceneBuilder_addPointLight(SceneBuilder* builder,
                               Vec3          posisition,
                               Color         color,
                               float         intensity) {
    PointLight* pointLight = malloc(sizeof(PointLight));
    if (pointLight == NULL) return 1;
    PointLight_init(pointLight, posisition, color, intensity);
    list_add(builder->lights, Light*, (Light*)pointLight, 1);
    return 0;
}

int SceneBuilder_addVolumeLight(SceneBuilder* builder, Vec3 posisition, Color color, float intensity, float radius) {
    VolumeLight* light = malloc(sizeof(VolumeLight));
    if (light == NULL) return 1;
    VolumeLight_init(light, posisition, color, intensity, radius);
    list_add(builder->lights, Light*, (Light*)light, 1);
    return 0;
}

int SceneBuilder_addDiskAreaLight(SceneBuilder* builder, Vec3 posisition, Color color, float intensity, float radius) {
    DiskAreaLight* light = malloc(sizeof(DiskAreaLight));
    if (light == NULL) return 1;
    DiskAreaLight_init(light, posisition, color, intensity, radius);
    list_add(builder->lights, Light*, (Light*)light, 1);
    return 0;
}

Scene SceneBuilder_build(SceneBuilder* builder) {
    return (Scene){
        .camera      = builder->camera,
        .objects     = builder->objects.items,
        .objectCount = builder->objects.count,
        .lights      = builder->lights.items,
        .lightCount  = builder->lights.count,
    };
}
