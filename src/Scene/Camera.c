#include "Scene/Camera.h"

Ray cameraRay(const Camera* camera, int pixelX, int pixelY) {
    Vec3 right     = Vec3_scale(camera->right, camera->screenX[pixelX]);
    Vec3 up        = Vec3_scale(camera->up, camera->screenY[pixelY]);
    Vec3 direction = Vec3_add(Vec3_add(right, up), camera->forward);

    return (Ray){ .origin = camera->position, .direction = Vec3_normalize(direction) };
}
