#include "../index.h"

#ifndef TYNGAME_LOCOMOTIONS_H
#define TYNGAME_LOCOMOTIONS_H

Vector2 lmp = { -1, -1 };
Vector2 getmp() {
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) || IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
        lmp = GetMousePosition();
        return lmp;
    }
    
    if (lmp.x != -1 && lmp.y != -1) {
        return lmp;
    }
    
    Vector2 mp = { GetScreenWidth() * 0.5, GetScreenHeight() * 0.5 };
    return mp;
};

#define WATTS 4
#define FREC 1.2
#define WAVE 1.0
#define ACC 2.0

void locomotion_pull(float *x, float *y, float *dirx, float *diry, Vector2 goal) {
    Vector2 pos = { *x, *y };
    Vector2 dir = { *dirx, *diry };
    Vector2 goaldelta = Vector2Subtract(goal, pos);
    Vector2 ngoaldelta = Vector2Normalize(goaldelta);
    Vector2 ndir = Vector2Normalize(dir);
    float angle = Vector2Angle(ndir, ngoaldelta);
    Vector2 newdir = Vector2Rotate(dir, angle * GetFrameTime() * FREC);
    Vector2 nnewdir = Vector2Normalize(newdir);
    Vector2 snewdir = Vector2Scale(
              nnewdir,
              1 + Vector2DotProduct(ngoaldelta, nnewdir) *ACC
             );
    newdir.x = dlerp(newdir.x, snewdir.x, WAVE, GetFrameTime());
    newdir.y = dlerp(newdir.y, snewdir.y, WAVE, GetFrameTime());
    *dirx = newdir.x;
    *diry = newdir.y;
}

void locomotion_push(float *x, float *y, float *dirx, float *diry, Vector2 goal) {
    Vector2 pos = { *x, *y };
    Vector2 dir = { *dirx, *diry };
    Vector2 goaldelta = Vector2Subtract(pos, goal);
    Vector2 ngoaldelta = Vector2Normalize(goaldelta);
    Vector2 ndir = Vector2Normalize(dir);
    float angle = Vector2Angle(ndir, ngoaldelta);
    Vector2 newdir = Vector2Rotate(dir, angle * GetFrameTime() * FREC);
    Vector2 nnewdir = Vector2Normalize(newdir);
    Vector2 snewdir = Vector2Scale(
              nnewdir,
              1 + Vector2DotProduct(ngoaldelta, nnewdir) *ACC
             );
    newdir.x = dlerp(newdir.x, snewdir.x, WAVE, GetFrameTime());
    newdir.y = dlerp(newdir.y, snewdir.y, WAVE, GetFrameTime());
    
    *dirx = newdir.x;
    *diry = newdir.y;
}



#define PAD_LVBT 1024
void loc_viewport_bound_teleport(float *x, float *y) {
    if (*x < -PAD_LVBT) {
        *x = viewport_w + PAD_LVBT;
    } else if (*x > viewport_w + PAD_LVBT) {
        *x = - PAD_LVBT;
    }
    
    if (*y < -PAD_LVBT) {
        *y = viewport_h + PAD_LVBT;
    } else if (*y > viewport_h + PAD_LVBT) {
        *y = - PAD_LVBT;
    }
}


#endif