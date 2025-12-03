#ifndef LASER_H_INCLUDED
#define LASER_H_INCLUDED

#include <rafgl.h>
#include <player.h>

#define MAX_LASERS 10

typedef struct
{
    float pos_x;
    float pos_y;
    float vel_x;
    float vel_y;
    int active;
} laser_t;

void lasers_init(void);
void lasers_spawn(const player_t *player);
void lasers_update(float delta_time, int screen_width, int screen_height);
void lasers_draw(rafgl_raster_t *raster);

#endif
