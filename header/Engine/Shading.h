#ifndef INCLUDE_ENGINE_SHADING_H
#define INCLUDE_ENGINE_SHADING_H

#include "Color.h"
#include "Scene/Objects/Material.h"

Color combineLighting(Color local, Color reflected, Color refracted, const Material* material);

#endif /* ifndef INCLUDE_ENGINE_SHADING_H */
