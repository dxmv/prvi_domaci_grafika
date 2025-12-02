#include <asteroids.h>
#include <math.h>
#include <stdlib.h>

static int width;
static int height;
static asteroid_t asteroids[MAX_ASTEROIDS];
static const uint32_t asteroid_color = rafgl_RGB(139, 69, 19);

void asteroids_init(int screen_width, int screen_height)
{
    width = screen_width;
    height = screen_height;

    for(int i = 0; i < MAX_ASTEROIDS; i++)
    {
        asteroids[i].active = 1;
        asteroids[i].pos_x = (rand() % width);
        asteroids[i].pos_y = (rand() % height);
        asteroids[i].vel_x = ((float)rand() / RAND_MAX - 0.5f) * 50.0f;
        asteroids[i].vel_y = ((float)rand() / RAND_MAX - 0.5f) * 50.0f;
        asteroids[i].rotation = ((float)rand() / RAND_MAX) * M_PI * 2.0f;
        asteroids[i].rotation_speed = ((float)rand() / RAND_MAX - 0.5f) * 2.0f;
        asteroids[i].radius = 15.0f + ((float)rand() / RAND_MAX) * 20.0f;
    }
}

void asteroids_update(float delta_time)
{
    for(int i = 0; i < MAX_ASTEROIDS; i++)
    {
        if(!asteroids[i].active)
        {
            continue;
        }

        asteroid_t *a = &asteroids[i];
        a->pos_x += a->vel_x * delta_time;
        a->pos_y += a->vel_y * delta_time;
        a->rotation += a->rotation_speed * delta_time;

        if(a->pos_x < -a->radius)
        {
            a->pos_x = width + a->radius;
        }
        if(a->pos_x > width + a->radius)
        {
            a->pos_x = -a->radius;
        }
        if(a->pos_y < -a->radius)
        {
            a->pos_y = height + a->radius;
        }
        if(a->pos_y > height + a->radius)
        {
            a->pos_y = -a->radius;
        }
    }
}

void asteroids_draw(rafgl_raster_t *raster)
{
    for(int i = 0; i < MAX_ASTEROIDS; i++)
    {
        if(!asteroids[i].active)
        {
            continue;
        }

        asteroid_t *a = &asteroids[i];
        if(a->pos_x < 0 || a->pos_x >= width || a->pos_y < 0 || a->pos_y >= height)
        {
            continue;
        }

        if(a->pos_x - a->radius >= 0 && a->pos_x + a->radius < width &&
           a->pos_y - a->radius >= 0 && a->pos_y + a->radius < height)
        {
            rafgl_raster_draw_circle(raster, a->pos_x, a->pos_y, a->radius, asteroid_color);
        }

        float line_x = a->pos_x + cosf(a->rotation) * a->radius;
        float line_y = a->pos_y + sinf(a->rotation) * a->radius;
        rafgl_raster_draw_line(raster, a->pos_x, a->pos_y, line_x, line_y, asteroid_color);
    }
}

const asteroid_t *asteroids_get_all(void)
{
    return asteroids;
}
