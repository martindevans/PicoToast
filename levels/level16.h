#ifndef _level16_H
#define _level16_H

#include "levels.h"

box_t level16_boxes[] = {
    {
        .x = 510,
        .y = 1470,
        .w = 495,
        .h = 495,
        .exit_zone = true,
        .has_enemy = false
    },
    {
        .x = 2460,
        .y = 3285,
        .w = 1575,
        .h = 180,
        .exit_zone = false,
        .has_enemy = true
    },
    {
        .x = 60,
        .y = 45,
        .w = 12900,
        .h = 30,
        .exit_zone = false,
        .has_enemy = true
    },
    {
        .x = 5280,
        .y = 60,
        .w = 135,
        .h = 2865,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 5115,
        .y = 6240,
        .w = 2085,
        .h = 105,
        .exit_zone = false,
        .has_enemy = true
    },
    {
        .x = 9465,
        .y = 4335,
        .w = 2085,
        .h = 105,
        .exit_zone = false,
        .has_enemy = true
    },
    {
        .x = 7275,
        .y = 1635,
        .w = 2085,
        .h = 105,
        .exit_zone = false,
        .has_enemy = true
    },
};

melon_t level16_melons[] = {
    {
        .x = 5640.0,
        .y = 255.0
    },
    {
        .x = 9060.0,
        .y = 1260.0
    },
    {
        .x = 9690.0,
        .y = 3885.0
    },
    {
        .x = 6870.0,
        .y = 5940.0
    },
    {
        .x = 2655.0,
        .y = 2985.0
    },
    {
        .x = 675.0,
        .y = 1140.0
    },
    {
        .x = 1200.0,
        .y = 1680.0
    },
};

level_t level16 = {
    .spawnX = 450.0,
    .spawnY = 300.0,
    .numboxes = 7,
    .boxes = &level16_boxes[0],
    .nummelons = 7,
    .melons = &level16_melons[0],
};

#endif
