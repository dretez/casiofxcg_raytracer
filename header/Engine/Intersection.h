#ifndef INCLUDE_ENGINE_INTERSECTION_H
#define INCLUDE_ENGINE_INTERSECTION_H

#include "Scene/Scene.h"

#include "HitRecord.h"
#include "Scene/Ray.h"

HitRecord intersectScene(Ray ray, const Scene* scene);

#endif /* ifndef INCLUDE_ENGINE_INTERSECTION_H */
