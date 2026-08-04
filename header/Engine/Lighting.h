#ifndef INCLUDE_ENGINE_LIGHTING_H
#define INCLUDE_ENGINE_LIGHTING_H

#include "Scene/Scene.h"

#include "Color.h"
#include "HitRecord.h"
#include "Scene/Ray.h"

Color computeLighting(const HitRecord* hit, const Ray* ray, const Scene* scene);

extern i64 shadowTests;
extern i64 shadowHits;

#endif /* ifndef INCLUDE_ENGINE_LIGHTING_H */
