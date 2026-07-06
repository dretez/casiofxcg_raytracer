#include "Scene/Light/LightSampler.h"

#include "Scene/Light/AreaLight.h"
#include "Scene/Light/Light.h"
#include "Vector.h"

#include <math.h>

void LightSampler_init(const Light* light, Vec3 shadingPoint, LightSampler* sampler) {
    switch (light->type) {
    case LIGHT_POINT:
        return;
    case LIGHT_AREA:
        sampler->center    = light->pos;
        sampler->curSample = 0;

        Vec3 w = Vec3_normalize(Vec3_sub(light->pos, shadingPoint));

        Vec3  up  = w.y < 0.99f ? vec(0.0f, 1.0f, 0.0f) : vec(1.0f, 0.0f, 0.0f);
        float inv = 1.0f / sqrtf(1.0f - (up.y ? w.y * w.y : w.x * w.x));

        sampler->u = Vec3_scale(up.y ? vec(w.z, 0.0f, -w.x) : vec(0.0f, -w.z, w.y),
                                inv * ((AreaLight*)light)->radius);
        sampler->v = Vec3_cross(w, sampler->u);
        break;
    }
}

Vec3 LightSampler_sample(const Light* light, LightSampler* sampler) {
    switch (light->type) {
    case LIGHT_POINT:
        return light->pos;
    case LIGHT_AREA:
        return AreaLight_sample(sampler->curSample++, sampler);
    }
    return (Vec3){ 0 };
}
