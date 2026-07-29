#include "Scene/Camera.h"

#include <math.h>
#include <stdlib.h>

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

    geo_t invWidth  = geo_inv(geo_from_float((float)width));
    geo_t invHeight = geo_inv(geo_from_float((float)height));

    geo_t aspect = geo_mul(geo_from_float((float)width), invHeight);
    geo_t scaleY = geo_from_float(tanf(fov * 0.5f));
    geo_t scaleX = geo_mul(aspect, scaleY);

    geo_t pixelScaleX = geo_mul(geo_mul(GEO_TWO,  scaleX), invWidth);
    geo_t pixelScaleY = geo_mul(geo_mul(-GEO_TWO, scaleY), invHeight);
    geo_t offsetX     = geo_sub(geo_mul(scaleX, invWidth), scaleX);
    geo_t offsetY     = geo_sub(scaleY, geo_mul(scaleY, invHeight));

    camera->position = position;
    camera->forward = Vec3_sub(target, position);
    camera->forward = Vec3_normalize(camera->forward);
    camera->right   = Vec3_cross(camera->forward, worldUp);
    camera->right   = Vec3_normalize(camera->right);
    camera->up      = Vec3_cross(camera->right, camera->forward);

    geo_t x = GEO_ZERO;
    for (int i = 0; i < width; i++, x = geo_add(x, pixelScaleX))
        camera->screenX[i] = x + offsetX;
    geo_t y = GEO_ZERO;
    for (int i = 0; i < height; i++, y = geo_add(y, pixelScaleY))
        camera->screenY[i] = y + offsetY;

    return 0;
}

Ray cameraRay(const Camera* camera, int pixelX, int pixelY) {
    Vec3 right     = Vec3_scale(camera->right, camera->screenX[pixelX]);
    Vec3 up        = Vec3_scale(camera->up, camera->screenY[pixelY]);
    Vec3 direction = Vec3_add(Vec3_add(right, up), camera->forward);

    return (Ray){ .origin = camera->position, .direction = Vec3_normalize(direction) };
}
