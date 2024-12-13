#include "../math.h"
#include <string.h>
#include "../ru.h"

#ifndef SCREENPLAY_CORE_H
#define SCREENPLAY_CORE_H

#define SCREENPLAY_DEBUG

typedef struct ScreenplayScene {
    int page;
    float elapsed;
    float progress;
} ScreenplayScene;

typedef struct ScreenplaySceneConf {
    float duration;
    int pages_count;
    bool hold;
    float pages_durations[8];
} ScreenplaySceneConf;

void print_screenplay_text_pos(
    const char *text, float ta, float tb, float e, int x, int y, Color c
) {
    const float p = nmap(ta, tb, e);
    const char *t = TextFormat("%.*s",  (int)(strlen(text) * p), text);
    draw_text_ru( t, x, y, c);
}

#endif
