#ifndef _level25_H
#define _level25_H

#include "levels.h"

box_t level25_boxes[] = {
    {
        .x = 12615,
        .y = 3870,
        .w = 120,
        .h = 780,
        .exit_zone = true,
        .has_enemy = false
    },
    {
        .x = 375,
        .y = 8580,
        .w = 855,
        .h = 150,
        .exit_zone = false,
        .has_enemy = true
    },
    {
        .x = 1095,
        .y = 8580,
        .w = 855,
        .h = 150,
        .exit_zone = false,
        .has_enemy = true
    },
    {
        .x = 1785,
        .y = 8580,
        .w = 855,
        .h = 150,
        .exit_zone = false,
        .has_enemy = true
    },
    {
        .x = 2460,
        .y = 8580,
        .w = 855,
        .h = 150,
        .exit_zone = false,
        .has_enemy = true
    },
    {
        .x = 3060,
        .y = 8580,
        .w = 855,
        .h = 150,
        .exit_zone = false,
        .has_enemy = true
    },
    {
        .x = 3750,
        .y = 8580,
        .w = 855,
        .h = 150,
        .exit_zone = false,
        .has_enemy = true
    },
    {
        .x = 4485,
        .y = 8580,
        .w = 855,
        .h = 150,
        .exit_zone = false,
        .has_enemy = true
    },
    {
        .x = 5175,
        .y = 8580,
        .w = 855,
        .h = 150,
        .exit_zone = false,
        .has_enemy = true
    },
    {
        .x = 5790,
        .y = 8580,
        .w = 855,
        .h = 150,
        .exit_zone = false,
        .has_enemy = true
    },
    {
        .x = 6495,
        .y = 8580,
        .w = 855,
        .h = 150,
        .exit_zone = false,
        .has_enemy = true
    },
    {
        .x = 7185,
        .y = 8580,
        .w = 855,
        .h = 150,
        .exit_zone = false,
        .has_enemy = true
    },
    {
        .x = 7845,
        .y = 8580,
        .w = 855,
        .h = 150,
        .exit_zone = false,
        .has_enemy = true
    },
    {
        .x = 8520,
        .y = 8580,
        .w = 855,
        .h = 150,
        .exit_zone = false,
        .has_enemy = true
    },
    {
        .x = 9165,
        .y = 8580,
        .w = 855,
        .h = 150,
        .exit_zone = false,
        .has_enemy = true
    },
    {
        .x = 9810,
        .y = 8580,
        .w = 855,
        .h = 150,
        .exit_zone = false,
        .has_enemy = true
    },
    {
        .x = 10545,
        .y = 8580,
        .w = 855,
        .h = 150,
        .exit_zone = false,
        .has_enemy = true
    },
    {
        .x = 11190,
        .y = 8580,
        .w = 855,
        .h = 150,
        .exit_zone = false,
        .has_enemy = true
    },
    {
        .x = 60,
        .y = 4710,
        .w = 855,
        .h = 150,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 2445,
        .y = 3915,
        .w = 195,
        .h = 120,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 5475,
        .y = 3930,
        .w = 195,
        .h = 120,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 9480,
        .y = 3930,
        .w = 195,
        .h = 120,
        .exit_zone = false,
        .has_enemy = false
    },
};

melon_t level25_melons[] = {
    {
        .x = 2490.0,
        .y = 3615.0
    },
    {
        .x = 5520.0,
        .y = 3645.0
    },
    {
        .x = 9510.0,
        .y = 3645.0
    },
    {
        .x = 9525.0,
        .y = 2820.0
    },
    {
        .x = 5580.0,
        .y = 2550.0
    },
    {
        .x = 2445.0,
        .y = 2520.0
    },
    {
        .x = 4050.0,
        .y = 2910.0
    },
    {
        .x = 7755.0,
        .y = 2970.0
    },
};

level_t level25 = {
    .spawnX = 120.0,
    .spawnY = 3930.0,
    .numboxes = 22,
    .boxes = &level25_boxes[0],
    .nummelons = 8,
    .melons = &level25_melons[0],
};

#endif
