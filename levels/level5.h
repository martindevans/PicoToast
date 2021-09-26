#ifndef _level5_H
#define _level5_H

#include "level.h"

box_t level5_boxes[] = {
    {
        .x = 1440,
        .y = 5820,
        .w = 120,
        .h = 75,
        .exit_zone = true,
        .has_enemy = false
    },
    {
        .x = 3600,
        .y = 930,
        .w = 285,
        .h = 1395,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 6165,
        .y = 5145,
        .w = 180,
        .h = 1110,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 8715,
        .y = 1020,
        .w = 180,
        .h = 1320,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 11685,
        .y = 3045,
        .w = 135,
        .h = 2190,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 1830,
        .y = 6990,
        .w = 315,
        .h = 1245,
        .exit_zone = false,
        .has_enemy = false
    },
};

melon_t level5_melons[] = {
    {
        .x = 12015.0,
        .y = 4185.0
    },
};

level_t level5 = {
    .spawnX = 1200.0,
    .spawnY = 4755.0,
    .numboxes = 6,
    .boxes = &level5_boxes[0],
    .nummelons = 1,
    .melons = &level5_melons[0],
};

#endif
