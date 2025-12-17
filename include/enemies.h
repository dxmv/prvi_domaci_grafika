#ifndef ENEMIES_H_INCLUDED
#define ENEMIES_H_INCLUDED

#include <rafgl.h>
#include <player.h>

#define MAX_ENEMIES 10

typedef struct
{
    float pos_x;
    float pos_y;
    float size;
    float speed;
    float scale;      
    float alpha;      
    int is_dying;     
    int active;
} enemy_t;

void enemies_init(int screen_width, int screen_height);
void enemies_update(float delta_time, player_t *player);
void enemies_draw(rafgl_raster_t *raster);
enemy_t* enemies_get_all(void);

#endif

