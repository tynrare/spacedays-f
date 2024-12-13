#ifndef SCENE_TEST_DZENSUN_H
#define SCENE_TEST_DZENSUN_H

#include "raylib.h"
#include "./screenplay_core.h"
#include "../gtbox/index.h"
#include <stdlib.h>
#include "../assets.h"

typedef struct SceneTestDzensunState {
    GtboxDzenSun dzensun;
} SceneTestDzensunState;

SceneTestDzensunState *scene_test_dzensun_state = { 0 };


void draw_dzensun_particle(
    float *x, float *y, 
    float *dx, float *dy,
    float *intensity
    ) {
        //DrawRectangle(*x - 2, *y - 2, 4, 4, MAGENTA);
        //DrawLine(*x, *y, *x + *dx * 10, *y + *dy * 10, MAGENTA);
        
        const Texture tex = ASSET_TEXTURE_PARTICLE1;
        float angle = Vector2Angle(vup, (Vector2) { *dx, *dy }) - 0;
        const float size = 64;
        DrawTexturePro(
            tex, 
            (Rectangle){ 0, 0, size, size }, 
            (Rectangle){ *x, *y, size, size }, 
            (Vector2) { size * 0.5, size * 0.5 }, 
            angle * RAD2DEG, 
            Fade(WHITE, *intensity));

}

int scene_test_dzensun(ScreenplayScene *scene, bool commit) {
    DrawText("Test dzensun.", 16, 16, 42, WHITE);
    
    if (scene->page == 0) {
        scene->page += 1;
        scene_test_dzensun_state = malloc(sizeof(SceneTestDzensunState));
        gtbox_dzensun_init(&scene_test_dzensun_state->dzensun);
    }
    
    Vector2 mp = getmp();
    TynPoolCell *aims_p = scene_test_dzensun_state->dzensun.aims->active;
    float *aims_x = aims_p->point + 0;
    float *aims_y = aims_x + 1;
    *aims_x = mp.x;
    *aims_y = mp.y;
    
    gtbox_dzensun_step(&scene_test_dzensun_state->dzensun, draw_dzensun_particle);
    
    if (scene->page == 2) {
        scene->page += 1;
        gtbox_dzensun_dispose(&scene_test_dzensun_state->dzensun);
        free(scene_test_dzensun_state);
    }
    
    return 0;
}

#endif