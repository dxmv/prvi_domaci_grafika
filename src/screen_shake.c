#include <screen_shake.h>
#include <math.h>
#include <stdlib.h>

static float shake_timer = 0.0f;
static float shake_duration = 0.0f;
static float shake_magnitude = 0.0f;
static float offset_x = 0.0f;
static float offset_y = 0.0f;

void screen_shake_trigger(float duration, float magnitude)
{
    if(duration <= 0.0f || magnitude <= 0.0f)
    {
        return;
    }

    shake_timer = duration;
    shake_duration = duration;
    shake_magnitude = magnitude;
}

void screen_shake_update(float delta_time)
{
    if(shake_timer > 0.0f)
    {
        shake_timer -= delta_time;
        if(shake_timer < 0.0f)
        {
            shake_timer = 0.0f;
        }

        float progress = (shake_duration > 0.0f) ? (shake_timer / shake_duration) : 0.0f;
        float current_magnitude = shake_magnitude * progress;

        float angle = ((float)rand() / RAND_MAX) * 2.0f * (float)M_PI;
        offset_x = cosf(angle) * current_magnitude;
        offset_y = sinf(angle) * current_magnitude;
    }
    else
    {
        offset_x = 0.0f;
        offset_y = 0.0f;
    }
}

void screen_shake_get_offset(float *out_x, float *out_y)
{
    if(out_x)
    {
        *out_x = offset_x;
    }
    if(out_y)
    {
        *out_y = offset_y;
    }
}

void screen_shake_reset(void)
{
    shake_timer = 0.0f;
    shake_duration = 0.0f;
    shake_magnitude = 0.0f;
    offset_x = 0.0f;
    offset_y = 0.0f;
}
