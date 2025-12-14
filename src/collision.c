#include <collision.h>
#include <math.h>
#include <stdio.h>

// Generic collision check using distance formula
// Returns 1 if two circles overlap, 0 otherwise
int check_collision(float x1, float y1, float r1, float x2, float y2, float r2)
{
    float dx = x1 - x2;
    float dy = y1 - y2;
    float radius_sum = r1 + r2;
    
    // Check if distance squared is less than or equal to radius sum squared
    // This avoids the expensive sqrt operation
    return (dx * dx + dy * dy) <= (radius_sum * radius_sum);
}

// Check all active lasers against all active enemies
void check_laser_enemy_collisions(void)
{
    laser_t *lasers = lasers_get_all();
    enemy_t *enemies = enemies_get_all();
    
    // Laser radius for collision detection (lasers are thin, so small radius)
    const float LASER_RADIUS = 3.0f;
    
    for(int i = 0; i < MAX_LASERS; i++)
    {
        if(!lasers[i].active)
            continue;
            
        for(int j = 0; j < MAX_ENEMIES; j++)
        {
            if(!enemies[j].active || enemies[j].is_dying)
                continue;  // skip inactive or already dying enemies
                
            // Use enemy.size as radius (size is the full diameter, so divide by 2)
            float enemy_radius = enemies[j].size / 2.0f;
            
            if(check_collision(lasers[i].pos_x, lasers[i].pos_y, LASER_RADIUS,
                             enemies[j].pos_x, enemies[j].pos_y, enemy_radius))
            {
                printf("Hit!\n");
                
                // Start the death animation for the enemy
                enemies[j].is_dying = 1;
                
                // Deactivate the laser immediately
                lasers[i].active = 0;
                
                break;  // laser can only hit one enemy, so break inner loop
            }
        }
    }
}

// Check player against all active enemies
void check_player_enemy_collisions(player_t *player)
{
    enemy_t *enemies = enemies_get_all();
    
    for(int i = 0; i < MAX_ENEMIES; i++)
    {
        if(!enemies[i].active || enemies[i].is_dying)
            continue;  // skip inactive or dying enemies
            
        // Use enemy.size as radius
        float enemy_radius = enemies[i].size / 2.0f;
        
        if(check_collision(player->pos_x, player->pos_y, player->radius,
                         enemies[i].pos_x, enemies[i].pos_y, enemy_radius))
        {
            printf("Hit!\n");
            
            // Set hit timer to flash red for 20 frames
            player->hit_timer = 20;
            player->health--;
            
            // Start death animation for the enemy
            enemies[i].is_dying = 1;
        }
    }
}
