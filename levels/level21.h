#ifndef _level21_H
#define _level21_H

#include "level.h"

box_t level21_boxes[] = {
    {
        .x = 10050,
        .y = 8520,
        .w = 2535,
        .h = 285,
        .exit_zone = true,
        .has_enemy = false
    },
    {
        .x = 6855,
        .y = 7065,
        .w = 2535,
        .h = 285,
        .exit_zone = false,
        .has_enemy = true
    },
    {
        .x = 4020,
        .y = 6015,
        .w = 2535,
        .h = 285,
        .exit_zone = false,
        .has_enemy = true
    },
    {
        .x = 1965,
        .y = 5250,
        .w = 2535,
        .h = 285,
        .exit_zone = false,
        .has_enemy = true
    },
    {
        .x = 5070,
        .y = 3930,
        .w = 2535,
        .h = 285,
        .exit_zone = false,
        .has_enemy = true
    },
    {
        .x = 7575,
        .y = 2970,
        .w = 2535,
        .h = 285,
        .exit_zone = false,
        .has_enemy = true
    },
    {
        .x = 11010,
        .y = 255,
        .w = 255,
        .h = 2730,
        .exit_zone = false,
        .has_enemy = false
    },
};

melon_t level21_melons[] = {
    {
        .x = 10770.0,
        .y = 2445.0
    },
    {
        .x = 10740.0,
        .y = 1920.0
    },
    {
        .x = 10695.0,
        .y = 1455.0
    },
    {
        .x = 10725.0,
        .y = 855.0
    },
    {
        .x = 10710.0,
        .y = 465.0
    },
    {
        .x = 9705.0,
        .y = 2625.0
    },
    {
        .x = 9345.0,
        .y = 2640.0
    },
    {
        .x = 8925.0,
        .y = 2640.0
    },
    {
        .x = 8415.0,
        .y = 2640.0
    },
    {
        .x = 7980.0,
        .y = 2670.0
    },
    {
        .x = 6270.0,
        .y = 3615.0
    },
    {
        .x = 3045.0,
        .y = 4995.0
    },
    {
        .x = 5130.0,
        .y = 5715.0
    },
    {
        .x = 8025.0,
        .y = 6720.0
    },
};

level_t level21 = {
    .spawnX = 10800.0,
    .spawnY = 7260.0,
    .numboxes = 7,
    .boxes = &level21_boxes[0],
    .nummelons = 14,
    .melons = &level21_melons[0],
};

#endif
