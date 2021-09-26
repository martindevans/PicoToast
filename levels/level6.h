#ifndef _level6_H
#define _level6_H

#include "level.h"

box_t level6_boxes[] = {
    {
        .x = 13050,
        .y = 270,
        .w = 495,
        .h = 495,
        .exit_zone = true,
        .has_enemy = false
    },
    {
        .x = 150,
        .y = 105,
        .w = 495,
        .h = 495,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 6585,
        .y = 9060,
        .w = 1065,
        .h = 120,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 1860,
        .y = 6480,
        .w = 495,
        .h = 495,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 11535,
        .y = 5265,
        .w = 495,
        .h = 495,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 1830,
        .y = 2625,
        .w = 495,
        .h = 495,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 11415,
        .y = 1095,
        .w = 495,
        .h = 495,
        .exit_zone = false,
        .has_enemy = false
    },
};

melon_t level6_melons[] = {
    {
        .x = 7110.0,
        .y = 870.0
    },
};

level_t level6 = {
    .spawnX = 6810.0,
    .spawnY = 8310.0,
    .numboxes = 7,
    .boxes = &level6_boxes[0],
    .nummelons = 1,
    .melons = &level6_melons[0],
};

#endif
