#ifndef STARS_H_INCLUDED
#define STARS_H_INCLUDED

#include <rafgl.h>

#define MAX_STARS 200

typedef struct
{
    float pos_x;
    float pos_y;
    float vel_x;
    float vel_y;
    float brightness;
    float twinkle_speed;
    float size;
} star_t;

void stars_init(int screen_width, int screen_height);
void stars_update(float delta_time);
void stars_draw(rafgl_raster_t *raster);

#endif
