#include <gint/display.h>

#include "Scene/Camera.h"
#include "Scene/SceneBuilder.h"
#include "assets/materials.h"

static const Material white = DEFINE_MATERIAL(COLOR_RGB(0.90f, 0.90f, 0.90f),
                                              COLOR_FROM_FLOAT(0.25f),
                                              COLOR_FROM_FLOAT(0.9f),
                                              COLOR_FROM_FLOAT(0.0f),
                                              COLOR_FROM_FLOAT(0.0f),
                                              COLOR_FROM_FLOAT(0.0f),
                                              1.0f,
                                              16);

static const Material red = DEFINE_MATERIAL(COLOR_RGB(0.97f, 0.07f, 0.07f),
                                            COLOR_FROM_FLOAT(0.1f),
                                            COLOR_FROM_FLOAT(0.9f),
                                            COLOR_FROM_FLOAT(0.0f),
                                            COLOR_FROM_FLOAT(0.0f),
                                            COLOR_FROM_FLOAT(0.0f),
                                            1.0f,
                                            16);

static const Material green = DEFINE_MATERIAL(COLOR_RGB(0.11f, 0.96f, 0.02f),
                                              COLOR_FROM_FLOAT(0.1f),
                                              COLOR_FROM_FLOAT(0.9f),
                                              COLOR_FROM_FLOAT(0.0f),
                                              COLOR_FROM_FLOAT(0.0f),
                                              COLOR_FROM_FLOAT(0.0f),
                                              1.0f,
                                              16);

static const Material blue = DEFINE_MATERIAL(COLOR_RGB(0.07f, 0.26f, 0.97f),
                                             COLOR_FROM_FLOAT(0.1f),
                                             COLOR_FROM_FLOAT(0.9f),
                                             COLOR_FROM_FLOAT(0.0f),
                                             COLOR_FROM_FLOAT(0.0f),
                                             COLOR_FROM_FLOAT(0.0f),
                                             1.0f,
                                             16);

Scene createCornellBoxScene() {
    SceneBuilder builder;
    SceneBuilder_init(&builder);

    Camera camera;
    makeCamera(&camera,
               VEC3_INIT(0, 0, 0),
               VEC3_INIT(0, 0, -1),
               VEC3_INIT(0, 1, 0),
               1.570796f,
               DWIDTH,
               DHEIGHT);

    SceneBuilder_setCamera(&builder, camera);

    const float L  = 3.0f;
    const float Z0 = 0.1f;
    const float Z1 = -5.9f;

    // Floor
    SceneBuilder_addTriangle(
        &builder, &white, VEC3_INIT(-L, -L, Z0), VEC3_INIT(L, -L, Z0), VEC3_INIT(L, -L, Z1));
    SceneBuilder_addTriangle(
        &builder, &white, VEC3_INIT(-L, -L, Z0), VEC3_INIT(L, -L, Z1), VEC3_INIT(-L, -L, Z1));

    // Ceiling
    SceneBuilder_addTriangle(
        &builder, &white, VEC3_INIT(-L, L, Z0), VEC3_INIT(L, L, Z1), VEC3_INIT(L, L, Z0));
    SceneBuilder_addTriangle(
        &builder, &white, VEC3_INIT(-L, L, Z0), VEC3_INIT(-L, L, Z1), VEC3_INIT(L, L, Z1));

    // Back wall
    SceneBuilder_addTriangle(
        &builder, &white, VEC3_INIT(-L, -L, Z1), VEC3_INIT(L, -L, Z1), VEC3_INIT(L, L, Z1));
    SceneBuilder_addTriangle(
        &builder, &white, VEC3_INIT(-L, -L, Z1), VEC3_INIT(L, L, Z1), VEC3_INIT(-L, L, Z1));

    // Front wall
    SceneBuilder_addTriangle(
        &builder, &blue, VEC3_INIT(L, L, Z0), VEC3_INIT(L, -L, Z0), VEC3_INIT(-L, -L, Z0));
    SceneBuilder_addTriangle(
        &builder, &blue, VEC3_INIT(-L, L, Z0), VEC3_INIT(L, L, Z0), VEC3_INIT(-L, -L, Z0));

    // Left wall
    SceneBuilder_addTriangle(
        &builder, &red, VEC3_INIT(-L, -L, Z0), VEC3_INIT(-L, -L, Z1), VEC3_INIT(-L, L, Z1));
    SceneBuilder_addTriangle(
        &builder, &red, VEC3_INIT(-L, -L, Z0), VEC3_INIT(-L, L, Z1), VEC3_INIT(-L, L, Z0));

    // Right wall
    SceneBuilder_addTriangle(
        &builder, &green, VEC3_INIT(L, -L, Z0), VEC3_INIT(L, L, Z1), VEC3_INIT(L, -L, Z1));
    SceneBuilder_addTriangle(
        &builder, &green, VEC3_INIT(L, -L, Z0), VEC3_INIT(L, L, Z0), VEC3_INIT(L, L, Z1));

    SceneBuilder_addSphere(&builder, VEC3_INIT(-1.5f, -2.0f, -3.2f), 1.0f, &MATERIAL_POLISHED);
    SceneBuilder_addSphere(&builder, VEC3_INIT(1.7f, -2.0f, -4.4f), 1.0f, &MATERIAL_MATTE);

    // SceneBuilder_addPointLight(&builder, VEC3_INIT(0.0f, 2.6f, -3.0f), COLOR_RGB(1, 1, 1), 1.0f);
    SceneBuilder_addVolumeLight(&builder,
                                VEC3_INIT(0.0f, 2.6f, -3.0f),
                                COLOR_RGB(1,1,1),
                                1.5f,
                                1.0f);

    return SceneBuilder_build(&builder);
}
