#include <laser.h>
#include <math.h>

static laser_t lasers[MAX_LASERS];
static const float LASER_SPEED = 500.0f;
static const float LASER_LENGTH = 15.0f;

void lasers_init(void)
{
    for(int i = 0; i < MAX_LASERS; i++)
    {
        lasers[i].active = 0;
    }
}

void lasers_spawn(const player_t *player)
{
    // Find an inactive laser
    for(int i = 0; i < MAX_LASERS; i++)
    {
        if(!lasers[i].active)
        {
            // Spawn laser at player's position
            lasers[i].pos_x = player->pos_x;
            lasers[i].pos_y = player->pos_y;
            
            // Set velocity in the direction the player is facing
            lasers[i].vel_x = cosf(player->angle) * LASER_SPEED;
            lasers[i].vel_y = sinf(player->angle) * LASER_SPEED;
            
            lasers[i].active = 1;
            break; // Only spawn one laser per call
        }
    }
}

void lasers_update(float delta_time, int screen_width, int screen_height)
{
    for(int i = 0; i < MAX_LASERS; i++)
    {
        if(!lasers[i].active)
        {
            continue;
        }
        
        laser_t *laser = &lasers[i];
        
        // Update position
        laser->pos_x += laser->vel_x * delta_time;
        laser->pos_y += laser->vel_y * delta_time;
        
        // Deactivate if off screen
        if(laser->pos_x < 0 || laser->pos_x > screen_width ||
           laser->pos_y < 0 || laser->pos_y > screen_height)
        {
            laser->active = 0;
        }
    }
}

void lasers_draw(rafgl_raster_t *raster)
{
    uint32_t laser_color = rafgl_RGB(255, 0, 0); 
    
    for(int i = 0; i < MAX_LASERS; i++)
    {
        if(!lasers[i].active)
        {
            continue;
        }
        
        laser_t *laser = &lasers[i];
        
        // Calculate the angle of the laser's movement
        float angle = atan2f(laser->vel_y, laser->vel_x);
        
        // Draw laser as a line in the direction of movement
        float end_x = laser->pos_x + cosf(angle) * LASER_LENGTH;
        float end_y = laser->pos_y + sinf(angle) * LASER_LENGTH;
        
        rafgl_raster_draw_line(raster, 
                               (int)laser->pos_x, (int)laser->pos_y,
                               (int)end_x, (int)end_y,
                               laser_color);
    }
}
