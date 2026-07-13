#include "assets/materials.h"
#include "Scene/Objects/Material.h"

const Material MATERIAL_MATTE    = DEFINE_MATERIAL(COLOR_RGB(1.0f, 0.0f, 0.0f),
                                                   COLOR_FROM_FLOAT(0.1f),
                                                   COLOR_FROM_FLOAT(1.0f),
                                                   COLOR_FROM_FLOAT(0.0f),
                                                   COLOR_FROM_FLOAT(0.0f),
                                                   COLOR_FROM_FLOAT(0.0f),
                                                   1.0f,
                                                   16);
const Material MATERIAL_PLASTIC  = DEFINE_MATERIAL(COLOR_RGB(0.2f, 0.6f, 1.0f),
                                                   COLOR_FROM_FLOAT(0.1f),
                                                   COLOR_FROM_FLOAT(1.0f),
                                                   COLOR_FROM_FLOAT(0.4f),
                                                   COLOR_FROM_FLOAT(0.0f),
                                                   COLOR_FROM_FLOAT(0.0f),
                                                   1.0f,
                                                   32);
const Material MATERIAL_POLISHED = DEFINE_MATERIAL(COLOR_RGB(1.0f, 1.0f, 1.0f),
                                                   COLOR_FROM_FLOAT(0.05f),
                                                   COLOR_FROM_FLOAT(0.8f),
                                                   COLOR_FROM_FLOAT(1.0f),
                                                   COLOR_FROM_FLOAT(0.8f),
                                                   COLOR_FROM_FLOAT(0.0f),
                                                   1.0f,
                                                   128);
const Material MATERIAL_GLASS    = DEFINE_MATERIAL(COLOR_RGB(1.0f, 1.0f, 1.0f),
                                                   COLOR_FROM_FLOAT(0.0f),
                                                   COLOR_FROM_FLOAT(0.1f),
                                                   COLOR_FROM_FLOAT(1.0f),
                                                   COLOR_FROM_FLOAT(0.05f),
                                                   COLOR_FROM_FLOAT(0.95f),
                                                   1.458f,
                                                   128);
const Material MATERIAL_MIRROR   = DEFINE_MATERIAL(COLOR_RGB(1.0f, 1.0f, 1.0f),
                                                   COLOR_FROM_FLOAT(0.05f),
                                                   COLOR_FROM_FLOAT(1.0f),
                                                   COLOR_FROM_FLOAT(0.0f),
                                                   COLOR_FROM_FLOAT(0.95f),
                                                   COLOR_FROM_FLOAT(0.0f),
                                                   1.0f,
                                                   128);
