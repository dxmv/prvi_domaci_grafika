#ifndef HEART_H_INCLUDED
#define HEART_H_INCLUDED

#include <rafgl.h>
#include <player.h>

typedef struct
{
    int pos_x;
    int pos_y;
} heart_t;

void heart_init(player_t *player);
void heart_update(player_t *player);
void heart_draw(player_t *player, rafgl_raster_t *raster);
void heart_cleanup(void);

#endif