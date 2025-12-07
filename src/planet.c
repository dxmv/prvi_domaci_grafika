#include <planets.h>
#include <math.h>
#include <stdlib.h>

static int width;
static int height;
static planet_t planets[MAX_PLANETS];
static rafgl_spritesheet_t planet_spritesheets[2];  // 2 razlicita sprite-a

void planets_init(int screen_width, int screen_height)
{
    width = screen_width;
    height = screen_height;

    float center_x = width * 0.5f;
    float center_y = height * 0.5f;

    // Ucitaj oba sprite-a: 50 frame-ova svaki (50 kolona x 1 red)
    rafgl_spritesheet_init(&planet_spritesheets[0], "res/planet_sprite_1.png", 50, 1);
    rafgl_spritesheet_init(&planet_spritesheets[1], "res/planet_sprite_2.png", 50, 1);

    // Planeta 1 - Plava ledena planeta
    planets[0].radius = 60.0f;
    planets[0].color = rafgl_RGB(100, 150, 255);  // fallback boja
    planets[0].orbit_center_x = center_x - 200.0f;
    planets[0].orbit_center_y = center_y - 100.0f;
    planets[0].orbit_radius = 120.0f;
    planets[0].orbit_speed = 0.1f;
    planets[0].orbit_angle = 0.0f;
    planets[0].pos_x = planets[0].orbit_center_x + cosf(planets[0].orbit_angle) * planets[0].orbit_radius;
    planets[0].pos_y = planets[0].orbit_center_y + sinf(planets[0].orbit_angle) * planets[0].orbit_radius;
    planets[0].current_frame = 0.0f;
    planets[0].animation_speed = 10.0f;
    planets[0].sprite_index = 0;  // koristi prvi sprite

    // Planeta 2 - Narandzasta vatrena planeta
    planets[1].radius = 50.0f;
    planets[1].color = rafgl_RGB(255, 120, 60);  // fallback boja
    planets[1].orbit_center_x = center_x + 200.0f;
    planets[1].orbit_center_y = center_y + 100.0f;
    planets[1].orbit_radius = 150.0f;
    planets[1].orbit_speed = 0.15f;
    planets[1].orbit_angle = 0.0f;  // pocinje sa druge strane
    planets[1].pos_x = planets[1].orbit_center_x + cosf(planets[1].orbit_angle) * planets[1].orbit_radius;
    planets[1].pos_y = planets[1].orbit_center_y + sinf(planets[1].orbit_angle) * planets[1].orbit_radius;
    planets[1].current_frame = 0.0f;
    planets[1].animation_speed = 10.0f;
    planets[1].sprite_index = 1;  // koristi drugi sprite

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
        
        // Uzmi odgovarajuci spritesheet za ovu planetu
        rafgl_spritesheet_t *sprite = &planet_spritesheets[p->sprite_index];
        
        // Crtaj planetu iz sprite-a ako je ucitan
        if(sprite->sheet.data != NULL)
        {
            // Trenutni frame animacije (integer)
            int frame = (int)p->current_frame;
            
            // Crtaj sprite centriran na poziciji planete
            int draw_x = (int)p->pos_x - sprite->frame_width / 2;
            int draw_y = (int)p->pos_y - sprite->frame_height / 2;
            
            // frame je kolona (0-49), row je uvek 0 (jedan red)
            rafgl_raster_draw_spritesheet(raster, sprite, frame, 0, draw_x, draw_y);
        }
        else
        {
            // Fallback na krug ako sprite nije ucitan
            rafgl_raster_draw_circle(raster, (int)p->pos_x, (int)p->pos_y, (int)p->radius, p->color);
        }
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
