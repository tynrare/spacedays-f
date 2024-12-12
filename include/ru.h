#include "raylib.h"
#include <stdlib.h>
#include <string.h>

#ifndef RULANG_H
#define RULANG_H

#ifndef FONT_TEXT
// ======
// FONT_TEXT
#define FONT_TEXT "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдеёжзийклмнопрстуфхцчшщъыьэюя!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHI\nJKLMNOPQRSTUVWXYZ[]^_`abcdefghijklmn\nopqrstuvwxyz{|}~¿♥"
// FONT_TEXT
// ======
#endif

#define PRINTTTEXT "Орнитоптер: летательный аппарат, способный зависать на месте. \nИмеет крылья, подобные птичьим"
#define RCOMPAS "Компас"
#define RSCANNER "Сканер"
#define RALLPRINTTEXT FONT_TEXT PRINTTTEXT RCOMPAS RSCANNER
    

static Font rufont = {0};
static int rufont_size = 42;

// === рутекст
#define DrawTextRu(t, x, y) DrawTextEx(rufont, t, (Vector2){x, y}, rufont_size, 2, RAYWHITE);

static int *CodepointRemoveDuplicates(int *codepoints, int codepointCount,
                                      int *codepointsResultCount) {
  int codepointsNoDupsCount = codepointCount;
  int *codepointsNoDups = (int *)calloc(codepointCount, sizeof(int));
  memcpy(codepointsNoDups, codepoints, codepointCount * sizeof(int));

  // Remove duplicates
  for (int i = 0; i < codepointsNoDupsCount; i++) {
    for (int j = i + 1; j < codepointsNoDupsCount; j++) {
      if (codepointsNoDups[i] == codepointsNoDups[j]) {
        for (int k = j; k < codepointsNoDupsCount; k++)
          codepointsNoDups[k] = codepointsNoDups[k + 1];

        codepointsNoDupsCount--;
        j--;
      }
    }
  }

  // NOTE: The size of codepointsNoDups is the same as original array but
  // only required positions are filled (codepointsNoDupsCount)

  *codepointsResultCount = codepointsNoDupsCount;
  return codepointsNoDups;
}

void draw_text_ru(const char *t, float x, float y, Color c) {
    DrawTextEx(rufont, t, (Vector2){ x, y }, rufont_size, 2,  c);
}

int load_rutopter() {
    static char *initial_text = FONT_TEXT;
    
    // Get codepoints from text
  int codepointCount = 0;
  int *codepoints = LoadCodepoints(initial_text, &codepointCount);

  // Removed duplicate codepoints to generate smaller font atlas
  int codepointsNoDupsCount = 0;
  int *codepointsNoDups = CodepointRemoveDuplicates(codepoints, codepointCount,
                                                    &codepointsNoDupsCount);
  UnloadCodepoints(codepoints);
  
     rufont = LoadFontEx("resources/monogram-extended.ttf", rufont_size, codepointsNoDups,
                    codepointsNoDupsCount);
    SetTextLineSpacing(28);
    
    return 0;
}

// Remove codepoint duplicates if requested
// WARNING: This process could be a bit slow if there text to process is very
// long


#endif