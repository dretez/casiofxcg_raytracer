#include "./Light_protected.h"

void Light_init(Light*             light,
                const LightVTable* vtable,
                LightType          type,
                Vec3               posisition,
                Color              color,
                float              intensity,
                int                samplec) {
    light->vtable     = vtable;
    light->type       = type;
    light->pos        = posisition;
    light->color      = Color_scale(color, float2color(intensity));
    light->samplec    = samplec;
    light->invsamplec = uq0_16_from_float(1.0f / samplec);
}
