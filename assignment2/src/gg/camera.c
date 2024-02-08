#include "camera.h"

#include <raymath.h>

static float key_axis(KeyboardKey negative, KeyboardKey positive) {
    return (IsKeyDown(negative) ? -1.f : 1.f) +
           (IsKeyDown(positive) ? 1.f : -1.f);
}

void Camera_Create(gg_camera_t* camera, uint32_t w, uint32_t h) {
    camera->offset = (Vector2){.x = w / 2.f, .y = h / 2.f};
    camera->position = (Vector2){.x = 0, .y = 0};
    camera->rotation = 0.f;
    camera->zoom = 2.f;
}

void Camera_Update(gg_camera_t* camera, float dt) {
    if (IsWindowResized()) {
        camera->offset =
            (Vector2){GetRenderWidth() / 2.f, GetRenderHeight() / 2.f};
    }

    camera->position =
        Vector2Add(camera->position,
                   (Vector2){key_axis(KEY_A, KEY_D), key_axis(KEY_W, KEY_S)});
}

Camera2D Camera_GetCamera2D(gg_camera_t* camera) {
    return (Camera2D){.zoom = camera->zoom,
                      .rotation = camera->rotation,
                      .offset = camera->offset,
                      .target = camera->position};
}

void Camera_DrawDebug(gg_camera_t* camera) {
    DrawLine(0, 0, 10, 0, RED);
    DrawLine(0, 0, 0, 10, GREEN);
}