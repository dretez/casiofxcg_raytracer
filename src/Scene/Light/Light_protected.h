#ifndef SRC_SCENE_LIGHT_LIGHTPROTECTED_H
#define SRC_SCENE_LIGHT_LIGHTPROTECTED_H

#include "Scene/Light/Light.h"

void Light_init(Light*             light,
                const LightVTable* vtable,
                LightType          type,
                Vec3               posisition,
                Color              color,
                float              intensity,
                int                samplec);

#endif /* ifndef SRC_SCENE_LIGHT_LIGHTPROTECTED_H */
