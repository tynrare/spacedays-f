#ifndef MISC_H
#define MISC_H

#include "index.h"

void draw_lint_x(int y) {
    DrawLine(0, y, viewport_w, y, RED);    
}

void draw_lint_y(int x) {
    DrawLine(x, 0, x, viewport_h, RED);    
}

void DrawTextureFlipped(Texture texture, Color tint) {
        DrawTexturePro(
            texture, 
            (Rectangle){ 0, 0, texture.width, -texture.height }, 
            (Rectangle){ 0, 0, texture.width, texture.height }, 
            (Vector2) { 0,0 }, 
            0 * RAD2DEG, tint);
}

void draw_grid() {
    draw_lint_x(2);
    draw_lint_x(viewport_h - 2);
    draw_lint_y(2);
    draw_lint_y(viewport_w - 2);
    
    //DrawLine(2, 0, 2, viewport_h, RED);
    //DrawLine(0, 2, viewport_w, 2, RED);
    //DrawLine(viewport_w - 2, 0, viewport_w - 2, viewport_h, RED);
    //DrawLine(0, viewport_h - 2, viewport_w, viewport_h - 2, RED);
}

#endif