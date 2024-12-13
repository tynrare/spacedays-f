#include <stdlib.h>

#ifndef TYNPOOL_H
#define TYNPOOL_H

typedef struct TynPoolCell {
    int index;
    void *point;
    struct TynPoolCell *prev;
    struct TynPoolCell *next;
} TynPoolCell;

typedef struct TynPool {
    int                 cells;
    int                 cellsize;
    void               *mem_pool;
    struct TynPoolCell *cells_pool;
    struct TynPoolCell *idle;
    struct TynPoolCell *active;
} TynPool;

TynPool *typool_allocate(int cells, int cellsize) {
    TynPool *tp = malloc(sizeof(TynPool));
    tp->cells = cells;
    tp->cellsize = cellsize;
    tp->mem_pool = malloc(cellsize * cells);
    tp->cells_pool = malloc(sizeof(TynPoolCell) * cells);
    
    for (int i = 0; i < cells; i++) {
        TynPoolCell *tpc = tp->cells_pool + i;
        tpc->index = i;
        tpc->point = tp->mem_pool + (i * cellsize);
        
        tpc->prev = 0;
        tpc->next = 0;
        
        if (i > 0) {
            tpc->prev = tp->cells_pool + i - 1;
        } 
        if (i < cells - 1) {
            tpc->next = tp->cells_pool + i + 1;
        }
        
    }
    
    tp->idle = tp->cells_pool;
    tp->active = 0;
    
    return tp;
};

void tynpool_dispose(TynPool *tp) {
    free(tp->mem_pool);
    free(tp->cells_pool);
    free(tp);
};

TynPoolCell *tynpool_cell_alloc(TynPool *tp) {
    TynPoolCell *tpc = tp->idle;
    if (!tpc) {
        return 0;
    }
    
    // a
    tp->idle = tpc->next;
    if (tp->idle) {
        tp->idle->prev = 0;
    }
    
    // b
    if (tp->active) {
        tp->active->prev = tpc;
    }
    
    tpc->next = tp->active;
    tpc->prev = 0;
    tp->active = tpc;
    
    return tpc;
};

void tynpool_cell_free(TynPool *tp, TynPoolCell *tpc) {
    // a
    if (tp->active == tpc) {
        tp->active = tpc->next;
    }
    
    TynPoolCell *next = tpc->next;
    TynPoolCell *prev = tpc->prev;
    if (next && prev) {
        prev->next = next;
        next->prev = prev;
    } else if (prev) {
        prev->next = 0;
    } else if (next) {
        next->prev = 0;
    }

    // b
    if (tp->idle) {
        tp->idle->prev = tpc;
    }
    tpc->next = tp->idle;
    tpc->prev = 0;
    tp->idle = tpc;
    
    return;
};

// idle iterations over active cells to make it ordered.
//void tynpool_shuffle(TynPool *tp) {}

#endif