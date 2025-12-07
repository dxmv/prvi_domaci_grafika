#include <enemies.h>
#include <math.h>
#include <stdlib.h>

static int width;
static int height;
static enemy_t enemies[MAX_ENEMIES];
static float spawn_timer = 0.0f;
static float spawn_interval = 2.0f;  // Spawn enemy every 2 seconds

void enemies_init(int screen_width, int screen_height)
{
    width = screen_width;
    height = screen_height;
    
    // Initialize all enemies as inactive
    for(int i = 0; i < MAX_ENEMIES; i++)
    {
        enemies[i].active = 0;
    }
    
    spawn_timer = 0.0f;
}

void enemies_update(float delta_time, player_t *player)
{
    // Update spawn timer
    spawn_timer += delta_time;
    
    // Spawn new enemy if timer exceeded and we have inactive enemies
    if(spawn_timer >= spawn_interval)
    {
        spawn_timer = 0.0f;
        
        // Find an inactive enemy slot
        for(int i = 0; i < MAX_ENEMIES; i++)
        {
            if(!enemies[i].active)
            {
                // Spawn enemy at random edge of screen
                int edge = rand() % 4;  // 0=top, 1=right, 2=bottom, 3=left
                
                switch(edge)
                {
                    case 0: // Top
                        enemies[i].pos_x = (float)(rand() % width);
                        enemies[i].pos_y = -20.0f;
                        break;
                    case 1: // Right
                        enemies[i].pos_x = width + 20.0f;
                        enemies[i].pos_y = (float)(rand() % height);
                        break;
                    case 2: // Bottom
                        enemies[i].pos_x = (float)(rand() % width);
                        enemies[i].pos_y = height + 20.0f;
                        break;
                    case 3: // Left
                        enemies[i].pos_x = -20.0f;
                        enemies[i].pos_y = (float)(rand() % height);
                        break;
                }
                
                enemies[i].size = 15.0f;
                enemies[i].speed = 50.0f + (rand() % 50);  // Random speed between 50-100
                enemies[i].active = 1;
                break;  // Only spawn one enemy per interval
            }
        }
    }
    
    // Update all active enemies
    for(int i = 0; i < MAX_ENEMIES; i++)
    {
        if(!enemies[i].active)
            continue;
            
        enemy_t *e = &enemies[i];
        
        // Calculate direction vector to player
        float dx = player->pos_x - e->pos_x;
        float dy = player->pos_y - e->pos_y;
        float distance = sqrtf(dx * dx + dy * dy);
        
        // Normalize and move towards player
        if(distance > 0.1f)
        {
            dx /= distance;
            dy /= distance;
            
            e->pos_x += dx * e->speed * delta_time;
            e->pos_y += dy * e->speed * delta_time;
        }
        
        // Deactivate enemies that are too far off screen (safety cleanup)
        if(e->pos_x < -100 || e->pos_x > width + 100 ||
           e->pos_y < -100 || e->pos_y > height + 100)
        {
            e->active = 0;
        }
    }
}

void enemies_draw(rafgl_raster_t *raster)
{
    uint32_t red_color = rafgl_RGB(255, 0, 0);
    
    for(int i = 0; i < MAX_ENEMIES; i++)
    {
        if(!enemies[i].active)
            continue;
            
        enemy_t *e = &enemies[i];
        
        // Draw enemy as a filled red rectangle
        int half_size = (int)(e->size / 2.0f);
        int x1 = (int)(e->pos_x - half_size);
        int y1 = (int)(e->pos_y - half_size);
        int x2 = (int)(e->pos_x + half_size);
        int y2 = (int)(e->pos_y + half_size);
        
        // Draw filled rectangle
        for(int y = y1; y <= y2; y++)
        {
            for(int x = x1; x <= x2; x++)
            {
                if(x >= 0 && x < raster->width && y >= 0 && y < raster->height)
                {
                    pixel_at_pm(raster, x, y).rgba = red_color;
                }
            }
        }
    }
}

