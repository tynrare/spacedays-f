#include "../tynpool.h"
#include "locomotions.h"

#ifndef GTBOX_DZENSUN_H
#define GTBOX_DZENSUN_H

#define GTBOX_DZENSUN_POOLD_ 64
#define GTBOX_DZENSUN_AIMSD_ 8
#define GTBOX_DZENSUN_SUMMONDIST_ 64
#define GTBOX_DZENSUN_FREC_ 0.001

typedef struct GtboxDzenSun {
  struct TynPool *pool;
  struct TynPool *aims;
  double elapsed;
} GtboxDzenSun;

void gtbox_dzensun_init(GtboxDzenSun *tchant_dzen_sun) {
  tchant_dzen_sun->pool =
      typool_allocate(GTBOX_DZENSUN_POOLD_, sizeof(float) * 6);
  tchant_dzen_sun->aims =
      typool_allocate(GTBOX_DZENSUN_AIMSD_, sizeof(float) * 2);

  TynPoolCell *cell = tynpool_cell_alloc(tchant_dzen_sun->aims);
  float *r = cell->point;
  float *x = r + 0;
  float *y = r + 1;

  *x = 0.0;
  *y = 0.0;
}

void gtbox_dzensun_dispose(GtboxDzenSun *tchant_dzen_sun) {
  tynpool_dispose(tchant_dzen_sun->pool);
  tynpool_dispose(tchant_dzen_sun->aims);
}

void gtbox_dzensun_init_particle(TynPoolCell *c, float cx, float cy) {
  float *r = c->point;
  float *x = r + 0;
  float *y = r + 1;
  float *dx = r + 2;
  float *dy = r + 3;
  float *e = r + 4;
  float *intensity = x + 5;
  float rx = GetRandomValue(-256, 256);
  float ry = GetRandomValue(-256, 256);
  Vector2 dist = {rx, ry};
  Vector2 d = Vector2Normalize(dist);
  d = Vector2Scale(d, GetRandomValue(1, 5) * 0.1 + 1);
  *x = cx + d.x * GTBOX_DZENSUN_SUMMONDIST_;
  *y = cy + d.y * GTBOX_DZENSUN_SUMMONDIST_;
  *dx = d.x;
  *dy = d.y;
  *e = 0.0;
  *intensity = 0.0;
}

void gtbox_dzensun_step(GtboxDzenSun *gtbox_dzen_sun,
                        void (*callback)(float *, float *, float *, float *,
                                         float *)) {
  gtbox_dzen_sun->elapsed += GetFrameTime();

  TynPoolCell *aims_p = gtbox_dzen_sun->aims->active;
  float *aims_x = aims_p->point + 0;
  float *aims_y = aims_x + 1;
  Vector2 vaims = {*aims_x, *aims_y};

  float watts = 1.0;

  for (TynPoolCell *p = gtbox_dzen_sun->pool->active; p; p = p->next) {
    float *x = p->point;
    float *y = x + 1;
    float *dx = x + 2;
    float *dy = x + 3;
    float *e = x + 4;
    float *intensity = x + 5;

    *e += GetFrameTime();
    *intensity = fminf(*e, 1.0);

    if (*e > 0.4) {
      watts = 1.0;
      locomotion_pull(x, y, dx, dy, vaims); // pull in
    } else {
      watts = 2.0;
      locomotion_push(x, y, dx, dy, vaims); // push out
    }

    *x += *dx * watts;
    *y += *dy * watts;

    Vector2 delta = {vaims.x - *x, vaims.y - *y};
    float dist = Vector2Length(delta);
    float deta = dist / GTBOX_DZENSUN_SUMMONDIST_ - 0.5;
    *intensity = Clamp(0.0, 1.0, fminf(*intensity, deta - 0.1));

    if (deta < 0) {
			gtbox_dzensun_init_particle(p, *aims_x, *aims_y);
      // tynpool_cell_free(gtbox_dzen_sun->pool, p);
      continue;
    }

    // loc_viewport_bound_teleport(x, y);
    callback(x, y, dx, dy, intensity);
  }

  if (gtbox_dzen_sun->pool->idle &&
      gtbox_dzen_sun->elapsed > GTBOX_DZENSUN_FREC_) {
    gtbox_dzen_sun->elapsed = 0;
    TynPoolCell *cell = tynpool_cell_alloc(gtbox_dzen_sun->pool);
		gtbox_dzensun_init_particle(cell, *aims_x, *aims_y);
  }
}

#endif
