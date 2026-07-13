#ifndef INCLUDE_ENGINE_TRACING_H
#define INCLUDE_ENGINE_TRACING_H

#include "Scene/Scene.h"

#include "Color.h"
#include "HitRecord.h"
#include "Scene/Ray.h"

Color trace(Ray ray, int depth, const Scene* scene);

#endif /* ifndef INCLUDE_ENGINE_TRACING_H */
