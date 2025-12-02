#ifndef ASTEROIDS_H_INCLUDED
#define ASTEROIDS_H_INCLUDED

#include <rafgl.h>

#define MAX_ASTEROIDS 3

typedef struct
{
    float pos_x;
    float pos_y;
    float vel_x;
    float vel_y;
    float rotation;
    float rotation_speed;
    float radius;
    int active;
} asteroid_t;

void asteroids_init(int screen_width, int screen_height);
void asteroids_update(float delta_time);
void asteroids_draw(rafgl_raster_t *raster);

#endif
