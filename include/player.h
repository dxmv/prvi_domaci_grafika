#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include <rafgl.h>

typedef struct
{
    float pos_x;
    float pos_y;
    float angle;
    float speed;
    float rotation_speed;
    float radius;
    int hit_timer;  // frames remaining for red flash effect
    int health;
} player_t;

void player_init(player_t *player, float start_x, float start_y);
void player_update(player_t *player, float delta_time, const rafgl_game_data_t *game_data);
void player_draw(const player_t *player, rafgl_raster_t *raster);

#endif
