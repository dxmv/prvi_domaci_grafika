#include <item.h>

static item_t items[MAX_ITEMS];

void items_init(void)
{
    for(int i = 0; i < MAX_ITEMS; i++)
    {
        items[i].active = 0;
        items[i].width = 32;
        items[i].height = 32;
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
        rafgl_raster_draw_rectangle(raster, items[i].pos_x, items[i].pos_y, items[i].width, items[i].height, rafgl_RGB(0, 255, 0));
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