#ifndef PLANETS_H_INCLUDED
#define PLANETS_H_INCLUDED

#include <rafgl.h>

#define MAX_PLANETS 2

typedef struct
{
    float pos_x;
    float pos_y;
    float radius;
    uint32_t color;
    // Orbital parameters
    float orbit_center_x;
    float orbit_center_y;
    float orbit_radius;
    float orbit_speed;
    float orbit_angle;
    // animacija
    float current_frame;
    float animation_speed;
    int sprite_index;  // koji sprite koristi (0 ili 1)
} planet_t;

void planets_init(int screen_width, int screen_height);
void planets_update(float delta_time);
void planets_draw(rafgl_raster_t *raster);
void planets_cleanup(void);

#endif
