#include <particles.h>
#include <math.h>
#include <screen_shake.h>

static particle_t particles[MAX_PARTICLES];
static float elasticity = 0.6f;
static int raster_width = 1000;
static int raster_height = 1000;

void particles_init(int screen_width, int screen_height)
{
    raster_width = screen_width;
    raster_height = screen_height;
    
    for(int i = 0; i < MAX_PARTICLES; i++)
    {
        particles[i].life = 0;
    }
}

void particles_spawn(float x, float y, int count, float radius)
{
    int spawned = 0;
    float angle, speed;
    
    for(int i = 0; (i < MAX_PARTICLES) && (spawned < count); i++)
    {
        if(particles[i].life <= 0)
        {
            particles[i].life = 100 * randf() + 100;
            particles[i].x = x;
            particles[i].y = y;
            
            angle = randf() * M_PI * 2.0f;
            speed = (0.3f + 0.7f * randf()) * radius;
            particles[i].dx = cosf(angle) * speed;
            particles[i].dy = sinf(angle) * speed;
            
            spawned++;
        }
    }
}

void particles_update(float delta_time)
{
    for(int i = 0; i < MAX_PARTICLES; i++)
    {
        if(particles[i].life <= 0) continue;

        particles[i].life--;

        particles[i].x += particles[i].dx;
        particles[i].y += particles[i].dy;
        particles[i].dx *= 0.995f;
        particles[i].dy *= 0.995f;
        particles[i].dy += 0.05f;

        if(particles[i].x < 0)
        {
            particles[i].x = 0;
            particles[i].dx = (rafgl_abs_m(particles[i].dx)) * randf() * elasticity;
        }

        if(particles[i].y < 0)
        {
            particles[i].y = 0;
            particles[i].dy = (rafgl_abs_m(particles[i].dy)) * randf() * elasticity;
        }

        if(particles[i].x >= raster_width)
        {
            particles[i].x = raster_width - 1;
            particles[i].dx = (rafgl_abs_m(particles[i].dx)) * randf() * (-elasticity);
        }

        if(particles[i].y >= raster_height)
        {
            particles[i].y = raster_height - 1;
            particles[i].dy = (rafgl_abs_m(particles[i].dy)) * randf() * (-elasticity);
        }
    }
}

void particles_draw(rafgl_raster_t *raster)
{
    float shake_x, shake_y;
    screen_shake_get_offset(&shake_x, &shake_y);
    particle_t p;
    for(int i = 0; i < MAX_PARTICLES; i++)
    {
        p = particles[i];
        if(p.life <= 0) continue;
        rafgl_raster_draw_line(raster, 
            (int)(p.x + shake_x - p.dx), 
            (int)(p.y + shake_y - p.dy), 
            (int)(p.x + shake_x), 
            (int)(p.y + shake_y), 
            rafgl_RGB(255, 255, 255));
    }
}

void particles_cleanup(void)
{
    for(int i = 0; i < MAX_PARTICLES; i++)
    {
        particles[i].life = 0;
    }
}
