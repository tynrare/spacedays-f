#include "raylib.h"
#include "index.h"

#ifndef ARENDER_H
#define ARENDER_H

typedef struct AppRenderTextures {
  RenderTexture2D rt_f0;
  RenderTexture2D rt_f1;
  RenderTexture2D rt_f2;
} AppRenderTextures;

AppRenderTextures *arender = {0};

void arender_resized(int w, int h) {
  UnloadRenderTexture(arender->rt_f0);
  arender->rt_f0 = LoadRenderTexture(viewport_w, viewport_h);
  UnloadRenderTexture(arender->rt_f1);
  arender->rt_f1 = LoadRenderTexture(viewport_w, viewport_h);
  UnloadRenderTexture(arender->rt_f2);
  arender->rt_f2 = LoadRenderTexture(viewport_w, viewport_h);
}

void arender_load() {
  arender = malloc(sizeof(AppRenderTextures));
  arender->rt_f0 = LoadRenderTexture(viewport_w, viewport_h);
  arender->rt_f1 = LoadRenderTexture(viewport_w, viewport_h);
  arender->rt_f2 = LoadRenderTexture(viewport_w, viewport_h);
}

void arender_unload() {
  UnloadRenderTexture(arender->rt_f0);
  UnloadRenderTexture(arender->rt_f1);
  UnloadRenderTexture(arender->rt_f2);
  free(arender);
}

void DrawTextureFlipped(Texture texture, Color tint) {
        DrawTexturePro(
            texture, 
            (Rectangle){ 0, 0, texture.width, -texture.height }, 
            (Rectangle){ 0, 0, texture.width, texture.height }, 
            (Vector2) { 0,0 }, 
            0 * RAD2DEG, tint);
}


#endif
