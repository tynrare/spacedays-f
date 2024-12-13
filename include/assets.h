#ifndef ASSETS_H
#define ASSETS_H


#include "raylib.h"
#include "resources.h"
#include "index.h"

#include "rlgl.h"

#define TEXTURES_AUTO_RELOAD
#define SHADERS_AUTO_RELOAD


#define ASSETS_SOUNDS_COUNT 4
#define ASSETS_TEXTURES_COUNT 6
#define ASSETS_SHADERS_COUNT 4

static const char* const assets_sounds_filenames[] = {
    SOUNDS_PATH "story_mode.ogg",
    SOUNDS_PATH "switch1.ogg",
    SOUNDS_PATH "chip-lay-1.ogg",
    SOUNDS_PATH "ready.ogg"
};

static const char* const assets_textures_filenames[] = {
    TEXTURES_PATH "palette.png",
    TEXTURES_PATH "suit_hearts.png",
    TEXTURES_PATH "hexagon.png",
    TEXTURES_PATH "noise0.png",
    TEXTURES_PATH "particle0.png",
    TEXTURES_PATH "particle1.png",
};

typedef struct TynShaderGeneric {
    Shader shader;
    int time_loc;
    int tpalette_loc; 
    int resolution_loc;
    int tnoise0_loc;
    int tnoise1_loc;
} TynShaderGeneric;

static const char* const assets_shaders_filenames[] = {
    SHADERS_PATH "sprite_generic.fs",
    SHADERS_PATH "chromakey.fs",
    SHADERS_PATH "sprite_sdf.fs",
    SHADERS_PATH "vfx_goldflames.fs"
};

typedef struct Assets {
    Sound sounds[ASSETS_SOUNDS_COUNT];
    int audio_file_mod_times[ASSETS_SOUNDS_COUNT];
    Texture textures[ASSETS_TEXTURES_COUNT];
    int texture_file_mod_times[ASSETS_TEXTURES_COUNT];
    TynShaderGeneric shaders[ASSETS_SHADERS_COUNT];
    int shader_file_mod_times[ASSETS_SHADERS_COUNT];
    Texture tex_noise1;
} Assets;

Assets *assets = { 0 };

#define ASSET_SOUND_STORYMODE           assets->sounds[0]
#define ASSET_SOUND_SWITCH1             assets->sounds[1]
#define ASSET_SOUND_CHIP1               assets->sounds[2]
#define ASSET_SOUND_READY               assets->sounds[3]

#define ASSET_TEXTURE_PALETTE           assets->textures[0]
#define ASSET_TEXTURE_HEART             assets->textures[1]
#define ASSET_TEXTURE_HEXAGON           assets->textures[2]
#define ASSET_TEXTURE_NOISE0            assets->textures[3]
#define ASSET_TEXTURE_PARTICLE0         assets->textures[4]
#define ASSET_TEXTURE_PARTICLE1         assets->textures[5]


#define ASSET_SHADER_SPRITE_GENERIC     assets->shaders[0].shader
#define ASSET_SHADER_CHROMAKEY          assets->shaders[1].shader
#define ASSET_SHADER_SDF                assets->shaders[2].shader
#define ASSET_GSHADER_VFX_GOLDFLAMES    assets->shaders[3]
#define ASSET_SHADER_VFX_GOLDFLAMES     ASSET_GSHADER_VFX_GOLDFLAMES.shader

static void assets_load_sound(Assets *assets, int index) {
    Sound sound = LoadSound(assets_sounds_filenames[index]);
    /*
    if (texture.id == 0) {
        return;
    }
    */
    if (assets->audio_file_mod_times[index] != 0) {
         UnloadSound(assets->sounds[index]);
    }
    assets->sounds[index] = sound;
    assets->audio_file_mod_times[index] = GetFileModTime(assets_sounds_filenames[index]);
}

static void load_texture(Assets *assets, int index) {
    Texture texture = LoadTexture(assets_textures_filenames[index]);
    if (texture.id == 0) {
        return;
    }
    if (assets->texture_file_mod_times[index] != 0) {
         UnloadTexture(assets->textures[index]);
    }
    assets->textures[index] = texture;
    assets->texture_file_mod_times[index] = GetFileModTime(assets_textures_filenames[index]);
}

