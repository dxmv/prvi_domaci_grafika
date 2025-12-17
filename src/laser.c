#include <laser.h>
#include <math.h>

static laser_t lasers[MAX_LASERS];
static const float LASER_SPEED = 500.0f;
static const float LASER_LENGTH = 20.0f;
static const float LASER_EXPANSION_RATE = 5.0f;
static const float LASER_MAX_WIDTH = 50.0f;
static const int THICKNESS = 1;


void lasers_init(void)
{
    // svi lasersi su neaktivni i expanduju se
    for(int i = 0; i < MAX_LASERS; i++)
    {
        lasers[i].active = 0;
        lasers[i].is_expanding = 1;
        lasers[i].current_width = 1.0f;
    }
}

void lasers_spawn(const player_t *player)
{
    // nadji prvi neaktivan laser
    for(int i = 0; i < MAX_LASERS; i++)
    {
        if(!lasers[i].active)
        {
            // laser na poziciji playera
            lasers[i].pos_x = player->pos_x;
            lasers[i].pos_y = player->pos_y;
            
            // vel je u smeru playera
            lasers[i].vel_x = cosf(player->angle) * LASER_SPEED;
            lasers[i].vel_y = sinf(player->angle) * LASER_SPEED;
            
            lasers[i].active = 1;
            lasers[i].is_expanding = 1;
            lasers[i].current_width = 1.0f;
            break; // samo 1 laser
        }
    }
}

void lasers_update(float delta_time, int screen_width, int screen_height)
{
    // update samo aktivni
    for(int i = 0; i < MAX_LASERS; i++)
    {
        if(!lasers[i].active)
        {
            continue;
        }
        
        laser_t *laser = &lasers[i];
        
        
        laser->pos_x += laser->vel_x * delta_time;
        laser->pos_y += laser->vel_y * delta_time;

        // expandujemo
        if(laser->is_expanding)
        {
            laser->current_width += LASER_EXPANSION_RATE * delta_time;
            if(laser->current_width >= LASER_MAX_WIDTH)
            {
                laser->is_expanding = 0;
            }
        }
        
        // ako je van ekrana, deaktiviraj
        if(laser->pos_x < 0 || laser->pos_x > screen_width ||
           laser->pos_y < 0 || laser->pos_y > screen_height)
        {
            laser->active = 0;
        }
    }
}

void lasers_draw(rafgl_raster_t *raster)
{
    float brightness[4] = {1.0f, 0.6f, 0.25f, 0.15f};
    for(int i = 0; i < MAX_LASERS; i++)
    {
        if(!lasers[i].active)
        {
            continue;
        }
        
        laser_t *laser = &lasers[i];
        
        float angle = atan2f(laser->vel_y, laser->vel_x);
        
        float end_x = laser->pos_x + cosf(angle) * LASER_LENGTH * laser->current_width;
        float end_y = laser->pos_y + sinf(angle) * LASER_LENGTH * laser->current_width;

        // glow efekat - crtamo od spolja ka unutra (dim -> bright)
        for(int layer = 3; layer >= 0; layer--)
        {
            float brightness_factor = brightness[layer];
            uint32_t layer_color = rafgl_RGB(
                (int)(255 * brightness_factor), 
                (int)(0 ), 
                (int)(0 )
            );
            
            int thickness = layer * THICKNESS;
            for(int j = -thickness; j <= thickness; j++)
            {
                float offset_x = cosf(angle + M_PI / 2) * j;
                float offset_y = sinf(angle + M_PI / 2) * j;
                rafgl_raster_draw_line(raster, 
                    (int)(laser->pos_x + offset_x), 
                    (int)(laser->pos_y + offset_y), 
                    (int)(end_x + offset_x), 
                    (int)(end_y + offset_y), 
                    layer_color);
            }
        }
    }
}

laser_t* lasers_get_all(void)
{
    return lasers;
}
