#ifndef _level18_H
#define _level18_H

#include "level.h"

box_t level18_boxes[] = {
    {
        .x = 4725,
        .y = 9735,
        .w = 3180,
        .h = 90,
        .exit_zone = true,
        .has_enemy = true
    },
    {
        .x = 5745,
        .y = 1365,
        .w = 60,
        .h = 210,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 5760,
        .y = 2955,
        .w = 60,
        .h = 210,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 5745,
        .y = 4410,
        .w = 60,
        .h = 210,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 5775,
        .y = 5775,
        .w = 60,
        .h = 210,
        .exit_zone = false,
        .has_enemy = false
    },
};

melon_t level18_melons[] = {
    {
        .x = 5430.0,
        .y = 5775.0
    },
    {
        .x = 5445.0,
        .y = 4380.0
    },
    {
        .x = 5430.0,
        .y = 3015.0
    },
    {
        .x = 5370.0,
        .y = 1365.0
    },
    {
        .x = 6030.0,
        .y = 1365.0
    },
    {
        .x = 6015.0,
        .y = 2985.0
    },
    {
        .x = 5985.0,
        .y = 4485.0
    },
    {
        .x = 6135.0,
        .y = 5805.0
    },
};

level_t level18 = {
    .spawnX = 5400.0,
    .spawnY = 165.0,
    .numboxes = 5,
    .boxes = &level18_boxes[0],
    .nummelons = 8,
    .melons = &level18_melons[0],
};

#endif
