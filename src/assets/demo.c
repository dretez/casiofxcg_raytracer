#include <gint/display.h>

#include "Scene/Camera.h"
#include "Scene/SceneBuilder.h"
#include "Vector/FixedVector.h"
#include "assets/materials.h"

static const Material floor_mat = {
    .color        = COLOR_RGB(0.8, 0.8, 0.8),
    .ambient      = COLOR_FROM_FLOAT(0.1f),
    .diffuse      = COLOR_FROM_FLOAT(1.0f),
    .specular     = COLOR_FROM_FLOAT(0.0f),
    .reflectivity = COLOR_FROM_FLOAT(0.05f),
    .transparency = COLOR_FROM_FLOAT(0.0f),
    .ior          = 1.0f,
    .invior       = 1.0f,
    .shininess    = 16,
};

Scene createDemoScene() {
    SceneBuilder builder;
    SceneBuilder_init(&builder);

    Camera camera;
    makeCamera(&camera, vec(0, 0, 0), vec(0, 0, -5), vec(0, 1, 0), 0.7854f, DWIDTH, DHEIGHT);

    SceneBuilder_setCamera(&builder, camera);

    SceneBuilder_addSphere(&builder, vec(0, 1, -7), 1, &MATERIAL_MATTE);
    SceneBuilder_addSphere(&builder, vec(2, 0, -6), 1.25, &MATERIAL_PLASTIC);
    SceneBuilder_addSphere(&builder, vec(-2, 0, -6), 1, &MATERIAL_POLISHED);
    SceneBuilder_addSphere(&builder, vec(0, -1001, -5), 1000, &floor_mat);
    SceneBuilder_addSphere(&builder, vec(0.5, 0, -5), 1, &MATERIAL_GLASS);

    SceneBuilder_addAreaLight(&builder, vec(5, 5, -4), COLOR_RGB(1, 1, 1), 1.0, 3);

    return SceneBuilder_build(&builder);
}
