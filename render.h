#ifndef RENDER_H_INCLUDED
#define RENDER_H_INCLUDED

#include <stdlib.h>
#include <string.h>

#define SCREEN_SIZE_X 64
#define SCREEN_SIZE_Y 32
const int PIXEL_SCALE = 16;

typedef struct {
    bool pixel_screen[SCREEN_SIZE_X][SCREEN_SIZE_Y];
} Render;

Render * InitRender() {
    Render *tmp_render;
    tmp_render = calloc(1, sizeof(Render));
    return tmp_render;
}

void FreeRender(Render *self) {
    free(self);
}

void ClearRender(Render *self){
    memset(self, 0, sizeof(Render));
}

bool SetPixelRender(Render *self, int x, int y){
    self->pixel_screen[x][y] = !self->pixel_screen[x][y];
    return !self->pixel_screen[x][y];
}

void UpdateRender(Render *self){
    BeginDrawing();
    ClearBackground(BLACK);
    for (int x = 0; x < SCREEN_SIZE_X; x++)
        for (int y = 0; y < SCREEN_SIZE_Y; y++)
            if (self->pixel_screen[x][y] == true)
                DrawRectangle(x * PIXEL_SCALE, y * PIXEL_SCALE, PIXEL_SCALE, PIXEL_SCALE, WHITE);
    EndDrawing();
}

#endif // RENDER_H_INCLUDED
