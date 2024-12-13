#ifndef SCREENPLAY_H
#define SCREENPLAY_H

#include "../ru.h"
#include "../math.h"
#include "../assets.h"
#include "../index.h"
#include "./screenplay_core.h"

void print_screenplay_text_color(const char *text, int index, float ta, float tb, float e, Color c) {
    print_screenplay_text_pos(text, ta, tb, e, 16, (rufont_size + 2) * index + 16, c);
}

void print_screenplay_text(const char *text, int index, float ta, float tb, float e) {
    print_screenplay_text_color(text, index, ta, tb, e, WHITE);
}

void step_screenplay_scene(
    const ScreenplaySceneConf *conf, 
    ScreenplayScene *scene, 
    bool commit) 
{
     int _page = scene->page;
    float _elapsed = scene->elapsed;
             
    const float scene_duration = conf->duration;
    const int pages_count = conf->pages_count;
    const float *page_durations = conf->pages_durations;
    
    if (commit) {
        _page += 1;
    }
    
      if (_page > pages_count + 1) {
        scene->progress = 1.0;
        return;
    }
    
     float e = fminf(_elapsed / scene_duration, 1.0);

    // a. time flips over page
    if ( page_durations[_page] > e) {
        e = page_durations[_page];
    }
    
    // b. page flips over time
    for (int i = pages_count + 1; i >= _page ; i--) {
        const float d = page_durations[i];
        if (d < e) {
            _page = i;
            break;
        }
    }
    
     if (_page != scene->page) {
        PlaySound(ASSET_SOUND_CHIP1);
     }
     
     scene->progress = e;
     scene->elapsed = e * scene_duration;
     scene->page = _page;
     
          
#ifdef SCREENPLAY_DEBUG
    draw_text_ru(
        TextFormat(
            "elapsed:  %f\n"
            "progress: %f\n"
            "page:     %d\n",
            scene->elapsed,
            scene->progress,
            scene->page
         ), 
        16, viewport_h - 16 - rufont_size * 3, WHITE);
#endif

    if (conf->hold) {
         scene->progress = clamp(0.0, 0.99, scene->progress);
    }
}

ScreenplaySceneConf screenplay_scene_a_conf = {
    4, 2, true,
    { 
        0.0, // begin
        0.4, //  task text
        0.6, // storymode text
        0.9   //  inputs hint
     }
};
int scene_a(ScreenplayScene *scene, bool commit) {
    const ScreenplaySceneConf *conf = &screenplay_scene_a_conf;
    
    int _page = scene->page;
    float e = fminf( scene->elapsed / conf->duration, 1.0);
    
     step_screenplay_scene(conf, scene, commit);
    
    if (_page != scene->page) {
        if (scene->page == 1) {
            PlaySound(ASSET_SOUND_STORYMODE);
        }
        if (scene->page == 3) {
            PlaySound(ASSET_SOUND_READY);
        }
    }

    const float *pd = conf->pages_durations;
    print_screenplay_text("Space", 0, pd[0], pd[1], e);
    print_screenplay_text_color("     days.", 0, pd[0], pd[1], e, RED);
    print_screenplay_text("Story mode.", 1, pd[1], pd[2], e);
    print_screenplay_text("SPACE, ENTER, LMB для продолжения.", 3, pd[2], pd[3], e);
     
     return 0;
}

ScreenplaySceneConf screenplay_scene_b_conf = {
    4, 2, true,
    { 
        0.0, // begin
        0.1,  // background 1 
        0.4,  // hint
        0.7,  // credits
        0.9   // end
     }
};

int scene_b(ScreenplayScene *scene, bool commit) {
    const ScreenplaySceneConf *conf = &screenplay_scene_b_conf;
    
   // int _page = scene->page;
    float e = fminf( scene->elapsed / conf->duration, 1.0);
    
     step_screenplay_scene(conf, scene, commit);
     
     const float *pd = conf->pages_durations;
     
     const int pad = 8;
     const int redarea_h = 256 - pad * 2;
     const Color bc = IsKeyUp(KEY_R) ? BLUE : RED;
     DrawRectangle(pad, pad, viewport_w - pad * 2, redarea_h, Fade(bc, nmap(pd[0], pd[2], e) * 0.7 + 0.3));
     
     print_screenplay_text_color("@tynroar, wit.games;", 0, pd[1], pd[3], e, BLACK);
     
     const char *text_hint = "А вы знали, что при загрузке \nпоказываются дурацкие советы?";
     print_screenplay_text_pos(text_hint, pd[1], pd[2], e, 16 + pad, redarea_h + 2, WHITE);

     const int tl_px = viewport_w - 400;
     const int tl_py = viewport_h - 32 - rufont_size;
    const char *text_loading = "Типа загрузка...";
     draw_text_ru(text_loading, tl_px, tl_py, GRAY);
     print_screenplay_text_pos(text_loading, pd[1], pd[4], e, tl_px, tl_py, RED);

     //DrawRectangle(0, 0, viewport_w, viewport_h, Fade(BLACK, nmap(pd[4], 1.0, e)));
     
     const int picsize = 32;
     const int hearts = 3;
     Color hearts_tint = Fade(BLACK, nmap(pd[2], pd[3], e));
     for (int i = 1; i <= hearts; i++) {
              DrawTexturePro(
                ASSET_TEXTURE_HEART,
                (Rectangle) { 0, 0, ASSET_TEXTURE_HEART.width, ASSET_TEXTURE_HEART.height },
                (Rectangle) { viewport_w - picsize * i - pad * 2 - 2 * i, pad * 2, picsize, picsize },
                (Vector2)      {0, 0},
                0, hearts_tint
             );
     }
     
    DrawTexturePro(
            ASSET_TEXTURE_HEXAGON,
            (Rectangle) { 0, 0, ASSET_TEXTURE_HEART.width, ASSET_TEXTURE_HEART.height },
            (Rectangle) { viewport_w - picsize - pad * 2 - 2, pad * 2 + picsize * 1 + 2 * 1, picsize, picsize },
            (Vector2)      {0, 0},
            0, hearts_tint
         );
    draw_text_ru("0", viewport_w - picsize - pad - 32, pad * 2 + picsize - 6, hearts_tint);
        
     return 0;
}


#endif
