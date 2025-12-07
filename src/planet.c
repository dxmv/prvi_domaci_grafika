#include <planets.h>
#include <math.h>
#include <stdlib.h>

static int width;
static int height;
static planet_t planets[MAX_PLANETS];
static rafgl_spritesheet_t planet_spritesheet;

void planets_init(int screen_width, int screen_height)
{
    width = screen_width;
    height = screen_height;

    float center_x = width * 0.5f;
    float center_y = height * 0.5f;

    // Load the spritesheet: 50 frames of rotating planet animation (50 columns x 1 row)
    rafgl_spritesheet_init(&planet_spritesheet, "res/planet_sprite.png", 50, 1);

    // Planet 1 - Large planet with slow orbit
    planets[0].radius = 60.0f;
    planets[0].color = rafgl_RGB(255, 140, 50);  // Fallback color
    planets[0].orbit_center_x = center_x - 150.0f;
    planets[0].orbit_center_y = center_y - 100.0f;
    planets[0].orbit_radius = 120.0f;
    planets[0].orbit_speed = 0.1f;  // Slowest orbit
    planets[0].orbit_angle = 0.0f;
    planets[0].pos_x = planets[0].orbit_center_x + cosf(planets[0].orbit_angle) * planets[0].orbit_radius;
    planets[0].pos_y = planets[0].orbit_center_y + sinf(planets[0].orbit_angle) * planets[0].orbit_radius;
    planets[0].current_frame = 0.0f;
    planets[0].animation_speed = 10.0f;  // Frames per second

    // Planet 2 - Medium planet with medium orbit
    planets[1].radius = 45.0f;
    planets[1].color = rafgl_RGB(80, 120, 255);  // Fallback color
    planets[1].orbit_center_x = center_x;
    planets[1].orbit_center_y = center_y + 50.0f;
    planets[1].orbit_radius = 150.0f;
    planets[1].orbit_speed = 0.15f;  // Medium orbit speed
    planets[1].orbit_angle = 2.0f;
    planets[1].pos_x = planets[1].orbit_center_x + cosf(planets[1].orbit_angle) * planets[1].orbit_radius;
    planets[1].pos_y = planets[1].orbit_center_y + sinf(planets[1].orbit_angle) * planets[1].orbit_radius;
    planets[1].current_frame = 16.0f;  // Start at different frame for variety
    planets[1].animation_speed = 12.0f;  // Slightly faster rotation

    // Planet 3 - Small planet with faster orbit
    planets[2].radius = 35.0f;
    planets[2].color = rafgl_RGB(255, 80, 80);   // Fallback color
    planets[2].orbit_center_x = center_x + 150.0f;
    planets[2].orbit_center_y = center_y - 80.0f;
    planets[2].orbit_radius = 100.0f;
    planets[2].orbit_speed = 0.2f;  // Fastest orbit
    planets[2].orbit_angle = 4.0f;
    planets[2].pos_x = planets[2].orbit_center_x + cosf(planets[2].orbit_angle) * planets[2].orbit_radius;
    planets[2].pos_y = planets[2].orbit_center_y + sinf(planets[2].orbit_angle) * planets[2].orbit_radius;
    planets[2].current_frame = 33.0f;  // Start at different frame for variety
    planets[2].animation_speed = 15.0f;  // Fastest rotation
}

void planets_update(float delta_time)
{
    for(int i = 0; i < MAX_PLANETS; i++)
    {
        planet_t *p = &planets[i];
        
        // Update orbit position
        p->orbit_angle += p->orbit_speed * delta_time;
        p->pos_x = p->orbit_center_x + cosf(p->orbit_angle) * p->orbit_radius;
        p->pos_y = p->orbit_center_y + sinf(p->orbit_angle) * p->orbit_radius;
        
        // Update animation frame
        p->current_frame += p->animation_speed * delta_time;
        
        // Loop animation (50 frames total)
        if(p->current_frame >= 50.0f)
        {
            p->current_frame -= 50.0f;
        }
    }
}

void planets_draw(rafgl_raster_t *raster)
{
    for(int i = 0; i < MAX_PLANETS; i++)
    {
        planet_t *p = &planets[i];
        
        // Draw planet from spritesheet if loaded
        if(planet_spritesheet.sheet.data != NULL)
        {
            // Get the current animation frame (integer)
            int frame = (int)p->current_frame;
            
            // Draw the sprite centered on the planet position
            int draw_x = (int)p->pos_x - planet_spritesheet.frame_width / 2;
            int draw_y = (int)p->pos_y - planet_spritesheet.frame_height / 2;
            
            // frame_x is the column (0-49), frame_y is always 0 (single row)
            rafgl_raster_draw_spritesheet(raster, &planet_spritesheet, frame, 0, draw_x, draw_y);
        }
        else
        {
            // Fallback to circle if spritesheet didn't load
            rafgl_raster_draw_circle(raster, (int)p->pos_x, (int)p->pos_y, (int)p->radius, p->color);
        }
    }
}

void planets_cleanup(void)
{
    // Clean up the spritesheet
    if(planet_spritesheet.sheet.data != NULL)
    {
        rafgl_raster_cleanup(&planet_spritesheet.sheet);
    }
}
