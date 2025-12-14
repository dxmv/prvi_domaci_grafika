#ifndef ITEM_H_INCLUDED
#define ITEM_H_INCLUDED

#include <rafgl.h>

#define MAX_ITEMS 3

typedef struct
{
    float pos_x;
    float pos_y;
    int active;
    int width;
    int height;
    float scale;
    int is_growing;
} item_t;

void items_init(void);
void items_update(float delta_time);
void items_spawn(rafgl_raster_t *raster);
void items_draw(rafgl_raster_t *raster);
void items_cleanup(void);
item_t* items_get_all(void);

#endif