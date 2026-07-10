#include "Engine/Background.h"

Color background(Ray ray) {
    color_t      t     = (float2color(ray.direction.y) + COLOR_ONE) >> 1;
    static Color white = COLOR_RGB(1.0f, 1.0f, 1.0f);
    static Color blue  = COLOR_RGB(0.5f, 0.7f, 1.0f);
    return Color_add(Color_scale(white, COLOR_ONE - t), Color_scale(blue, t));
}
