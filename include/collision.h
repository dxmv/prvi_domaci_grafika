#ifndef COLLISION_H_INCLUDED
#define COLLISION_H_INCLUDED

#include <player.h>
#include <laser.h>
#include <enemies.h>

// Generic circle collision check
int check_collision(float x1, float y1, float r1, float x2, float y2, float r2);

// Specific collision checks
void check_laser_enemy_collisions(void);
void check_player_enemy_collisions(const player_t *player);

#endif
