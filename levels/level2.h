#ifndef _level2_H
#define _level2_H

#include "levels.h"

box_t level2_boxes[] = {
    {
        .x = 1905,
        .y = 3840,
        .w = 2490,
        .h = 225,
        .exit_zone = true,
        .has_enemy = false
    },
    {
        .x = 4380,
        .y = 1755,
        .w = 255,
        .h = 4695,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 4620,
        .y = 3840,
        .w = 2490,
        .h = 225,
        .exit_zone = false,
        .has_enemy = true
    },
    {
        .x = 9870,
        .y = 3765,
        .w = 495,
        .h = 495,
        .exit_zone = false,
        .has_enemy = false
    },
};

melon_t level2_melons[] = {
    {
        .x = 5835.0,
        .y = 3405.0
    },
    {
        .x = 10065.0,
        .y = 3450.0
    },
    {
        .x = 3030.0,
        .y = 3480.0
    },
};

level_t level2 = {
    .spawnX = 3007.5,
    .spawnY = 2377.5,
    .numboxes = 4,
    .boxes = &level2_boxes[0],
    .nummelons = 3,
    .melons = &level2_melons[0],
};

#endif
