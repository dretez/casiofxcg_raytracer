#include "Scene/Camera.h"

#include <math.h>
#include <stdlib.h>

#include "Vector/Geometry.h"
#include "Vector/Vector.h"

void Camera_free(Camera* camera) {
    if (camera->screenX) {
        free(camera->screenX);
        camera->screenX = NULL;
    }
    if (camera->screenY) {
        free(camera->screenY);
        camera->screenY = NULL;
    }
}

int makeCamera(Camera* camera, Vec3 position, Vec3 target, Vec3 worldUp, float fov, int width, int height) {
    camera->screenX = malloc(sizeof(geo_t) * width);
    if (camera->screenX == NULL) return 1;
    camera->screenY = malloc(sizeof(geo_t) * height);
    if (camera->screenY == NULL) {
        free(camera->screenX);
        camera->screenX = NULL;
        return 1;
    }

    geo_t invWidth  = geo_inv(width);
    geo_t invHeight = geo_inv(height);

    geo_t aspect = geo_mul((width * GEO_ONE), invHeight);
    geo_t scaleY = geo_from_float(tanf(fov * 0.5f));
    geo_t scaleX = geo_mul(aspect, scaleY);

    geo_t pixelScaleX = geo_mul(GEO_TWO, geo_mul(scaleX, invWidth));
    geo_t pixelScaleY = geo_mul(-GEO_TWO, geo_mul(scaleY, invHeight));
    geo_t offsetX     = geo_sub(geo_mul(scaleX, invWidth), scaleX);
    geo_t offsetY     = geo_sub(scaleY, geo_mul(scaleY, invHeight));

    camera->forward = Vec3_normalize(Vec3_sub(target, position));
    camera->right   = Vec3_normalize(Vec3_cross(camera->forward, worldUp));
    camera->up      = Vec3_cross(camera->right, camera->forward);

    geo_t x = 0;
    for (int i = 0; i < width; i++, x += pixelScaleX) camera->screenX[i] = x + offsetX;
    geo_t y = 0;
    for (int i = 0; i < height; i++, y += pixelScaleY) camera->screenY[i] = y + offsetY;

    return 0;
}

Ray cameraRay(const Camera* camera, int pixelX, int pixelY) {
    Vec3 right     = Vec3_scale(camera->right, camera->screenX[pixelX]);
    Vec3 up        = Vec3_scale(camera->up, camera->screenY[pixelY]);
    Vec3 direction = Vec3_add(Vec3_add(right, up), camera->forward);

    return (Ray){ .origin = camera->position, .direction = Vec3_normalize(direction) };
}
