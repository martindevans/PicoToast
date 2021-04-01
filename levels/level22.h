#ifndef _level22_H
#define _level22_H

#include "levels.h"

box_t level22_boxes[] = {
    {
        .x = 1290,
        .y = 4830,
        .w = 10575,
        .h = 345,
        .exit_zone = true,
        .has_enemy = false
    },
    {
        .x = 1605,
        .y = 2175,
        .w = 240,
        .h = 1305,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 780,
        .y = 1950,
        .w = 1935,
        .h = 210,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 3480,
        .y = 1935,
        .w = 1935,
        .h = 210,
        .exit_zone = false,
        .has_enemy = true
    },
    {
        .x = 3510,
        .y = 3375,
        .w = 1935,
        .h = 210,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 5415,
        .y = 1920,
        .w = 180,
        .h = 1665,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 3330,
        .y = 1935,
        .w = 180,
        .h = 1665,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 6030,
        .y = 1905,
        .w = 180,
        .h = 1665,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 7455,
        .y = 1935,
        .w = 180,
        .h = 1665,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 6045,
        .y = 1845,
        .w = 1590,
        .h = 180,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 6030,
        .y = 2565,
        .w = 1590,
        .h = 180,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 8280,
        .y = 1860,
        .w = 1590,
        .h = 180,
        .exit_zone = false,
        .has_enemy = true
    },
    {
        .x = 8280,
        .y = 3585,
        .w = 1590,
        .h = 180,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 8265,
        .y = 1875,
        .w = 135,
        .h = 720,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 8265,
        .y = 2565,
        .w = 1605,
        .h = 210,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 9675,
        .y = 2790,
        .w = 210,
        .h = 795,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 10485,
        .y = 1845,
        .w = 2190,
        .h = 210,
        .exit_zone = false,
        .has_enemy = true
    },
    {
        .x = 11595,
        .y = 2055,
        .w = 180,
        .h = 2340,
        .exit_zone = false,
        .has_enemy = false
    },
};

melon_t level22_melons[] = {
    {
        .x = 2100.0,
        .y = 2700.0
    },
    {
        .x = 1080.0,
        .y = 2700.0
    },
    {
        .x = 4425.0,
        .y = 3945.0
    },
    {
        .x = 4485.0,
        .y = 1515.0
    },
    {
        .x = 6780.0,
        .y = 1380.0
    },
    {
        .x = 6765.0,
        .y = 3120.0
    },
    {
        .x = 8850.0,
        .y = 3105.0
    },
    {
        .x = 8835.0,
        .y = 1230.0
    },
    {
        .x = 10935.0,
        .y = 3105.0
    },
    {
        .x = 11010.0,
        .y = 3900.0
    },
};

level_t level22 = {
    .spawnX = 1005.0,
    .spawnY = 330.0,
    .numboxes = 18,
    .boxes = &level22_boxes[0],
    .nummelons = 10,
    .melons = &level22_melons[0],
};

#endif
