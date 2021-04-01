#ifndef _level10_H
#define _level10_H

#include "levels.h"

box_t level10_boxes[] = {
    {
        .x = 1725,
        .y = 285,
        .w = 1680,
        .h = 150,
        .exit_zone = true,
        .has_enemy = false
    },
    {
        .x = 195,
        .y = 3690,
        .w = 240,
        .h = 5775,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 195,
        .y = 9450,
        .w = 1605,
        .h = 210,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 1560,
        .y = 3675,
        .w = 240,
        .h = 5790,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 11535,
        .y = 8790,
        .w = 495,
        .h = 495,
        .exit_zone = false,
        .has_enemy = false
    },
};

melon_t level10_melons[] = {
    {
        .x = 11715.0,
        .y = 8445.0
    },
    {
        .x = 630.0,
        .y = 8355.0
    },
    {
        .x = 1230.0,
        .y = 8325.0
    },
    {
        .x = 930.0,
        .y = 8340.0
    },
    {
        .x = 630.0,
        .y = 8100.0
    },
    {
        .x = 960.0,
        .y = 8085.0
    },
    {
        .x = 1260.0,
        .y = 8085.0
    },
    {
        .x = 630.0,
        .y = 7785.0
    },
    {
        .x = 930.0,
        .y = 7785.0
    },
    {
        .x = 1290.0,
        .y = 7800.0
    },
    {
        .x = 630.0,
        .y = 7440.0
    },
    {
        .x = 990.0,
        .y = 7455.0
    },
    {
        .x = 1320.0,
        .y = 7440.0
    },
    {
        .x = 615.0,
        .y = 7140.0
    },
    {
        .x = 990.0,
        .y = 7140.0
    },
    {
        .x = 1290.0,
        .y = 7110.0
    },
    {
        .x = 11130.0,
        .y = 8955.0
    },
    {
        .x = 12255.0,
        .y = 8940.0
    },
};

level_t level10 = {
    .spawnX = 705.0,
    .spawnY = 8655.0,
    .numboxes = 5,
    .boxes = &level10_boxes[0],
    .nummelons = 18,
    .melons = &level10_melons[0],
};

#endif
