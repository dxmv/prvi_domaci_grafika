#include <stars.h>
#include <math.h>
#include <stdlib.h>

static int width;
static int height;
static star_t stars[MAX_STARS];
static float time_accumulator = 0.0f;

// inicijalizujemo zvezde
void stars_init(int screen_width, int screen_height)
{
    width = screen_width;
    height = screen_height;

    for(int i = 0; i < MAX_STARS; i++)
    {
        stars[i].pos_x = (float)(rand() % width);
        stars[i].pos_y = (float)(rand() % height);
        stars[i].vel_x = ((float)rand() / RAND_MAX - 0.5f) * 20.0f;
        stars[i].vel_y = ((float)rand() / RAND_MAX - 0.5f) * 20.0f;
        stars[i].brightness = 0.5f + ((float)rand() / RAND_MAX) * 0.5f;
        stars[i].twinkle_speed = 0.5f + ((float)rand() / RAND_MAX) * 2.0f;
        stars[i].size = 1.0f + ((float)rand() / RAND_MAX) * 2.0f;
    }
}


// update zvezda
void stars_update(float delta_time)
{
    time_accumulator += delta_time;
    
    for(int i = 0; i < MAX_STARS; i++)
    {
        star_t *s = &stars[i];
        
        // pomeranje zvezda
        s->pos_x += s->vel_x * delta_time;
        s->pos_y += s->vel_y * delta_time;
        
        // ivice ekrana
        if(s->pos_x < 0)
        {
            s->pos_x = width;
        }
        if(s->pos_x > width)
        {
            s->pos_x = 0;
        }
        if(s->pos_y < 0)
        {
            s->pos_y = height;
        }
        if(s->pos_y > height)
        {
            s->pos_y = 0;
        }
        
        // zvezde svetle sa sinusnom funkcijom
        float phase = time_accumulator * s->twinkle_speed;
        s->brightness = 0.5f + 0.5f * sinf(phase);
    }
}

// crtamo zvezde
void stars_draw(rafgl_raster_t *raster)
{
    for(int i = 0; i < MAX_STARS; i++)
    {
        star_t *s = &stars[i];
        
        // boja na osnovu brightnessa
        int intensity = (int)(stars[i].brightness * 255);
        uint32_t star_color = rafgl_RGB(intensity, intensity, intensity);
        
        if(s->size > 1.5f)
        {
            // vece zvezde
            rafgl_raster_draw_circle(raster, (int)s->pos_x, (int)s->pos_y, (int)s->size, star_color);
        }
        else
        {
            // pixel zvezde
            if(s->pos_x >= 0 && s->pos_x < width && s->pos_y >= 0 && s->pos_y < height)
            {
                pixel_at_pm(raster, (int)s->pos_x, (int)s->pos_y).rgba = star_color;
            }
        }
    }
}
