#ifndef _level14_H
#define _level14_H

#include "levels.h"

box_t level14_boxes[] = {
    {
        .x = 5625,
        .y = 4725,
        .w = 600,
        .h = 585,
        .exit_zone = true,
        .has_enemy = false
    },
    {
        .x = 5985,
        .y = 9015,
        .w = 60,
        .h = 15,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 10680,
        .y = 1485,
        .w = 495,
        .h = 495,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 10590,
        .y = 6855,
        .w = 495,
        .h = 495,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 5670,
        .y = 1245,
        .w = 495,
        .h = 495,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 570,
        .y = 4695,
        .w = 495,
        .h = 495,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 10620,
        .y = 4920,
        .w = 495,
        .h = 495,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 630,
        .y = 6810,
        .w = 495,
        .h = 495,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 660,
        .y = 1275,
        .w = 495,
        .h = 495,
        .exit_zone = false,
        .has_enemy = false
    },
};

melon_t level14_melons[] = {
    {
        .x = 750.0,
        .y = 4320.0
    },
    {
        .x = 10800.0,
        .y = 4515.0
    },
    {
        .x = 10740.0,
        .y = 6390.0
    },
    {
        .x = 10860.0,
        .y = 1020.0
    },
    {
        .x = 5835.0,
        .y = 735.0
    },
    {
        .x = 840.0,
        .y = 735.0
    },
    {
        .x = 810.0,
        .y = 6315.0
    },
    {
        .x = 5835.0,
        .y = 4170.0
    },
    {
        .x = 6855.0,
        .y = 4890.0
    },
    {
        .x = 4950.0,
        .y = 4905.0
    },
    {
        .x = 5895.0,
        .y = 5760.0
    },
    {
        .x = 6855.0,
        .y = 8235.0
    },
    {
        .x = 4920.0,
        .y = 8160.0
    },
};

level_t level14 = {
    .spawnX = 5730.0,
    .spawnY = 8310.0,
    .numboxes = 9,
    .boxes = &level14_boxes[0],
    .nummelons = 13,
    .melons = &level14_melons[0],
};

#endif
