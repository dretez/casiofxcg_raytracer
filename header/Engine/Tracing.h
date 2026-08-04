#ifndef INCLUDE_ENGINE_TRACING_H
#define INCLUDE_ENGINE_TRACING_H

#include "Scene/Scene.h"

#include "Color.h"
#include "HitRecord.h"
#include "Scene/Ray.h"

Color trace(const Ray* ray, const int depth, const Scene* scene);

extern u32 raycount;

#endif /* ifndef INCLUDE_ENGINE_TRACING_H */
