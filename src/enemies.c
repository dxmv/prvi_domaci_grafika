#include <enemies.h>
#include <math.h>
#include <stdlib.h>

static int width;
static int height;
static enemy_t enemies[MAX_ENEMIES];
static float spawn_timer = 0.0f;
static float spawn_interval = 2.0f;  // neprijatelj se pojavljuje svakih spawn_interval sekundi

// inicijalizujemo neprijatelje
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
    // azuriramo tajmer
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
                    case 0: 
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
                enemies[i].scale = 1.0f;
                enemies[i].alpha = 1.0f;
                enemies[i].is_dying = 0;
                enemies[i].active = 1;
                break;  // samo jedan neprijatelj po intervalu
            }
        }
    }
    
    // azuriramo sve aktivne neprijatelje
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
            
            // ako je alpha < 0, deaktiviramo neprijatelja
            if(e->alpha <= 0.0f)
            {
                e->active = 0;
                e->is_dying = 0;
                e->scale = 1.0f;
                e->alpha = 1.0f;
            }
            continue;  // ne pomeramo neprijatelja dok se ne umre
        }
        
        // izracunamo vektor smera ka player-u
        float dx = player->pos_x - e->pos_x;
        float dy = player->pos_y - e->pos_y;
        float distance = sqrtf(dx * dx + dy * dy);
        
        // normalizujemo i pomeramo se ka player-u
        if(distance > 0.1f)
        {
            dx /= distance;
            dy /= distance;
            
            e->pos_x += dx * e->speed * delta_time;
            e->pos_y += dy * e->speed * delta_time;
        }
        
        // deaktivujemo neprijatelje koji su preveliki za ekran (sigurnosna ciscenja)
        if(e->pos_x < -100 || e->pos_x > width + 100 ||
           e->pos_y < -100 || e->pos_y > height + 100)
        {
            e->active = 0;
        }
    }
}

// crtamo neprijatelje
void enemies_draw(rafgl_raster_t *raster)
{
    for(int i = 0; i < MAX_ENEMIES; i++)
    {
        if(!enemies[i].active)
            continue;
            
        enemy_t *e = &enemies[i];
        
        // skaliramo velicinu
        float scaled_size = e->size * e->scale;
        
        // izracunamo alpha kao 0-255 vrednost
        int alpha_value = (int)(e->alpha * 255.0f);
        if(alpha_value < 0) alpha_value = 0;
        if(alpha_value > 255) alpha_value = 255;
        
        // boja sa alpha
        uint32_t red_color = rafgl_RGB(255, 0, 0);
        
        // crtamo neprijatelja kao ispunjeni crveni pravougaonik sa scaling i alpha
        int half_size = (int)(scaled_size / 2.0f);
        int x1 = (int)(e->pos_x - half_size);
        int y1 = (int)(e->pos_y - half_size);
        int x2 = (int)(e->pos_x + half_size);
        int y2 = (int)(e->pos_y + half_size);
        
        // ispunjeni kvadrat sa alpha
        for(int y = y1; y <= y2; y++)
        {
            for(int x = x1; x <= x2; x++)
            {
                if(x >= 0 && x < raster->width && y >= 0 && y < raster->height)
                {
                    if(e->alpha >= 1.0f)
                    {
                        pixel_at_pm(raster, x, y).rgba = red_color;
                    }
                    else
                    {
                        rafgl_pixel_rgb_t bg = pixel_at_pm(raster, x, y);
                        int bg_r = bg.r;
                        int bg_g = bg.g;
                        int bg_b = bg.b;
                        
                        // blend red color with background
                        int final_r = (int)(255 * e->alpha + bg_r * (1.0f - e->alpha));
                        int final_g = (int)(0 * e->alpha + bg_g * (1.0f - e->alpha));
                        int final_b = (int)(0 * e->alpha + bg_b * (1.0f - e->alpha));
                        
                        pixel_at_pm(raster, x, y).rgba = rafgl_RGB(final_r, final_g, final_b);
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

