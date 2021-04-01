#ifndef _level29_H
#define _level29_H

#include "levels.h"

box_t level29_boxes[] = {
    {
        .x = 270,
        .y = 9150,
        .w = 2370,
        .h = 240,
        .exit_zone = true,
        .has_enemy = false
    },
    {
        .x = 4380,
        .y = 6870,
        .w = 2370,
        .h = 240,
        .exit_zone = false,
        .has_enemy = true
    },
    {
        .x = 10230,
        .y = 9135,
        .w = 2370,
        .h = 240,
        .exit_zone = false,
        .has_enemy = true
    },
    {
        .x = 9960,
        .y = 4815,
        .w = 2370,
        .h = 240,
        .exit_zone = false,
        .has_enemy = true
    },
    {
        .x = 285,
        .y = 4710,
        .w = 2370,
        .h = 240,
        .exit_zone = false,
        .has_enemy = true
    },
    {
        .x = 4800,
        .y = 2835,
        .w = 2370,
        .h = 240,
        .exit_zone = false,
        .has_enemy = true
    },
    {
        .x = 9795,
        .y = 705,
        .w = 2370,
        .h = 240,
        .exit_zone = false,
        .has_enemy = true
    },
    {
        .x = 195,
        .y = 945,
        .w = 2370,
        .h = 240,
        .exit_zone = false,
        .has_enemy = true
    },
};

melon_t level29_melons[] = {
    {
        .x = 12285.0,
        .y = 8730.0
    },
    {
        .x = 10530.0,
        .y = 8670.0
    },
    {
        .x = 6465.0,
        .y = 6465.0
    },
    {
        .x = 4545.0,
        .y = 6420.0
    },
    {
        .x = 2355.0,
        .y = 8790.0
    },
    {
        .x = 510.0,
        .y = 8820.0
    },
    {
        .x = 2325.0,
        .y = 4335.0
    },
    {
        .x = 480.0,
        .y = 4320.0
    },
    {
        .x = 11970.0,
        .y = 4470.0
    },
    {
        .x = 10200.0,
        .y = 4515.0
    },
    {
        .x = 6840.0,
        .y = 2565.0
    },
    {
        .x = 4995.0,
        .y = 2520.0
    },
    {
        .x = 11865.0,
        .y = 405.0
    },
    {
        .x = 9960.0,
        .y = 465.0
    },
    {
        .x = 2235.0,
        .y = 750.0
    },
    {
        .x = 390.0,
        .y = 675.0
    },
};

level_t level29 = {
    .spawnX = 1050.0,
    .spawnY = 8100.0,
    .numboxes = 8,
    .boxes = &level29_boxes[0],
    .nummelons = 16,
    .melons = &level29_melons[0],
};

#endif
