/*******************************************************************************************
*
*   tynroar gameprofitjam 240512 with Raylib assist.
*
*
********************************************************************************************/

#include "include/misc.h"
#include "include/index.h"
#include "include/ru.h"
#include "include/screenplay.h"

typedef struct AppState {
    int screen;
    ScreenplayScene sp_scene;
    float f;
} AppState;

typedef struct AppRenderTextures {
    RenderTexture2D rt_f0;
    RenderTexture2D rt_f1;
    RenderTexture2D rt_f2;
} AppRenderTextures;

AppState *ap = { 0 };
AppRenderTextures *arender = { 0 };

#define TITLE "Spacedays. tynroar. wit."

void draw_fin() {
    // draw chroma mask into second texture
    BeginShaderMode(ASSET_SHADER_CHROMAKEY);
    BeginTextureMode(arender->rt_f1);
    ClearBackground(BLACK);
    DrawTextureFlipped(arender->rt_f0.texture, WHITE);
    EndTextureMode();
    EndShaderMode();
    
    // subtract chroma from first texture
    BeginTextureMode(arender->rt_f2);
    ClearBackground(BLACK);
    BeginBlendMode(BLEND_SUBTRACT_COLORS);
    DrawTextureFlipped(arender->rt_f1.texture, WHITE);
    DrawTextureFlipped(arender->rt_f0.texture, WHITE);

    EndBlendMode();
    EndTextureMode();

    // draw post vfx onto first texture
    BeginTextureMode(arender->rt_f0);
    ClearBackground(BLACK);
    BeginBlendMode(BLEND_ADD_COLORS);
    
    DrawTextureFlipped(arender->rt_f2.texture, WHITE);
    
    TynShaderGeneric goldflames = ASSET_GSHADER_VFX_GOLDFLAMES;
    BeginShaderMode(goldflames.shader);
    SetShaderValueTexture(goldflames.shader, goldflames.tnoise0_loc, ASSET_TEXTURE_NOISE0);
    SetShaderValueTexture(goldflames.shader, goldflames.tnoise1_loc, assets->tex_noise1);
    DrawTextureFlipped(arender->rt_f1.texture, WHITE);
    EndShaderMode();
    
    EndBlendMode();
    EndTextureMode();

    // fin 
    DrawTextureFlipped(arender->rt_f0.texture, WHITE);
}

void draw() {    
    bool commit = 
        IsKeyPressed(KEY_SPACE) || 
        IsKeyPressed(KEY_ENTER) ||
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
        
        if (commit) {
            PlaySound(ASSET_SOUND_SWITCH1);
        }

    BeginTextureMode(arender->rt_f0);
    ClearBackground(BLACK);
    int page = 0;
    if (ap->screen >= SCREENPLAYS_COUNT) {
        draw_text_ru("Сцены кончились; kinda gameover.", 16, 16, WHITE);
    } else {
        ap->sp_scene.elapsed += GetFrameTime();
        page = screenplays[ap->screen](&ap->sp_scene, commit);
    }
    EndTextureMode();
    
    bool prev_screen = IsKeyPressed(KEY_LEFT);
    bool next_screen = page < 0 || ap->sp_scene.progress >= 1.0 || IsKeyPressed(KEY_RIGHT);
    bool reset = prev_screen || next_screen || IsKeyPressed(KEY_R);
    
    if (prev_screen && ap->screen > 0) {
        ap->screen -= 1;
    } else if (next_screen) {
        ap->screen += 1;
    }
    if (reset) {
        ap->sp_scene.elapsed = 0;
        ap->sp_scene.progress = 0;
        ap->sp_scene.page = 0;
        ap->f = 0;
    } 

    draw_fin();
    //DrawTexture(assets->tex_noise1, 0, 0, WHITE);
    draw_grid();
}

void resized(int w, int h) {
    viewport_w = w;
    viewport_h = h;
    
    UnloadRenderTexture(arender->rt_f0);
   arender->rt_f0  = LoadRenderTexture(viewport_w, viewport_h);
   UnloadRenderTexture(arender->rt_f1);
   arender->rt_f1  = LoadRenderTexture(viewport_w, viewport_h);
  UnloadRenderTexture(arender->rt_f2);
   arender->rt_f2 = LoadRenderTexture(viewport_w, viewport_h);
}

void step() {
    const int w = GetScreenWidth();
    const int h = GetScreenHeight();
    if (w != viewport_w || h != viewport_h) {
        resized(w, h);
    }
    
    assets_update(assets);
    
    BeginDrawing();
        draw();
    EndDrawing();
}

bool loop() {
    if (WindowShouldClose()) {
        return false;
    }
    
    // Main game loop
    step();
    
    return true;
}

void load() {
     load_rutopter();
     assets_load(assets);
     arender->rt_f0  = LoadRenderTexture(viewport_w, viewport_h);
     arender->rt_f1  = LoadRenderTexture(viewport_w, viewport_h);
     arender->rt_f2  = LoadRenderTexture(viewport_w, viewport_h);
}

void run() {
    InitWindow(viewport_w, viewport_h, TITLE);
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60);
    InitAudioDevice();

    load();

     while(loop()) {};
}

void init() {
    ap = malloc(sizeof(AppState));
    ap->screen = 0;
    ap->sp_scene.elapsed = 0;
    ap->sp_scene.page = 0;
    ap->f = 0;
    
    assets = malloc(sizeof(Assets));
    arender = malloc(sizeof(AppRenderTextures));
}

void unload() {
    UnloadRenderTexture(arender->rt_f0);
    assets_unload(assets);
}

void dispose() {
    free(ap);
}

int main(void)
{
    // Initialization
    
    init();
    run(); // loop here
    unload();
    dispose();
    CloseWindow();        // Close window and OpenGL context

    return 0;
}
