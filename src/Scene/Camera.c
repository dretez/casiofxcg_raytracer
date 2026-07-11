#include "Scene/Camera.h"

#include <math.h>
#include <stdlib.h>

#include "Vector/FloatingVector.h"
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
    camera->screenX = malloc(sizeof(float) * width);
    if (camera->screenX == NULL) return 1;
    camera->screenY = malloc(sizeof(float) * height);
    if (camera->screenY == NULL) {
        free(camera->screenX);
        camera->screenX = NULL;
        return 1;
    }

    float invWidth  = 1.0f / width;
    float invHeight = 1.0f / height;

    float aspect = (float)width * invHeight;
    float scaleY = tanf(fov * 0.5f);
    float scaleX = aspect * scaleY;

    float pixelScaleX = 2.0f * scaleX * invWidth;
    float pixelScaleY = -2.0f * scaleY * invHeight;
    float offsetX     = scaleX * invWidth - scaleX;
    float offsetY     = scaleY - scaleY * invHeight;

    camera->forward = Vec3_normalize(Vec3_sub(target, position));
    camera->right   = Vec3_normalize(Vec3_cross(camera->forward, worldUp));
    camera->up      = Vec3_cross(camera->right, camera->forward);

    float x = 0;
    for (int i = 0; i < width; i++, x += pixelScaleX) camera->screenX[i] = x + offsetX;
    float y = 0;
    for (int i = 0; i < height; i++, y += pixelScaleY) camera->screenY[i] = y + offsetY;

    return 0;
}

Ray cameraRay(const Camera* camera, int pixelX, int pixelY) {
    Vec3 right     = Vec3_scale(camera->right, camera->screenX[pixelX]);
    Vec3 up        = Vec3_scale(camera->up, camera->screenY[pixelY]);
    Vec3 direction = Vec3_add(Vec3_add(right, up), camera->forward);

    return (Ray){ .origin = camera->position, .direction = Vec3_normalize(direction) };
}
