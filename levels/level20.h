#ifndef _level20_H
#define _level20_H

#include "level.h"

box_t level20_boxes[] = {
    {
        .x = 4860,
        .y = 9405,
        .w = 3270,
        .h = 135,
        .exit_zone = true,
        .has_enemy = false
    },
    {
        .x = 4920,
        .y = 6750,
        .w = 3270,
        .h = 135,
        .exit_zone = false,
        .has_enemy = true
    },
    {
        .x = 4995,
        .y = 5190,
        .w = 3270,
        .h = 135,
        .exit_zone = false,
        .has_enemy = true
    },
    {
        .x = 4935,
        .y = 3960,
        .w = 3270,
        .h = 135,
        .exit_zone = false,
        .has_enemy = true
    },
    {
        .x = 4965,
        .y = 2910,
        .w = 3270,
        .h = 135,
        .exit_zone = false,
        .has_enemy = true
    },
    {
        .x = 4920,
        .y = 1725,
        .w = 3270,
        .h = 135,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 12150,
        .y = 2640,
        .w = 120,
        .h = 3390,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 1695,
        .y = 2415,
        .w = 120,
        .h = 3390,
        .exit_zone = false,
        .has_enemy = false
    },
};

melon_t level20_melons[] = {
    {
        .x = 2265.0,
        .y = 4005.0
    },
    {
        .x = 11805.0,
        .y = 4320.0
    },
    {
        .x = 6420.0,
        .y = 1395.0
    },
    {
        .x = 6510.0,
        .y = 2550.0
    },
    {
        .x = 6615.0,
        .y = 3570.0
    },
    {
        .x = 6495.0,
        .y = 4770.0
    },
    {
        .x = 6555.0,
        .y = 6405.0
    },
    {
        .x = 6495.0,
        .y = 9105.0
    },
    {
        .x = 8445.0,
        .y = 6750.0
    },
    {
        .x = 8715.0,
        .y = 5160.0
    },
    {
        .x = 8640.0,
        .y = 3945.0
    },
    {
        .x = 8580.0,
        .y = 2880.0
    },
    {
        .x = 8610.0,
        .y = 1695.0
    },
    {
        .x = 4530.0,
        .y = 1680.0
    },
    {
        .x = 4500.0,
        .y = 2895.0
    },
    {
        .x = 4560.0,
        .y = 3960.0
    },
    {
        .x = 4425.0,
        .y = 5115.0
    },
    {
        .x = 4425.0,
        .y = 6645.0
    },
};

level_t level20 = {
    .spawnX = 5850.0,
    .spawnY = 240.0,
    .numboxes = 8,
    .boxes = &level20_boxes[0],
    .nummelons = 18,
    .melons = &level20_melons[0],
};

#endif
