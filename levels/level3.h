#ifndef _level3_H
#define _level3_H

#include "level.h"

box_t level3_boxes[] = {
    {
        .x = 3750,
        .y = 2715,
        .w = 3615,
        .h = 480,
        .exit_zone = true,
        .has_enemy = false
    },
    {
        .x = 3270,
        .y = 2715,
        .w = 495,
        .h = 480,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 7350,
        .y = 2715,
        .w = 495,
        .h = 480,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 3270,
        .y = 3180,
        .w = 4575,
        .h = 180,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 2355,
        .y = 4965,
        .w = 4455,
        .h = 90,
        .exit_zone = false,
        .has_enemy = true
    },
    {
        .x = 9855,
        .y = 1335,
        .w = 90,
        .h = 7995,
        .exit_zone = false,
        .has_enemy = false
    },
};

melon_t level3_melons[] = {
    {
        .x = 9405.0,
        .y = 1845.0
    },
    {
        .x = 9420.0,
        .y = 3765.0
    },
    {
        .x = 9405.0,
        .y = 5580.0
    },
    {
        .x = 9360.0,
        .y = 7455.0
    },
    {
        .x = 10395.0,
        .y = 2850.0
    },
    {
        .x = 10290.0,
        .y = 4740.0
    },
    {
        .x = 10440.0,
        .y = 6615.0
    },
    {
        .x = 10335.0,
        .y = 8520.0
    },
    {
        .x = 6450.0,
        .y = 4590.0
    },
    {
        .x = 2580.0,
        .y = 4485.0
    },
    {
        .x = 4545.0,
        .y = 4605.0
    },
};

level_t level3 = {
    .spawnX = 5205.0,
    .spawnY = 1275.0,
    .numboxes = 6,
    .boxes = &level3_boxes[0],
    .nummelons = 11,
    .melons = &level3_melons[0],
};

#endif
