#include <main_state.h>
#include <glad/glad.h>
#include <math.h>
#include <stars.h>
#include <rafgl.h>
#include <planets.h>
#include <player.h>
#include <laser.h>
#include <enemies.h>
#include <collision.h>
#include <heart.h>
#include <item.h>
#include <particles.h>

#define ITEM_SPAWN_INTERVAL_INCREMENT 1

static int item_spawn_interval = 1;
static int w, h;
static rafgl_raster_t raster;
static rafgl_texture_t tex;
static player_t player;
static int game_over = 0;

void main_state_init(GLFWwindow *window, void *args, int width, int height)
{
    w = width;
    h = height;

    // inicijalizacija rastera na w*h piksela (u ovom primeru na 256*256)
    rafgl_raster_init(&raster, w, h);
    // inicijalizujemo slot za teksturu koji kasnije koristimo za prikazivanje rastera
    rafgl_texture_init(&tex);

    // inicijalizacija star sistema
    stars_init(w,h);

    // inicijalizacija planeta
    planets_init(w,h);

    // inicijalizacija player-a
    player_init(&player, w, h);

    // inicijalizacija lasera
    lasers_init();

    enemies_init(w,h);

    heart_init(&player);

    items_init();

    particles_init(w,h);

}

void main_state_update(GLFWwindow *window, float delta_time, rafgl_game_data_t *game_data, void *args)
{
    if(game_over)
    {
        if (game_data->keys_pressed[RAFGL_KEY_R])
        {
            game_over = 0;
        }
        return;
    }


    // updatujemo stvari ovde
    stars_update(delta_time);
    planets_update(delta_time);
    enemies_update(delta_time, &player);
    player_update(&player, delta_time, game_data);

    // handlujemo laser
    if(game_data->keys_pressed[RAFGL_KEY_SPACE])
    {
        lasers_spawn(&player);
    }
    lasers_update(delta_time, w, h);

    particles_update(delta_time);

    // kolizije
    check_laser_enemy_collisions(&player);
    check_player_enemy_collisions(&player);
    check_player_item_collisions(&player);
    heart_update(&player);
    // spawnujemo iteme
    if(player.score >= item_spawn_interval){
        items_spawn(&raster);
        item_spawn_interval += ITEM_SPAWN_INTERVAL_INCREMENT;
    }
    items_update(delta_time);
    int x,y;
    //    dvostrukom for petljom prolazimo kroz svaki piksel rastera
    //    tacka (0, 0) je gornji levi ugao slike a tacka (w-1, h-1) je donji desni ugao
    for(y=0; y<h; y++){
        float normalized_y = (float)y / h;
        for(x = 0; x < w; x++)
        {
            pixel_at_m(raster, x, y).rgba = rafgl_RGB(
                0, 
                (int)(20 * normalized_y), 
                (int)(40 * normalized_y)
            );
        }
    }

    // crtamo ovde stvari
    stars_draw(&raster);
    planets_draw(&raster);
    lasers_draw(&raster);
    particles_draw(&raster);
    player_draw(&player, &raster);
    enemies_draw(&raster);
    items_draw(&raster);
    if(player.health <= 0)
    {
        main_state_reset_run();
        return;
    }
    // beli flashbeng efekat
    if(player.hit_timer > 0)
    {
        float intensity = (float)player.hit_timer / 20.0f;
        float flash_scale = intensity * 180.0f / 255.0f;
        rafgl_pixel_rgb_t white = {.rgba = rafgl_RGB(255, 255, 255)};
        
        for(y = 0; y < h; y++)
        {
            for(x = 0; x < w; x++)
            {
                pixel_at_m(raster, x, y) = rafgl_lerppix(pixel_at_m(raster, x, y), white, flash_scale);
            }
        }
    }
    
    // crveni efekat kada je health manji od 2
    if(player.health <= 2 && player.hit_timer == 0)
    {
        float health_ratio = (float)player.health / 4.0f;
        float red_intensity = (1.0f - health_ratio) * 0.3f;
        rafgl_pixel_rgb_t red = {.rgba = rafgl_RGB(255, 0, 0)};
        
        for(y = 0; y < h; y++)
        {
            for(x = 0; x < w; x++)
            {
                pixel_at_m(raster, x, y) = rafgl_lerppix(pixel_at_m(raster, x, y), red, red_intensity);
            }
        }
    }
    
    // vignette efekat - tamnjenje ivica ekrana
    float center_x = w / 2.0f;
    float center_y = h / 2.0f;
    float max_dist = sqrtf(center_x * center_x + center_y * center_y);
    
    for(y = 0; y < h; y++)
    {
        for(x = 0; x < w; x++)
        {
            float dx = x - center_x;
            float dy = y - center_y;
            float dist = sqrtf(dx * dx + dy * dy);
            float vignette = 1.0f - (dist / max_dist) * 0.6f;
            
            if(vignette < 0.5f) vignette = 0.5f;
            
            rafgl_pixel_rgb_t p = pixel_at_m(raster, x, y);
            p.r = (int)(p.r * vignette);
            p.g = (int)(p.g * vignette);
            p.b = (int)(p.b * vignette);
            pixel_at_m(raster, x, y) = p;
        }
    }
    // ovo na kraju da ne bi imao vignette efek
    heart_draw(&player, &raster);


}

void main_state_render(GLFWwindow *window, void *args)
{
    /* update-uj teksturu */
    /* ova funkcija sluzi da primeni promene rastera na samu teksturu (spusta sliku iz rama u vram graficke kartice) */
    rafgl_texture_load_from_raster(&tex, &raster);
    /* prikazi teksturu */

    /* prikazujemo teksturu razvucenu preko celog prozora. Prozor i raster ne moraju biti istih dimenzija,
     ali ako nisu, moze doci do razvlacenja ili skupljanja rastera. */
    rafgl_texture_show(&tex, 0);


}

void main_state_cleanup(GLFWwindow *window, void *args)
{
    // stars_cleanup();
    planets_cleanup();
    heart_cleanup();
    items_cleanup();
    particles_cleanup();
}

void main_state_reset_run(void)
{
    // cleanup
    heart_cleanup();
    planets_cleanup();
    
    // init
    player_init(&player, w, h);
    lasers_init();
    enemies_init(w, h);
    stars_init(w, h);
    planets_init(w, h);
    heart_init(&player);
    items_init();
    particles_init(w,h);
    // game over logika
    game_over = 1;
    printf("Game over!\n");
}