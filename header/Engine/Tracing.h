#ifndef INCLUDE_ENGINE_TRACING_H
#define INCLUDE_ENGINE_TRACING_H

#include "Scene/Scene.h"

#include "Color.h"
#include "HitRecord.h"
#include "Scene/Ray.h"

Color trace(Ray ray, int depth, const Scene* scene);

Color traceRefraction(Ray incoming, HitRecord hit, float eta, int depth, const Scene* scene);

Color traceReflection(Ray incoming, HitRecord hit, int depth, const Scene* scene);

#endif /* ifndef INCLUDE_ENGINE_TRACING_H */
