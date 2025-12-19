#include <collision.h>
#include <heart.h>
#include <math.h>
#include <stdio.h>
#include <particles.h>
#include <screen_shake.h>

// kolizija krugova
int check_collision(float x1, float y1, float r1, float x2, float y2, float r2)
{
    float dx = x1 - x2;
    float dy = y1 - y2;
    float radius_sum = r1 + r2;
    
    return (dx * dx + dy * dy) <= (radius_sum * radius_sum);
}

// svi aktivni laseri i svi aktivni neprijatelji
void check_laser_enemy_collisions(player_t *player)
{
    laser_t *lasers = lasers_get_all();
    enemy_t *enemies = enemies_get_all();
    
    const float LASER_RADIUS = 3.0f;
    
    for(int i = 0; i < MAX_LASERS; i++)
    {
        if(!lasers[i].active)
            continue;
            
        for(int j = 0; j < MAX_ENEMIES; j++)
        {
            if(!enemies[j].active || enemies[j].is_dying)
                continue;  
                
            float enemy_radius = enemies[j].size / 2.0f;
            
            if(check_collision(lasers[i].pos_x, lasers[i].pos_y, LASER_RADIUS,
                             enemies[j].pos_x, enemies[j].pos_y, enemy_radius))
            {
                printf("Hit! +1 point\n");
                player->score++;
                
                // animacija umiranja
                enemies[j].is_dying = 1;
                
                lasers[i].active = 0;
                
                particles_spawn(enemies[j].pos_x, enemies[j].pos_y, 20, 10.0f);
                screen_shake_trigger(0.25f, 8.0f);
                
                break;  // laser moze samo da udari 1 neprijatelja
            }
        }
    }
}

// player i svi aktivni neprijatelji
void check_player_enemy_collisions(player_t *player)
{
    enemy_t *enemies = enemies_get_all();
    
    for(int i = 0; i < MAX_ENEMIES; i++)
    {
        if(!enemies[i].active || enemies[i].is_dying)
            continue;  
            
        float enemy_radius = enemies[i].size / 2.0f;
        
        if(check_collision(player->pos_x, player->pos_y, player->radius,
                         enemies[i].pos_x, enemies[i].pos_y, enemy_radius))
        {
            printf("Hit!\n");
            
            player->hit_timer = 20;
            player->health--;
            if(player->health < 0){
                player->health = 0;
            }
            screen_shake_trigger(0.35f, 12.0f);
            
            // animacija umiranja
            enemies[i].is_dying = 1;
        }
    }
}

void check_player_item_collisions(player_t *player)
{
    item_t *items = items_get_all();
    for(int i = 0; i < MAX_ITEMS; i++)
    {
        if(!items[i].active)
            continue;
        if(check_collision(player->pos_x, player->pos_y, player->radius,
                         items[i].pos_x, items[i].pos_y, items[i].width / 2.0f))
        {
            if(player->health < MAX_HEARTS)
            {
                printf("+1 health\n");
                player->health++;
                // heal animacija
                player->heal_timer = 20;
            }
            items[i].active = 0;
        }
    }
}
