#include "Engine/Shading.h"

Color combineLighting(Color local, Color reflected, Color refracted, const Material* material) {
    Color result = Color_add(Color_scale(refracted, material->transparency),
                             Color_scale(reflected, material->reflectivity));
    if (material->localWeight)
        result = Color_add(result, Color_scale(local, material->localWeight));
    return result;
}
