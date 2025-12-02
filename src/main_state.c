#include <main_state.h>
#include <glad/glad.h>
#include <math.h>

#include <rafgl.h>

static int w, h;
static rafgl_raster_t raster;
static rafgl_texture_t tex;

static float pos_x, pos_y;
static float ship_angle = 0.0f;
static float ship_speed = 250.0f;
static float rotation_speed = 5.0f;

void main_state_init(GLFWwindow *window, void *args, int width, int height)
{
    w = width;
    h = height;
    rafgl_raster_init(&raster, w, h);
    rafgl_texture_init(&tex);

    // sredina screena
    pos_x = w / 2.0f;
    pos_y = h / 2.0f;
}

void main_state_update(GLFWwindow *window, float delta_time, rafgl_game_data_t *game_data, void *args)
{
    // rotacija sa a i d
    if(game_data->keys_down[RAFGL_KEY_A])
    {
        ship_angle -= rotation_speed * delta_time;
    }
    else if(game_data->keys_down[RAFGL_KEY_D])
    {
        ship_angle += rotation_speed * delta_time;
    }

    // w/s za kretanje
    if(game_data->keys_down[RAFGL_KEY_W])
    {
        pos_x += cosf(ship_angle) * ship_speed * delta_time;
        pos_y += sinf(ship_angle) * ship_speed * delta_time;
    }
    else if(game_data->keys_down[RAFGL_KEY_S])
    {
        pos_x -= cosf(ship_angle) * ship_speed * delta_time;
        pos_y -= sinf(ship_angle) * ship_speed * delta_time;
    }

    // klirujemo background
    int x, y;
    for(y = 0; y < h; y++){
        for(x = 0; x < w; x++){
            pixel_at_m(raster, x, y).rgba = rafgl_RGB(0, 0, 0);
        }
    }

    // definisemo trougao
    float radius = 20.0f;
    
    // izracunavamo rotated vertices
    float cos_a = cosf(ship_angle);
    float sin_a = sinf(ship_angle);
    
    // glava (front point)
    int x0 = pos_x + radius * cos_a;
    int y0 = pos_y + radius * sin_a;
    
    // gornji desni ugao
    int x1 = pos_x + (-radius * cos_a - radius * 0.5f * sin_a);
    int y1 = pos_y + (-radius * sin_a + radius * 0.5f * cos_a);
    
    // donji desni ugao
    int x2 = pos_x + (-radius * cos_a + radius * 0.5f * sin_a);
    int y2 = pos_y + (-radius * sin_a - radius * 0.5f * cos_a);

    // crtamo trougao
    uint32_t ship_color = rafgl_RGB(255, 255, 255);
    rafgl_raster_draw_line(&raster, x0, y0, x1, y1, ship_color);
    rafgl_raster_draw_line(&raster, x1, y1, x2, y2, ship_color);
    rafgl_raster_draw_line(&raster, x2, y2, x0, y0, ship_color);
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
