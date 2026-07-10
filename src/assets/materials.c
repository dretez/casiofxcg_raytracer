#include "assets/materials.h"

const Material MATERIAL_MATTE = {
    .color        = COLOR_RGB(1.0f, 0.0f, 0.0f),
    .ambient      = COLOR_FROM_FLOAT(0.1f),
    .diffuse      = COLOR_FROM_FLOAT(1.0f),
    .specular     = COLOR_FROM_FLOAT(0.0f),
    .reflectivity = COLOR_FROM_FLOAT(0.0f),
    .transparency = COLOR_FROM_FLOAT(0.0f),
    .ior          = 1.0f,
    .invior       = 1.0f,
    .shininess    = 16,
};
const Material MATERIAL_PLASTIC = {
    .color        = COLOR_RGB(0.2f, 0.6f, 1.0f),
    .ambient      = COLOR_FROM_FLOAT(0.1f),
    .diffuse      = COLOR_FROM_FLOAT(1.0f),
    .specular     = COLOR_FROM_FLOAT(0.4f),
    .reflectivity = COLOR_FROM_FLOAT(0.0f),
    .transparency = COLOR_FROM_FLOAT(0.0f),
    .ior          = 1.0f,
    .invior       = 1.0f,
    .shininess    = 32,
};
const Material MATERIAL_POLISHED = {
    .color        = COLOR_RGB(1.0f, 1.0f, 1.0f),
    .ambient      = COLOR_FROM_FLOAT(0.05f),
    .diffuse      = COLOR_FROM_FLOAT(0.8f),
    .specular     = COLOR_FROM_FLOAT(1.0f),
    .reflectivity = COLOR_FROM_FLOAT(0.8f),
    .transparency = COLOR_FROM_FLOAT(0.0f),
    .ior          = 1.0f,
    .invior       = 1.0f,
    .shininess    = 128,
};
const Material MATERIAL_GLASS = {
    .color        = COLOR_RGB(1.0f, 1.0f, 1.0f),
    .ambient      = COLOR_FROM_FLOAT(0.0f),
    .diffuse      = COLOR_FROM_FLOAT(0.1f),
    .specular     = COLOR_FROM_FLOAT(1.0f),
    .reflectivity = COLOR_FROM_FLOAT(0.05f),
    .transparency = COLOR_FROM_FLOAT(0.95f),
    .ior          = 1.458f,
    .invior       = 1.0f / 1.458f,
    .shininess    = 128,
};
const Material MATERIAL_MIRROR = {
    .color        = COLOR_RGB(1.0f, 1.0f, 1.0f),
    .ambient      = COLOR_FROM_FLOAT(0.05f),
    .diffuse      = COLOR_FROM_FLOAT(1.0f),
    .specular     = COLOR_FROM_FLOAT(0.0f),
    .reflectivity = COLOR_FROM_FLOAT(0.95f),
    .transparency = COLOR_FROM_FLOAT(0.0f),
    .ior          = 1.0f,
    .invior       = 1.0f,
    .shininess    = 128,
};
