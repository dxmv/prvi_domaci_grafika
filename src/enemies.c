#include <enemies.h>
#include <math.h>
#include <stdlib.h>
#include <screen_shake.h>

static int width;
static int height;
static enemy_t enemies[MAX_ENEMIES];
static float spawn_timer = 0.0f;
static float spawn_interval = 2.0f;  // neprijatelj se pojavljuje svakih spawn_interval sekundi

void enemies_init(int screen_width, int screen_height)
{
    width = screen_width;
    height = screen_height;
    
    for(int i = 0; i < MAX_ENEMIES; i++)
    {
        enemies[i].active = 0;
        enemies[i].scale = 1.0f;
        enemies[i].alpha = 1.0f;
        enemies[i].is_dying = 0;
    }
    
    spawn_timer = 0.0f;
}

void enemies_update(float delta_time, player_t *player)
{
    spawn_timer += delta_time;
    
    // ako je tajmer veci od spawn_interval i imamo neaktivne neprijatelje, spawnujemo novog neprijatelja
    if(spawn_timer >= spawn_interval)
    {
        spawn_timer = 0.0f;
        
        for(int i = 0; i < MAX_ENEMIES; i++)
        {
            if(!enemies[i].active)
            {
                // spawnujemo neprijatelja na random ivici ekrana
                int edge = rand() % 4;  
                
                switch(edge)
                {
                    case 0: // gore
                        enemies[i].pos_x = (float)(rand() % width);
                        enemies[i].pos_y = -20.0f;
                        break;
                    case 1: // desno
                        enemies[i].pos_x = width + 20.0f;
                        enemies[i].pos_y = (float)(rand() % height);
                        break;
                    case 2: // dole
                        enemies[i].pos_x = (float)(rand() % width);
                        enemies[i].pos_y = height + 20.0f;
                        break;
                    case 3: // levo
                        enemies[i].pos_x = -20.0f;
                        enemies[i].pos_y = (float)(rand() % height);
                        break;
                }
                
                enemies[i].size = 15.0f;
                enemies[i].speed = 50.0f + (rand() % 50);  // 50 - 100 brzina
                enemies[i].scale = 1.0f;
                enemies[i].alpha = 1.0f;
                enemies[i].is_dying = 0;
                enemies[i].active = 1;
                break;  // samo jedan neprijatelj po intervalu
            }
        }
    }
    
    for(int i = 0; i < MAX_ENEMIES; i++)
    {
        if(!enemies[i].active)
            continue;
            
        enemy_t *e = &enemies[i];
        
        // animacija 
        if(e->is_dying)
        {
            e->scale -= 0.05f;
            e->alpha -= 0.05f;
            
            if(e->alpha <= 0.0f)
            {
                e->active = 0;
                e->is_dying = 0;
                e->scale = 1.0f;
                e->alpha = 1.0f;
            }
            continue;
        }
        
        // izracunamo vektor smera ka playeru
        float dx = player->pos_x - e->pos_x;
        float dy = player->pos_y - e->pos_y;
        float distance = sqrtf(dx * dx + dy * dy);
        
        // normalizujemo i pomeramo se ka playeru
        if(distance > 0.1f)
        {
            dx /= distance;
            dy /= distance;
            
            e->pos_x += dx * e->speed * delta_time;
            e->pos_y += dy * e->speed * delta_time;
        }
        
        if(e->pos_x < -100 || e->pos_x > width + 100 ||
           e->pos_y < -100 || e->pos_y > height + 100)
        {
            e->active = 0;
        }
    }
}

void enemies_draw(rafgl_raster_t *raster)
{
    float shake_x, shake_y;
    screen_shake_get_offset(&shake_x, &shake_y);
    for(int i = 0; i < MAX_ENEMIES; i++)
    {
        if(!enemies[i].active)
            continue;
            
        enemy_t *e = &enemies[i];
        
        float scaled_size = e->size * e->scale;
        
        rafgl_pixel_rgb_t red_color = {.rgba = rafgl_RGB(255, 0, 0)};
        
        int half_size = (int)(scaled_size / 2.0f);
        int x1 = (int)(e->pos_x + shake_x - half_size);
        int y1 = (int)(e->pos_y + shake_y - half_size);
        int x2 = (int)(e->pos_x + shake_x + half_size);
        int y2 = (int)(e->pos_y + shake_y + half_size);
        
        for(int y = y1; y <= y2; y++)
        {
            for(int x = x1; x <= x2; x++)
            {
                if(x >= 0 && x < raster->width && y >= 0 && y < raster->height)
                {
                    if(e->alpha >= 1.0f)
                    {
                        pixel_at_pm(raster, x, y).rgba = red_color.rgba;
                    }
                    else
                    {
                        rafgl_pixel_rgb_t bg = pixel_at_pm(raster, x, y);
                        pixel_at_pm(raster, x, y) = rafgl_lerppix(bg, red_color, e->alpha);
                    }
                }
            }
        }
    }
}

enemy_t* enemies_get_all(void)
{
    return enemies;
}
