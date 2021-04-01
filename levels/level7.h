#ifndef _level7_H
#define _level7_H

#include "levels.h"

box_t level7_boxes[] = {
    {
        .x = 11565,
        .y = 255,
        .w = 180,
        .h = 195,
        .exit_zone = true,
        .has_enemy = false
    },
    {
        .x = 1650,
        .y = 3630,
        .w = 165,
        .h = 4695,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 3930,
        .y = 3720,
        .w = 225,
        .h = 4560,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 11280,
        .y = 8010,
        .w = 495,
        .h = 495,
        .exit_zone = false,
        .has_enemy = false
    },
};

melon_t level7_melons[] = {
    {
        .x = 7680.0,
        .y = 3105.0
    },
};

level_t level7 = {
    .spawnX = 2535.0,
    .spawnY = 360.0,
    .numboxes = 4,
    .boxes = &level7_boxes[0],
    .nummelons = 1,
    .melons = &level7_melons[0],
};

#endif
