#ifndef _level26_H
#define _level26_H

#include "levels.h"

box_t level26_boxes[] = {
    {
        .x = 10335,
        .y = 270,
        .w = 795,
        .h = 135,
        .exit_zone = true,
        .has_enemy = false
    },
    {
        .x = 255,
        .y = 615,
        .w = 195,
        .h = 7350,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 1080,
        .y = 615,
        .w = 180,
        .h = 6915,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 255,
        .y = 7995,
        .w = 4305,
        .h = 180,
        .exit_zone = false,
        .has_enemy = true
    },
    {
        .x = 4275,
        .y = 7995,
        .w = 4305,
        .h = 180,
        .exit_zone = false,
        .has_enemy = true
    },
    {
        .x = 7710,
        .y = 7995,
        .w = 4305,
        .h = 180,
        .exit_zone = false,
        .has_enemy = true
    },
    {
        .x = 5985,
        .y = 6135,
        .w = 1515,
        .h = 165,
        .exit_zone = false,
        .has_enemy = true
    },
    {
        .x = 10065,
        .y = 4650,
        .w = 1515,
        .h = 165,
        .exit_zone = false,
        .has_enemy = true
    },
    {
        .x = 7065,
        .y = 2130,
        .w = 1515,
        .h = 165,
        .exit_zone = false,
        .has_enemy = true
    },
};

melon_t level26_melons[] = {
    {
        .x = 690.0,
        .y = 945.0
    },
    {
        .x = 705.0,
        .y = 1335.0
    },
    {
        .x = 675.0,
        .y = 1740.0
    },
    {
        .x = 660.0,
        .y = 2130.0
    },
    {
        .x = 690.0,
        .y = 2535.0
    },
    {
        .x = 675.0,
        .y = 2985.0
    },
    {
        .x = 675.0,
        .y = 3435.0
    },
    {
        .x = 690.0,
        .y = 3870.0
    },
    {
        .x = 675.0,
        .y = 4335.0
    },
    {
        .x = 675.0,
        .y = 4800.0
    },
    {
        .x = 690.0,
        .y = 5310.0
    },
    {
        .x = 690.0,
        .y = 5850.0
    },
    {
        .x = 705.0,
        .y = 6375.0
    },
    {
        .x = 675.0,
        .y = 7020.0
    },
    {
        .x = 5325.0,
        .y = 7050.0
    },
    {
        .x = 9720.0,
        .y = 7080.0
    },
    {
        .x = 6675.0,
        .y = 5235.0
    },
    {
        .x = 10545.0,
        .y = 3735.0
    },
    {
        .x = 10680.0,
        .y = 525.0
    },
};

level_t level26 = {
    .spawnX = 450.0,
    .spawnY = 120.0,
    .numboxes = 9,
    .boxes = &level26_boxes[0],
    .nummelons = 19,
    .melons = &level26_melons[0],
};

#endif
