#ifndef SCENE_TEST_DZENSUN_H
#define SCENE_TEST_DZENSUN_H

#include "../arender.h"
#include "../assets.h"
#include "../gtbox/index.h"
#include "./screenplay_core.h"
#include "raylib.h"
#include <stdlib.h>

typedef struct SceneTestDzensunState {
  GtboxDzenSun dzensun;
} SceneTestDzensunState;

SceneTestDzensunState *scene_test_dzensun_state = {0};

void draw_dzensun_particle(float *x, float *y, float *dx, float *dy,
                           float *intensity) {
  // DrawRectangle(*x - 2, *y - 2, 4, 4, MAGENTA);
  // DrawLine(*x, *y, *x + *dx * 10, *y + *dy * 10, MAGENTA);

  const Texture tex = ASSET_TEXTURE_PARTICLE1;
	Vector2 dir = {*dx, *dy};
  float angle = Vector2Angle(vup, dir) - 0;
  const float size = 32;
	float speed = Vector2Length(dir);
  DrawTexturePro(tex, (Rectangle){0, 0, 64, 64},
                 (Rectangle){*x, *y, size, size * speed},
                 (Vector2){size * 0.5, size * 0.5}, angle * RAD2DEG,
                 Fade(WHITE, *intensity));
}

int scene_test_dzensun(RenderTexture2D rt, ScreenplayScene *scene, bool commit) {
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

  BeginTextureMode(arender->rt_f1);
  ClearBackground(BLACK);
  gtbox_dzensun_step(&scene_test_dzensun_state->dzensun, draw_dzensun_particle);
  EndTextureMode();

  BeginShaderMode(ASSET_SHADER_SDF);
  BeginTextureMode(arender->rt_f2);
  ClearBackground(RED);
	BeginBlendMode(BLEND_MULTIPLIED);
  DrawTextureFlipped(arender->rt_f1.texture, WHITE);
	EndBlendMode();
  EndTextureMode();
  EndShaderMode();

  if (scene->page == 2) {
    scene->page += 1;
    gtbox_dzensun_dispose(&scene_test_dzensun_state->dzensun);
    free(scene_test_dzensun_state);
  }

	BeginTextureMode(rt);
  DrawTextureFlipped(arender->rt_f2.texture, WHITE);
  DrawText("Test dzensun.", 16, 16, 42, WHITE);
	EndTextureMode();

  return 0;
}

#endif
