#include <gint/display.h>

#include "Scene/Camera.h"
#include "Scene/SceneBuilder.h"
#include "assets/materials.h"

static const Material floor_mat = DEFINE_MATERIAL(COLOR_RGB(0.8, 0.8, 0.8),
                                                  COLOR_FROM_FLOAT(0.1f),
                                                  COLOR_FROM_FLOAT(1.0f),
                                                  COLOR_FROM_FLOAT(0.0f),
                                                  COLOR_FROM_FLOAT(0.05f),
                                                  COLOR_FROM_FLOAT(0.0f),
                                                  1.0f,
                                                  16);

Scene createDemoScene() {
    SceneBuilder builder;
    SceneBuilder_init(&builder);

    Camera camera;
    makeCamera(&camera,
               VEC3_INIT(0, 0, 0),
               VEC3_INIT(0, 0, -5),
               VEC3_INIT(0, 1, 0),
               0.7854f,
               DWIDTH,
               DHEIGHT);

    SceneBuilder_setCamera(&builder, camera);

    // SceneBuilder_addTriangle(&builder,
    //                          &MATERIAL_MATTE,
    //                          VEC3_INIT(-2, 0, -7.5f),
    //                          VEC3_INIT(2, 0, -12),
    //                          VEC3_INIT(0.125f, 2, -8.5f));
    SceneBuilder_addSphere(&builder, VEC3_INIT(0, 1, -8), 1, &MATERIAL_MATTE);
    SceneBuilder_addSphere(&builder, VEC3_INIT(2.25f, 0, -6.5f), 1.25f, &MATERIAL_PLASTIC);
    SceneBuilder_addSphere(&builder, VEC3_INIT(-2, 0, -6), 1, &MATERIAL_POLISHED);
    // SceneBuilder_addTriangle(&builder,
    //                          &floor_mat,
    //                          VEC3_INIT(0, -1, 4),
    //                          VEC3_INIT(500, -1, -500),
    //                          VEC3_INIT(-500, -1, -500));
    SceneBuilder_addSphere(&builder, VEC3_INIT(0, -1001, -5), 1000, &floor_mat);
    SceneBuilder_addSphere(&builder, VEC3_INIT(0.5, 0, -5), 1, &MATERIAL_GLASS);

    SceneBuilder_addVolumeLight(&builder, VEC3_INIT(5, 5, -4), COLOR_RGB(1, 1, 1), 1.0, 3);
    // SceneBuilder_addPointLight(&builder, VEC3_INIT(5, 5, -4), COLOR_RGB(1, 1, 1), 1.0f);
    // SceneBuilder_addDiskAreaLight(&builder, VEC3_INIT(5, 5, -4), COLOR_RGB(1, 1, 1), 1.0, 3);

    return SceneBuilder_build(&builder);
}
