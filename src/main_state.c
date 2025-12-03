#include <main_state.h>
#include <glad/glad.h>
#include <rafgl.h>
#include <player.h>
#include <stars.h>
#include <laser.h>

static int w, h;
static rafgl_raster_t raster;
static rafgl_texture_t tex;
static player_t player;

void main_state_init(GLFWwindow *window, void *args, int width, int height)
{
    w = width;
    h = height;
    rafgl_raster_init(&raster, w, h);
    rafgl_texture_init(&tex);

    srand(42);
    // inicijalizujemo player-a na sredini ekrana
    player_init(&player, w / 2.0f, h / 2.0f);

    // inicijalizujemo zvezde
    stars_init(w, h);
    
    // inicijalizujemo lasere
    lasers_init();
}

void main_state_update(GLFWwindow *window, float delta_time, rafgl_game_data_t *game_data, void *args)
{
    player_update(&player, delta_time, game_data);
    stars_update(delta_time);
    
    // Pucanje - proveri da li je pritisnut Space
    if(game_data->keys_pressed[RAFGL_KEY_SPACE])
    {
        lasers_spawn(&player);
    }
    
    // Azuriraj lasere
    lasers_update(delta_time, w, h);

    // klirujemo background
    int x, y;
    for(y = 0; y < h; y++){
        for(x = 0; x < w; x++){
            pixel_at_m(raster, x, y).rgba = rafgl_RGB(0, 0, 0);
        }
    }
    
    // crtamo zvezde
    stars_draw(&raster);
    
    // crtamo lasere
    lasers_draw(&raster);

    // crtamo player-a
    player_draw(&player, &raster);
}

void main_state_render(GLFWwindow *window, void *args)
{
    rafgl_texture_load_from_raster(&tex, &raster);
    rafgl_texture_show(&tex, 0);
}

void main_state_cleanup(GLFWwindow *window, void *args)
{
    rafgl_raster_cleanup(&raster);
    rafgl_texture_cleanup(&tex);
}
