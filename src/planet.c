#include <planets.h>
#include <math.h>
#include <stdlib.h>

static int width;
static int height;
static planet_t planets[MAX_PLANETS];

void planets_init(int screen_width, int screen_height)
{
    width = screen_width;
    height = screen_height;

    float center_x = width * 0.5f;
    float center_y = height * 0.5f;

    // Planet 1 - Large orange planet with slow orbit
    planets[0].radius = 60.0f;
    planets[0].color = rafgl_RGB(255, 140, 50);  // Orange
    planets[0].orbit_center_x = center_x - 150.0f;
    planets[0].orbit_center_y = center_y - 100.0f;
    planets[0].orbit_radius = 120.0f;
    planets[0].orbit_speed = 0.1f;  // Slowest
    planets[0].orbit_angle = 0.0f;
    planets[0].pos_x = planets[0].orbit_center_x + cosf(planets[0].orbit_angle) * planets[0].orbit_radius;
    planets[0].pos_y = planets[0].orbit_center_y + sinf(planets[0].orbit_angle) * planets[0].orbit_radius;

    // Planet 2 - Medium blue planet with medium orbit
    planets[1].radius = 45.0f;
    planets[1].color = rafgl_RGB(80, 120, 255);  // Blue
    planets[1].orbit_center_x = center_x;
    planets[1].orbit_center_y = center_y + 50.0f;
    planets[1].orbit_radius = 150.0f;
    planets[1].orbit_speed = 0.15f;  // Medium speed
    planets[1].orbit_angle = 2.0f;
    planets[1].pos_x = planets[1].orbit_center_x + cosf(planets[1].orbit_angle) * planets[1].orbit_radius;
    planets[1].pos_y = planets[1].orbit_center_y + sinf(planets[1].orbit_angle) * planets[1].orbit_radius;

    // Planet 3 - Small red planet with faster orbit
    planets[2].radius = 35.0f;
    planets[2].color = rafgl_RGB(255, 80, 80);   // Red
    planets[2].orbit_center_x = center_x + 150.0f;
    planets[2].orbit_center_y = center_y - 80.0f;
    planets[2].orbit_radius = 100.0f;
    planets[2].orbit_speed = 0.2f;  // Fastest (but still slow)
    planets[2].orbit_angle = 4.0f;
    planets[2].pos_x = planets[2].orbit_center_x + cosf(planets[2].orbit_angle) * planets[2].orbit_radius;
    planets[2].pos_y = planets[2].orbit_center_y + sinf(planets[2].orbit_angle) * planets[2].orbit_radius;
}

void planets_update(float delta_time)
{
    for(int i = 0; i < MAX_PLANETS; i++)
    {
        planet_t *p = &planets[i];
        
        // Update orbital angle
        p->orbit_angle += p->orbit_speed * delta_time;
        
        // Keep angle in 0-2PI range to prevent overflow
        if(p->orbit_angle > 6.28318530718f)  // 2 * PI
        {
            p->orbit_angle -= 6.28318530718f;
        }
        
        // Calculate new position based on orbit
        p->pos_x = p->orbit_center_x + cosf(p->orbit_angle) * p->orbit_radius;
        p->pos_y = p->orbit_center_y + sinf(p->orbit_angle) * p->orbit_radius;
    }
}

void planets_draw(rafgl_raster_t *raster)
{
    for(int i = 0; i < MAX_PLANETS; i++)
    {
        planet_t *p = &planets[i];
        
        // Draw planet as a filled circle
        rafgl_raster_draw_circle(raster, (int)p->pos_x, (int)p->pos_y, (int)p->radius, p->color);
    }
}
