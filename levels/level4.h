#ifndef _level4_H
#define _level4_H

#include "levels.h"

box_t level4_boxes[] = {
    {
        .x = 4125,
        .y = 8295,
        .w = 3435,
        .h = 435,
        .exit_zone = true,
        .has_enemy = false
    },
    {
        .x = 4125,
        .y = 4875,
        .w = 150,
        .h = 3420,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 8505,
        .y = 2775,
        .w = 300,
        .h = 2730,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 3090,
        .y = 1875,
        .w = 2670,
        .h = 465,
        .exit_zone = false,
        .has_enemy = true
    },
    {
        .x = 6090,
        .y = 4050,
        .w = 495,
        .h = 495,
        .exit_zone = false,
        .has_enemy = false
    },
};

melon_t level4_melons[] = {
    {
        .x = 4335.0,
        .y = 1500.0
    },
    {
        .x = 9120.0,
        .y = 4125.0
    },
    {
        .x = 6780.0,
        .y = 4200.0
    },
    {
        .x = 3855.0,
        .y = 6720.0
    },
};

level_t level4 = {
    .spawnX = 5625.0,
    .spawnY = 7050.0,
    .numboxes = 5,
    .boxes = &level4_boxes[0],
    .nummelons = 4,
    .melons = &level4_melons[0],
};

#endif
