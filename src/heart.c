#include <heart.h>

static heart_t* hearts;
static rafgl_raster_t heart_raster;
static int active_hearts;
static int previous_hearts;
static int heart_width = 32;

void heart_init(player_t *player)
{
    rafgl_raster_load_from_image(&heart_raster, "res/heart.png");
    // niz od player health elemenata
    hearts = (heart_t*)malloc(player->health * sizeof(heart_t));
    active_hearts = player->health;
    int padding = 10;
    for(int i = 0; i < active_hearts; i++)
    {
        hearts[i].pos_x = padding + i * (heart_raster.width + padding);
        hearts[i].pos_y = padding;
    }
}

void heart_update(player_t *player)
{
    if(player->health < 0){
        player->health = 0;
    }
    previous_hearts = active_hearts;
    active_hearts = player->health;
}

void heart_draw(player_t *player, rafgl_raster_t *raster){
    for(int i = 0; i < active_hearts; i++){
        int src_x, src_y, dst_x, dst_y;
        for(src_y = 0; src_y < heart_raster.height; src_y++){
            for(src_x = 0; src_x < heart_raster.width; src_x++){
                dst_x = hearts[i].pos_x + src_x;
                dst_y = hearts[i].pos_y + src_y;
                
                if(dst_x >= 0 && dst_x < raster->width && dst_y >= 0 && dst_y < raster->height){
                    rafgl_pixel_rgb_t src_pixel = pixel_at_m(heart_raster, src_x, src_y);
                    if(src_pixel.a > 0){
                        pixel_at_pm(raster, dst_x, dst_y) = src_pixel;
                    }
                }
            }
        }
    }
}

void heart_cleanup(void){
    rafgl_raster_cleanup(&heart_raster);
    free(hearts);
}