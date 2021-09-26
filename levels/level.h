#ifndef _LEVELS_H
#define _LEVELS_H

#include "pico/types.h"

typedef struct box {
    int16_t x;
    int16_t y;
    int16_t w;
    int16_t h;
    bool exit_zone;
    bool has_enemy;
} box_t;

typedef struct melon {
    int16_t x;
    int16_t y;
} melon_t;

typedef struct level {
    int16_t spawnX;
    int16_t spawnY;

    size_t numboxes;
    box_t *boxes;

    size_t nummelons;
    melon_t *melons;
} level_t;

#endif