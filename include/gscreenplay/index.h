#include "screenplay_core.h"
#include "screenplay_a.h"
#include "scene_test_dzensun.h"


#define SCREENPLAYS_COUNT 3

int (*screenplays[SCREENPLAYS_COUNT])(RenderTexture2D, ScreenplayScene *, bool) = {
    scene_a,
    scene_b,
    scene_test_dzensun
};
