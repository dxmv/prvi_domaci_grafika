#include <collision.h>

int collision_player_asteroids(const player_t *player)
{
    const asteroid_t *asteroid_list = asteroids_get_all();

    for(int i = 0; i < MAX_ASTEROIDS; i++)
    {
        if(!asteroid_list[i].active)
        {
            continue;
        }

        float dx = player->pos_x - asteroid_list[i].pos_x;
        float dy = player->pos_y - asteroid_list[i].pos_y;
        float radius_sum = player->radius + asteroid_list[i].radius;

        if((dx * dx + dy * dy) <= (radius_sum * radius_sum))
        {
            return 1;
        }
    }

    return 0;
}
