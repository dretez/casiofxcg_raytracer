#ifndef INCLUDE_SCENE_CAMERA_H
#define INCLUDE_SCENE_CAMERA_H

#include "Scene/Ray.h"
#include "Vector/Geometry.h"
#include "Vector/Vector.h"

typedef struct {
    Vec3 position;

    Vec3 forward;
    Vec3 up;
    Vec3 right;

    geo_t *screenX;
    geo_t *screenY;
} Camera;

void Camera_free(Camera* camera);
int makeCamera(Camera* camera, Vec3 position, Vec3 target, Vec3 worldUp, float fov, int width, int height);
Ray cameraRay(const Camera* camera, int pixelX, int pixelY);

#endif /* ifndef INCLUDE_SCENE_CAMERA_H */
