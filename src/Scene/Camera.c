#include "Scene/Camera.h"

#include <math.h>
#include <stdlib.h>

#include "Vector.h"

int makeCamera(Camera* camera, Vec3 position, Vec3 target, Vec3 worldUp, float fov, int width, int height) {
    float scale  = tanf(fov * 0.5f);
    float aspect = (float)width / height;

    float invWidth    = 1.0f / width;
    float invHeight   = 1.0f / height;
    float scaleX      = aspect * scale;
    float scaleY      = scale;
    float pixelScaleX = 2.0f * scaleX * invWidth;
    float pixelScaleY = -2.0f * scaleY * invHeight;
    float offsetX     = scaleX * invWidth - scaleX;
    float offsetY     = scaleY - scaleY * invHeight;

    camera->forward = Vec3_normalize(Vec3_sub(target, position));
    camera->right   = Vec3_normalize(Vec3_cross(camera->forward, worldUp));
    camera->up      = Vec3_cross(camera->right, camera->forward);

    camera->screenX = malloc(sizeof(float) * width);
    if (camera->screenX == NULL)
        return 1;
    camera->screenY = malloc(sizeof(float) * height);
    if (camera->screenY == NULL) {
        free(camera->screenX);
        return 1;
    }

    for (int i = 0; i < width; i++) camera->screenX[i] = i * pixelScaleX + offsetX;
    for (int i = 0; i < height; i++) camera->screenY[i] = i * pixelScaleY + offsetY;

    return 0;
}

Ray cameraRay(const Camera* camera, int pixelX, int pixelY) {
    Vec3 direction = Vec3_add(camera->forward, Vec3_scale(camera->right, camera->screenX[pixelX]));
    direction      = Vec3_add(direction, Vec3_scale(camera->up, camera->screenY[pixelY]));
    direction      = Vec3_normalize(direction);

    return (Ray){ .origin = camera->position, .direction = direction };
}
