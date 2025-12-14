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

#define ITEM_SPAWN_INTERVAL_INCREMENT 2

static int item_spawn_interval = 2;
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

    // handle laser input
    if(game_data->keys_pressed[RAFGL_KEY_SPACE])
    {
        lasers_spawn(&player);
    }
    lasers_update(delta_time, w, h);

    // kolizije
    check_laser_enemy_collisions(&player);
    check_player_enemy_collisions(&player);
    check_player_item_collisions(&player);
    heart_update(&player);
    if(player.score >= item_spawn_interval){
        items_spawn(&raster);
        item_spawn_interval += ITEM_SPAWN_INTERVAL_INCREMENT;
    }
    items_update(delta_time);
    int x,y;
    //    dvostrukom for petljom prolazimo kroz svaki piksel rastera
    //    tacka (0, 0) je gornji levi ugao slike a tacka (w-1, h-1) je donji desni ugao
    for(y=0; y<h; y++){
        for(x=0;x<w;x++){
//            uz pomoc pixel_at_m makroa mozemo pristupiti pikselu na koordinatama x, y unutar rastera r (makro pixel_at_pm koristimo ako imamo pokazivac na raster)

                /*
                pixel je union i mozemo ga koristiti na vise nacina. ako imamo pixel pix i zelimo da mu promenimo jednu komponentu, na primer crvenu
                mozemo da uradimo pix.r = 255; isto to mozemo uraditi kao pix.components[0] = 255; ako zelimo da promenimo sve komponente istovremeno
                (zadamo neku specificnu boju, na primer zutu) mozemo da uradimo pix.rgba = rafgl_RGB(255, 255, 0);
                */

            /* u ovom primeru je zadato da se raster popuni flat bojom (ista boja na celom rasteru),
            tako da jednostavno mozemo svaki piksel postaviti da ima istu vrednost */
            pixel_at_m(raster, x, y).rgba = rafgl_RGB(0, 0, 0);
        }
    }

    // crtamo ovde stvari
    stars_draw(&raster);
    planets_draw(&raster);
    lasers_draw(&raster);
    player_draw(&player, &raster);
    enemies_draw(&raster);
    heart_draw(&player, &raster);
    items_draw(&raster);
    if(player.health <= 0)
    {
        main_state_reset_run();
        return;
    }
    // beli flashbeng efekat
    if(player.hit_timer > 0)
    {
        float intensity;
        int flash_alpha;
        rafgl_pixel_rgb_t current;
        
        intensity = (float)player.hit_timer / 20.0f;
        flash_alpha = (int)(intensity * 180.0f); 
        
        for(y = 0; y < h; y++)
        {
            for(x = 0; x < w; x++)
            {
                current = pixel_at_m(raster, x, y);
                
                // blendovanje bele svjetlosti sa trenutnim pikselom
                current.r = current.r + (255 - current.r) * flash_alpha / 255;
                current.g = current.g + (255 - current.g) * flash_alpha / 255;
                current.b = current.b + (255 - current.b) * flash_alpha / 255;
                
                pixel_at_m(raster, x, y) = current;
            }
        }
    }

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
    // game over logika
    game_over = 1;
    printf("Game over!\n");
}