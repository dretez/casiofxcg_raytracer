#include <gint/display.h>

#include "Scene/Camera.h"
#include "Scene/SceneBuilder.h"
#include "Vector/FixedVector.h"
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
    makeCamera(&camera, vec(0, 0, 0), vec(0, 0, -5), vec(0, 1, 0), 0.7854f, DWIDTH, DHEIGHT);

    SceneBuilder_setCamera(&builder, camera);

    SceneBuilder_addTriangle(&builder, &MATERIAL_MATTE, vec(-2, 0, -6), vec(2, 0, -7), vec(0, 2, -6));
    // SceneBuilder_addSphere(&builder, vec(0, 1, -7), 1, &MATERIAL_MATTE);
    SceneBuilder_addSphere(&builder, vec(2, 0, -6), 1.25, &MATERIAL_PLASTIC);
    SceneBuilder_addSphere(&builder, vec(-2, 0, -6), 1, &MATERIAL_POLISHED);
    SceneBuilder_addTriangle(&builder, &floor_mat, vec(0, -1, 4), vec(500, -1, -500), vec(-500, -1, -500));
    // SceneBuilder_addSphere(&builder, vec(0, -1001, -5), 1000, &floor_mat);
    SceneBuilder_addSphere(&builder, vec(0.5, 0, -5), 1, &MATERIAL_GLASS);

    SceneBuilder_addVolumeLight(&builder, vec(5, 5, -4), COLOR_RGB(1, 1, 1), 1.0, 3);
    // SceneBuilder_addDiskAreaLight(&builder, vec(5, 5, -4), COLOR_RGB(1, 1, 1), 1.0, 3);

    return SceneBuilder_build(&builder);
}
