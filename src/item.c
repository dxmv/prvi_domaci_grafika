#include <item.h>

static item_t items[MAX_ITEMS];
static int base_width = 32;
static int base_height = 32;
static float max_scale = 1.2f;
static float min_scale = 0.8f;
static float scale_increment = 0.3f;

void items_init(void)
{
    for(int i = 0; i < MAX_ITEMS; i++)
    {
        items[i].active = 0;
        items[i].width = base_width;
        items[i].height = base_height;

    }
}

void items_spawn(rafgl_raster_t *raster)
{
    for(int i = 0; i < MAX_ITEMS; i++)
    {
        if(!items[i].active)
        {
            items[i].active = 1;
            items[i].pos_x = rand() % raster->width;
            items[i].pos_y = rand() % raster->height;
            items[i].width = base_width;
            items[i].height = base_height;
            items[i].scale = 1.0f;
            items[i].is_growing = 1;
            return;
        }
    }
}

void items_update(float delta_time)
{
    for(int i = 0; i < MAX_ITEMS; i++)
    {
        if(!items[i].active)
        {
            continue;
        }
        
        // Simple pulse animation
        if(items[i].is_growing)
        {
            items[i].scale += scale_increment * delta_time;
            if(items[i].scale >= max_scale)
            {
                items[i].scale = max_scale;
                items[i].is_growing = 0;
            }
        }
        else
        {
            items[i].scale -= scale_increment * delta_time;
            if(items[i].scale <= min_scale)
            {
                items[i].scale = min_scale;
                items[i].is_growing = 1;
            }
        }
    }
}

void items_draw(rafgl_raster_t *raster)
{
    for(int i = 0; i < MAX_ITEMS; i++)
    {
        if(!items[i].active)
        {
            continue;
        }
        int width = (int)(items[i].width * items[i].scale);
        int height = (int)(items[i].height * items[i].scale);
        int x = items[i].pos_x - width / 2;
        int y = items[i].pos_y - height / 2;
        rafgl_raster_draw_rectangle(raster, x, y, width, height, rafgl_RGB(0, 255, 0));
    }
}

void items_cleanup(void)
{
    for(int i = 0; i < MAX_ITEMS; i++)
    {
        items[i].active = 0;
    }
}

item_t* items_get_all(void){
    return items;
}