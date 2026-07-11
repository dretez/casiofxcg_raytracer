#include "Engine/Shading.h"

Color combineLighting(Color local, Color reflected, Color refracted, const Material* material) {
    Color   result = Color_add(Color_scale(refracted, material->transparency),
                               Color_scale(reflected, material->reflectivity));
    color_t localWeight = colorSub(COLOR_ONE, colorAdd(material->reflectivity, material->transparency));
    if (localWeight) result = Color_add(result, Color_scale(local, localWeight));
    return result;
}
