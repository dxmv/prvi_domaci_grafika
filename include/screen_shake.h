#ifndef SCREEN_SHAKE_H
#define SCREEN_SHAKE_H

void screen_shake_trigger(float duration, float magnitude);
void screen_shake_update(float delta_time);
void screen_shake_get_offset(float *offset_x, float *offset_y);
void screen_shake_reset(void);

#endif
