#include <stars.h>
#include <math.h>
#include <stdlib.h>
#include <screen_shake.h>

static int width;
static int height;
static star_t stars[MAX_STARS];
static float time_accumulator = 0.0f;

void stars_init(int screen_width, int screen_height)
{
    width = screen_width;
    height = screen_height;

    rafgl_pixel_rgb_t yellow = {.rgba = rafgl_RGB(255, 255, 0)};
    rafgl_pixel_rgb_t white = {.rgba = rafgl_RGB(255, 255, 255)};

    rafgl_pixel_rgb_t colors[2] = {yellow, white};

    for(int i = 0; i < MAX_STARS; i++)
    {
        // random stvari za svaku zvezdu
        stars[i].pos_x = (float)(rand() % width);
        stars[i].pos_y = (float)(rand() % height);
        stars[i].vel_x = ((float)rand() / RAND_MAX - 0.5f) * 10.0f;
        stars[i].vel_y = ((float)rand() / RAND_MAX - 0.5f) * 10.0f;
        stars[i].brightness = 0.5f + ((float)rand() / RAND_MAX) * 0.5f;
        stars[i].twinkle_speed = 0.5f + ((float)rand() / RAND_MAX) * 2.0f;
        stars[i].size = 1.0f + ((float)rand() / RAND_MAX) * 2.0f;
        stars[i].color = colors[rand() % 2].rgba;
    }
}

void stars_update(float delta_time)
{
    time_accumulator += delta_time;
    
    for(int i = 0; i < MAX_STARS; i++)
    {
        star_t *s = &stars[i];
        
        // pomeranje zvezda
        s->pos_x += s->vel_x * delta_time;
        s->pos_y += s->vel_y * delta_time;
        float precnik = s->size * M_PI*2;
        
        // ivice ekrana - odbijanje
        if(s->pos_x < 0)
        {
            s->pos_x = precnik;
            s->vel_x = -s->vel_x; 
        }
        if(s->pos_x + precnik > width)
        {
            s->pos_x = width - precnik;
            s->vel_x = -s->vel_x; 
        }
        if(s->pos_y < 0)
        {
            s->pos_y = precnik;
            s->vel_y = -s->vel_y; 
        }
        if(s->pos_y + precnik > height)
        {
            s->pos_y = height - precnik;
            s->vel_y = -s->vel_y; 
        }
        
        // menjanje brightnessa 
        float phase = time_accumulator * s->twinkle_speed;
        s->brightness = sinf(phase) * 0.3f + 0.5f;
    }
}


// crtamo zvezde 
void stars_draw(rafgl_raster_t *raster)
{
    float shake_x, shake_y;
    screen_shake_get_offset(&shake_x, &shake_y);
    for(int i = 0; i < MAX_STARS; i++)
    {
        star_t *star = &stars[i];
        
        rafgl_pixel_rgb_t color = {.rgba = star->color};
        
        float brightness = star->brightness;
        int r_final = (int)(color.r * brightness);
        int g_final = (int)(color.g * brightness);
        int b_final = (int)(color.b * brightness);
        
        uint32_t star_color = rafgl_RGB(r_final, g_final, b_final);
        // crtamo zvezdu samo ako je unutar ekrana
        if(star->pos_x < 0 || star->pos_x > width || star->pos_y < 0 || star->pos_y > height)
        {
            continue;
        }
        // crtamo ili kao krug ili kao pixel
        int draw_x = (int)(star->pos_x + shake_x);
        int draw_y = (int)(star->pos_y + shake_y);
        if(draw_x < 0 || draw_x >= raster->width || draw_y < 0 || draw_y >= raster->height)
        {
            continue;
        }
        if(star->size > 1.5f)
        {
            rafgl_raster_draw_circle(raster, draw_x, draw_y, (int)star->size, star_color);
        }
        else
        {
            pixel_at_pm(raster, draw_x, draw_y).rgba = star_color;
        }
    }
}
