#include <planets.h>
#include <math.h>
#include <stdlib.h>
#include <screen_shake.h>

static int width;
static int height;
static planet_t planets[MAX_PLANETS];
static rafgl_spritesheet_t planet_spritesheets[2];  // 2 razlicita sprite-a
float animation_speed = 5.0f;

void planets_init(int screen_width, int screen_height)
{
    width = screen_width;
    height = screen_height;

    float center_x = width * 0.5f;
    float center_y = height * 0.5f;

    // oba spritea
    rafgl_spritesheet_init(&planet_spritesheets[0], "res/planet_sprite_1.png", 50, 1);
    rafgl_spritesheet_init(&planet_spritesheets[1], "res/planet_sprite_2.png", 50, 1);

    // planeta 1
    planets[0].radius = 60.0f;
    planets[0].orbit_center_x = center_x / 2;
    planets[0].orbit_center_y = center_y / 2;
    planets[0].orbit_radius = 120.0f;
    planets[0].orbit_speed = 0.1f;
    planets[0].orbit_angle = 0.0f;
    planets[0].pos_x = planets[0].orbit_center_x + cosf(planets[0].orbit_angle) * planets[0].orbit_radius;
    planets[0].pos_y = planets[0].orbit_center_y + sinf(planets[0].orbit_angle) * planets[0].orbit_radius;
    planets[0].current_frame = 0.0f;
    planets[0].sprite_index = 0;  // koristi prvi sprite

    // planeta 2
    planets[1].radius = 60.0f;
    planets[1].orbit_center_x = center_x + (center_x / 2);
    planets[1].orbit_center_y = center_y + (center_y / 2);
    planets[1].orbit_radius = 120.0f;
    planets[1].orbit_speed = 0.15f;
    planets[1].orbit_angle = 1.0f;  
    planets[1].pos_x = planets[1].orbit_center_x + cosf(planets[1].orbit_angle) * planets[1].orbit_radius;
    planets[1].pos_y = planets[1].orbit_center_y + sinf(planets[1].orbit_angle) * planets[1].orbit_radius;
    planets[1].current_frame = 0.0f;
    planets[1].sprite_index = 1;  // koristi drugi sprite

}

void planets_update(float delta_time)
{
    for(int i = 0; i < MAX_PLANETS; i++)
    {
        planet_t *p = &planets[i];
        
        // orbita
        p->orbit_angle += p->orbit_speed * delta_time;
        p->pos_x = p->orbit_center_x + cosf(p->orbit_angle) * p->orbit_radius;
        p->pos_y = p->orbit_center_y + sinf(p->orbit_angle) * p->orbit_radius;
        
        // novi frame
        p->current_frame += animation_speed * delta_time;
        
        // loop
        if(p->current_frame >= 50.0f)
        {
            p->current_frame -= 50.0f;
        }
    }
}

void planets_draw(rafgl_raster_t *raster)
{
    float shake_x, shake_y;
    screen_shake_get_offset(&shake_x, &shake_y);
    for(int i = 0; i < MAX_PLANETS; i++)
    {
        planet_t *p = &planets[i];
        
        // nadji sprite
        rafgl_spritesheet_t *sprite = &planet_spritesheets[p->sprite_index];
        
        // trenutni frame
        int frame = (int)p->current_frame;
        
        // crtaj sprite centriran na poziciji planete
        int draw_x = (int)(p->pos_x + shake_x) - sprite->frame_width / 2;
        int draw_y = (int)(p->pos_y + shake_y) - sprite->frame_height / 2;
        
        rafgl_raster_draw_spritesheet(raster, sprite, frame, 0, draw_x, draw_y);
    }
}

void planets_cleanup(void)
{
    // Ocisti oba sprite-a
    for(int i = 0; i < 2; i++)
    {
        if(planet_spritesheets[i].sheet.data != NULL)
        {
            rafgl_raster_cleanup(&planet_spritesheets[i].sheet);
        }
    }
}
