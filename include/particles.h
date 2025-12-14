#ifndef PARTICLES_H
#define PARTICLES_H

#include <rafgl.h>

#define MAX_PARTICLES 500

typedef struct
{
    float x, y, dx, dy;
    int life;
} particle_t;

void particles_init(int screen_width, int screen_height);
void particles_spawn(float x, float y, int count, float radius);
void particles_update(float delta_time);
void particles_draw(rafgl_raster_t *raster);
void particles_cleanup(void);

#endif