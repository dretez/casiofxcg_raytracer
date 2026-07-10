#include "Engine/Shading.h"

Color combineLighting(Color local, Color reflected, Color refracted, const Material* material) {
    Color   result      = local;
    color_t localWeight = material->reflectivity + material->transparency >= COLOR_ONE
                              ? 0
                              : COLOR_ONE - material->reflectivity - material->transparency;
    result              = Color_scale(result, localWeight);
    result              = Color_add(result, Color_scale(reflected, material->reflectivity));
    result              = Color_add(result, Color_scale(refracted, material->transparency));
    return result;
}
