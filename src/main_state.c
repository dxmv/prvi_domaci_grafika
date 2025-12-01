#include <main_state.h>
#include <glad/glad.h>
#include <math.h>

#include <rafgl.h>

static int w, h;
static rafgl_raster_t raster;
static rafgl_texture_t tex;

static float pos_x, pos_y;
static int square_size = 20;

void main_state_init(GLFWwindow *window, void *args, int width, int height)
{
//  upamtimo vrednosti sirine i visine prozora
//koje smo prosledili pozivom main_state_init() funkcije u okviru main.c fajla
    w = width;
    h = height;
//    inicijalizacija rastera na w*h piksela (u ovom primeru na 256*256)
    rafgl_raster_init(&raster, w, h);
//     inicijalizujemo slot za teksturu koji kasnije koristimo za prikazivanje rastera
    rafgl_texture_init(&tex);

    pos_x = w / 2.0f;
    pos_y = h / 2.0f;

}

void main_state_update(GLFWwindow *window, float delta_time, rafgl_game_data_t *game_data, void *args)
{
    float speed = 200.0f;

    if(game_data->keys_down[RAFGL_KEY_W]) pos_y -= speed * delta_time;
    if(game_data->keys_down[RAFGL_KEY_S]) pos_y += speed * delta_time;
    if(game_data->keys_down[RAFGL_KEY_A]) pos_x -= speed * delta_time;
    if(game_data->keys_down[RAFGL_KEY_D]) pos_x += speed * delta_time;

    int x,y;
//    dvostrukom for petljom prolazimo kroz svaki piksel rastera
//    tacka (0, 0) je gornji levi ugao slike a tacka (w-1, h-1) je donji desni ugao
    for(y=0; y<h; y++){
        for(x=0;x<w;x++){
            pixel_at_m(raster, x, y).rgba = rafgl_RGB(0, 0, 0);
        }
    }

    int sx = (int)pos_x;
    int sy = (int)pos_y;
    
    /* Draw filled white square */
    for(y = sy; y < sy + square_size; y++) {
        for(x = sx; x < sx + square_size; x++) {
            if(x >= 0 && x < w && y >= 0 && y < h) {
                pixel_at_m(raster, x, y).rgba = rafgl_RGB(255, 255, 255);
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
    rafgl_raster_cleanup(&raster);
    rafgl_texture_cleanup(&tex);
}