static void load_shader(Assets *assets, int index) {
     const char *filename = assets_shaders_filenames[index];
    Shader shader = LoadShader(0,  filename);
    if (shader.id == rlGetShaderIdDefault()) { 
        return;
    }
          
    TynShaderGeneric *tsg = &assets->shaders[index];
     if (assets->shader_file_mod_times[index] != 0) {
        UnloadShader(tsg->shader);
     }

    tsg->shader = LoadShader(0,  filename);
    assets->shader_file_mod_times[index] = GetFileModTime(filename);

     tsg->time_loc = GetShaderLocation(tsg->shader , "time");
     tsg->tpalette_loc = GetShaderLocation(tsg->shader , "tex_palette");
     tsg->resolution_loc = GetShaderLocation(tsg->shader , "resolution");
     tsg->tnoise0_loc = GetShaderLocation(tsg->shader , "tex_noise0");
     tsg->tnoise1_loc = GetShaderLocation(tsg->shader , "tex_noise1");
     
     // Activate it after BeginShaderMode
     //SetShaderValueTexture(tsg->shader, tsg->tpalette_loc, ASSET_TEXTURE_PALETTE);
     //SetShaderValueTexture(tsg->shader, tsg->tnoise0_loc, assets->tex_noise0);
     //SetShaderValueTexture(tsg->shader, tsg->tnoise1_loc, assets->tex_noise1);
}

void assets_load(Assets *assets) {
    for (int i = 0; i < ASSETS_SOUNDS_COUNT; i++) {
        assets->audio_file_mod_times[i] = 0;
        assets_load_sound(assets, i);
    }
    
    for (int i = 0; i < ASSETS_TEXTURES_COUNT; i++) {
        assets->texture_file_mod_times[i] = 0;
        load_texture(assets, i);
    }
    
    for (int i = 0; i < ASSETS_SHADERS_COUNT; i++) {
        assets->shader_file_mod_times[i] = 0;
        load_shader(assets, i);
    }
    
    Image cellular = GenImageCellular(1024, 1024, 32);
    assets->tex_noise1 = LoadTextureFromImage(cellular);
    UnloadImage(cellular);
}

void assets_unload(Assets *assets) {
    for (int i = 0; i < ASSETS_SOUNDS_COUNT; i++) {
        assets->audio_file_mod_times[i] = 0;
        UnloadSound(assets->sounds[i]);
    }
   
   for (int i = 0; i < ASSETS_TEXTURES_COUNT; i++) {
        assets->texture_file_mod_times[i] = 0;
        UnloadTexture(assets->textures[i]);
    }
    
    UnloadTexture(assets->tex_noise1);
}

void assets_update_resized(Assets *assets) {
    const float resolution[2] = { viewport_w, viewport_h };
    for (int i = 0; i < ASSETS_SHADERS_COUNT; i++) {
                SetShaderValue(assets->shaders[i].shader, assets->shaders[i].resolution_loc, &resolution, SHADER_UNIFORM_IVEC2);
    }
}

void assets_update(Assets *assets) {
    #ifdef TEXTURES_AUTO_RELOAD
        for (int i = 0; i < ASSETS_TEXTURES_COUNT; i++) {
            if (file_modified(assets_textures_filenames[i], assets->texture_file_mod_times[i])) {
                load_texture(assets, i);
            }
        }
    #endif
    
   
    for (int i = 0; i < ASSETS_SHADERS_COUNT; i++) {
        #ifdef SHADERS_AUTO_RELOAD
        if (file_modified(assets_shaders_filenames[i], assets->shader_file_mod_times[i])) {
            load_shader(assets, i);
        }
        #endif
        
        float time = (float)GetTime();
        SetShaderValue(assets->shaders[i].shader, assets->shaders[i].time_loc, &time, SHADER_UNIFORM_FLOAT);
    }


}

#endif
