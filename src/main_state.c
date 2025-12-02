#include <main_state.h>
#include <glad/glad.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#include <rafgl.h>

typedef struct
{
    float pos_x;
    float pos_y;
    float angle;
    float speed;
    float rotation_speed;
    float radius;
} player_t;

typedef struct
{
    float pos_x;
    float pos_y;
    float vel_x;
    float vel_y;
    float rotation;
    float rotation_speed;
    float radius;
    int active;
} asteroid_t;

#define MAX_ASTEROIDS 10

static int w, h;
static rafgl_raster_t raster;
static rafgl_texture_t tex;
static player_t player;
static asteroid_t asteroids[MAX_ASTEROIDS];

// forward declarations
void draw_player(player_t *p, rafgl_raster_t *raster);
void draw_asteroids(rafgl_raster_t *raster);
void init_asteroids();
void update_asteroids(float delta_time);

void init_asteroids()
{
    int i;
    for(i = 0; i < MAX_ASTEROIDS; i++)
    {
        asteroids[i].active = 1;
        
        // random pozicija
        asteroids[i].pos_x = (rand() % w);
        asteroids[i].pos_y = (rand() % h);
        
        // random brzina
        asteroids[i].vel_x = ((float)rand() / RAND_MAX - 0.5f) * 100.0f;
        asteroids[i].vel_y = ((float)rand() / RAND_MAX - 0.5f) * 100.0f;
        
        // random rotacija
        asteroids[i].rotation = ((float)rand() / RAND_MAX) * 3.14159f * 2.0f;
        asteroids[i].rotation_speed = ((float)rand() / RAND_MAX - 0.5f) * 2.0f;
        
        // random velicina
        asteroids[i].radius = 15.0f + ((float)rand() / RAND_MAX) * 20.0f;
    }
}

void update_asteroids(float delta_time)
{
    int i;
    for(i = 0; i < MAX_ASTEROIDS; i++)
    {
        if(!asteroids[i].active) continue;
        
        // pomeramo asteroid
        asteroids[i].pos_x += asteroids[i].vel_x * delta_time;
        asteroids[i].pos_y += asteroids[i].vel_y * delta_time;
        
        // rotiramo
        asteroids[i].rotation += asteroids[i].rotation_speed * delta_time;
        
        // wrap around screen
        if(asteroids[i].pos_x < -asteroids[i].radius)
            asteroids[i].pos_x = w + asteroids[i].radius;
        if(asteroids[i].pos_x > w + asteroids[i].radius)
            asteroids[i].pos_x = -asteroids[i].radius;
        if(asteroids[i].pos_y < -asteroids[i].radius)
            asteroids[i].pos_y = h + asteroids[i].radius;
        if(asteroids[i].pos_y > h + asteroids[i].radius)
            asteroids[i].pos_y = -asteroids[i].radius;
    }
}

void draw_asteroids(rafgl_raster_t *raster)
{
    int i;
    for(i = 0; i < MAX_ASTEROIDS; i++)
    {
        if(!asteroids[i].active) continue;
        
        asteroid_t *a = &asteroids[i];
        uint32_t asteroid_color = rafgl_RGB(150, 150, 150);
        
        // provera da li je asteroid u granicama
        if(a->pos_x < 0 || a->pos_x >= w || a->pos_y < 0 || a->pos_y >= h)
            continue;
        
        // crtamo asteroid kao krug
        if(a->pos_x - a->radius >= 0 && a->pos_x + a->radius < w &&
           a->pos_y - a->radius >= 0 && a->pos_y + a->radius < h)
        {
            rafgl_raster_draw_circle(raster, a->pos_x, a->pos_y, a->radius, asteroid_color);
        }
        
        // dodajemo liniju da vidimo rotaciju
        float line_x = a->pos_x + cosf(a->rotation) * a->radius;
        float line_y = a->pos_y + sinf(a->rotation) * a->radius;
        rafgl_raster_draw_line(raster, a->pos_x, a->pos_y, line_x, line_y, asteroid_color);
    }
}

void main_state_init(GLFWwindow *window, void *args, int width, int height)
{
    w = width;
    h = height;
    rafgl_raster_init(&raster, w, h);
    rafgl_texture_init(&tex);


    // inicijalizujemo player-a
    player.pos_x = w / 2.0f;
    player.pos_y = h / 2.0f;
    player.angle = 0.0f;
    player.speed = 250.0f;
    player.rotation_speed = 5.0f;
    player.radius = 20.0f;
    
    // inicijalizujemo asteroide
    init_asteroids();
}

void main_state_update(GLFWwindow *window, float delta_time, rafgl_game_data_t *game_data, void *args)
{
    // rotacija sa a i d
    if(game_data->keys_down[RAFGL_KEY_A])
    {
        player.angle -= player.rotation_speed * delta_time;
    }
    else if(game_data->keys_down[RAFGL_KEY_D])
    {
        player.angle += player.rotation_speed * delta_time;
    }

    // w/s za kretanje
    if(game_data->keys_down[RAFGL_KEY_W])
    {
        player.pos_x += cosf(player.angle) * player.speed * delta_time;
        player.pos_y += sinf(player.angle) * player.speed * delta_time;
    }
    else if(game_data->keys_down[RAFGL_KEY_S])
    {
        player.pos_x -= cosf(player.angle) * player.speed * delta_time;
        player.pos_y -= sinf(player.angle) * player.speed * delta_time;
    }
    
    // update-ujemo asteroide
    update_asteroids(delta_time);

    // klirujemo background
    int x, y;
    for(y = 0; y < h; y++){
        for(x = 0; x < w; x++){
            pixel_at_m(raster, x, y).rgba = rafgl_RGB(0, 0, 0);
        }
    }
    
    // crtamo asteroide
    draw_asteroids(&raster);

    // crtamo player-a
    draw_player(&player, &raster);
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

// drawing funkcije
void draw_player(player_t *p, rafgl_raster_t *raster)
{
    // izracunavamo rotated vertices
    float cos_a = cosf(p->angle);
    float sin_a = sinf(p->angle);
    
    // glava (front point)
    int x0 = p->pos_x + p->radius * cos_a;
    int y0 = p->pos_y + p->radius * sin_a;
    
    // gornji desni ugao
    int x1 = p->pos_x + (-p->radius * cos_a - p->radius * 0.5f * sin_a);
    int y1 = p->pos_y + (-p->radius * sin_a + p->radius * 0.5f * cos_a);
    
    // donji desni ugao
    int x2 = p->pos_x + (-p->radius * cos_a + p->radius * 0.5f * sin_a);
    int y2 = p->pos_y + (-p->radius * sin_a - p->radius * 0.5f * cos_a);

    // crtamo trougao
    uint32_t ship_color = rafgl_RGB(255, 255, 255);
    rafgl_raster_draw_line(raster, x0, y0, x1, y1, ship_color);
    rafgl_raster_draw_line(raster, x1, y1, x2, y2, ship_color);
    rafgl_raster_draw_line(raster, x2, y2, x0, y0, ship_color);
}
