#ifndef INCLUDE_ENGINE_LIGHTING_H
#define INCLUDE_ENGINE_LIGHTING_H

#include "Scene/Scene.h"

#include "Color.h"
#include "HitRecord.h"
#include "Scene/Objects/Material.h"
#include "Scene/Ray.h"

Color computeLighting(const HitRecord* hit, Ray ray, const Scene* scene);

color_t shadowTransmission(const HitRecord* hit, Vec3 lightDir, float lightDist, const Scene* scene);

color_t phongSpecular(Vec3 normal, Vec3 lightDir, Vec3 viewDir, const Material* material);

#endif /* ifndef INCLUDE_ENGINE_LIGHTING_H */
