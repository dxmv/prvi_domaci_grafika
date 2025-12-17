#include <player.h>
#include <math.h>

void player_init(player_t *player, float width, float height)
{
    player->pos_x = width * 0.5f;
    player->pos_y = height * 0.5f;
    player->angle = 0.0f;
    player->speed = 250.0f;
    player->rotation_speed = 5.0f;
    player->radius = 20.0f;
    player->hit_timer = 0;
    player->heal_timer = 0;
    player->health = 4;
    player->score = 0;
}

void player_update(player_t *player, float delta_time, const rafgl_game_data_t *game_data)
{
    if(player->hit_timer > 0)
    {
        player->hit_timer--;
    }
    
    if(player->heal_timer > 0)
    {
        player->heal_timer--;
    }
    
    if(game_data->keys_down[RAFGL_KEY_A])
    {
        player->angle -= player->rotation_speed * delta_time;
    }
    else if(game_data->keys_down[RAFGL_KEY_D])
    {
        player->angle += player->rotation_speed * delta_time;
    }

    if(game_data->keys_down[RAFGL_KEY_W])
    {
        player->pos_x += cosf(player->angle) * player->speed * delta_time;
        player->pos_y += sinf(player->angle) * player->speed * delta_time;
    }
    else if(game_data->keys_down[RAFGL_KEY_S])
    {
        player->pos_x -= cosf(player->angle) * player->speed * delta_time;
        player->pos_y -= sinf(player->angle) * player->speed * delta_time;
    }
}

void player_draw(const player_t *player, rafgl_raster_t *raster)
{
    float cos_a = cosf(player->angle);
    float sin_a = sinf(player->angle);

    int x0 = player->pos_x + player->radius * cos_a;
    int y0 = player->pos_y + player->radius * sin_a;

    int x1 = player->pos_x + (-player->radius * cos_a - player->radius * 0.5f * sin_a);
    int y1 = player->pos_y + (-player->radius * sin_a + player->radius * 0.5f * cos_a);

    int x2 = player->pos_x + (-player->radius * cos_a + player->radius * 0.5f * sin_a);
    int y2 = player->pos_y + (-player->radius * sin_a - player->radius * 0.5f * cos_a);

    uint32_t ship_color;
    if(player->hit_timer > 0)
    {
        // crveni hit
        ship_color = rafgl_RGB(255, 0, 0);
    }
    else if(player->heal_timer > 0)
    {
        // zeleni heal
        ship_color = rafgl_RGB(0, 255, 0);
    }
    else
    {
        ship_color = rafgl_RGB(255, 255, 255);
    }

    
    rafgl_raster_draw_line(raster, x0, y0, x1, y1, ship_color);
    rafgl_raster_draw_line(raster, x1, y1, x2, y2, ship_color);
    rafgl_raster_draw_line(raster, x2, y2, x0, y0, ship_color);
}
