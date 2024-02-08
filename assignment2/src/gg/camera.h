#if !defined(CAMERA_H)
#define CAMERA_H

#include <stdint.h>
#include <raylib.h>

typedef struct camera {
    float zoom;
    float rotation;
    Vector2 offset;
    Vector2 position;
} gg_camera_t;

void Camera_Create(gg_camera_t* camera, uint32_t w, uint32_t h);
void Camera_Update(gg_camera_t* camera, float dt);
Camera2D Camera_GetCamera2D(gg_camera_t* camera);
void Camera_DrawDebug(gg_camera_t* camera);

#endif // CAMERA_H
