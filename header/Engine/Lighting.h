#ifndef INCLUDE_ENGINE_LIGHTING_H
#define INCLUDE_ENGINE_LIGHTING_H

#include "Scene/Scene.h"

#include "Color.h"
#include "HitRecord.h"
#include "Scene/Ray.h"

Color computeLighting(const HitRecord* hit, Ray ray, const Scene* scene);

#endif /* ifndef INCLUDE_ENGINE_LIGHTING_H */
