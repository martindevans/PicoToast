#ifndef _level28_H
#define _level28_H

#include "levels.h"

box_t level28_boxes[] = {
    {
        .x = 12195,
        .y = 2925,
        .w = 180,
        .h = 3825,
        .exit_zone = true,
        .has_enemy = false
    },
    {
        .x = 525,
        .y = 8520,
        .w = 2730,
        .h = 240,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 3240,
        .y = 8340,
        .w = 465,
        .h = 180,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 3555,
        .y = 8160,
        .w = 465,
        .h = 180,
        .exit_zone = false,
        .has_enemy = true
    },
    {
        .x = 3255,
        .y = 8415,
        .w = 930,
        .h = 345,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 3690,
        .y = 8160,
        .w = 495,
        .h = 315,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 4005,
        .y = 5955,
        .w = 180,
        .h = 2220,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 4005,
        .y = 5760,
        .w = 3630,
        .h = 210,
        .exit_zone = false,
        .has_enemy = true
    },
    {
        .x = 7470,
        .y = 5955,
        .w = 165,
        .h = 2160,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 4020,
        .y = 7935,
        .w = 3615,
        .h = 195,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 4035,
        .y = 7935,
        .w = 3600,
        .h = 825,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 4185,
        .y = 4800,
        .w = 660,
        .h = 945,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 4305,
        .y = 4335,
        .w = 105,
        .h = 465,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 4575,
        .y = 4335,
        .w = 105,
        .h = 465,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 4470,
        .y = 6180,
        .w = 90,
        .h = 540,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 5040,
        .y = 6165,
        .w = 90,
        .h = 540,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 4755,
        .y = 6165,
        .w = 90,
        .h = 540,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 4455,
        .y = 6075,
        .w = 675,
        .h = 105,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 4470,
        .y = 6690,
        .w = 675,
        .h = 105,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 7065,
        .y = 4905,
        .w = 675,
        .h = 105,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 7020,
        .y = 4815,
        .w = 165,
        .h = 930,
        .exit_zone = false,
        .has_enemy = false
    },
};

melon_t level28_melons[] = {
    {
        .x = 4575.0,
        .y = 4005.0
    },
    {
        .x = 4260.0,
        .y = 3990.0
    },
    {
        .x = 4560.0,
        .y = 3675.0
    },
    {
        .x = 4320.0,
        .y = 3645.0
    },
    {
        .x = 7575.0,
        .y = 4560.0
    },
    {
        .x = 3885.0,
        .y = 630.0
    },
    {
        .x = 4035.0,
        .y = 495.0
    },
    {
        .x = 4200.0,
        .y = 390.0
    },
    {
        .x = 4380.0,
        .y = 435.0
    },
    {
        .x = 4545.0,
        .y = 555.0
    },
    {
        .x = 4710.0,
        .y = 465.0
    },
    {
        .x = 4890.0,
        .y = 375.0
    },
    {
        .x = 5070.0,
        .y = 435.0
    },
    {
        .x = 5265.0,
        .y = 570.0
    },
    {
        .x = 5415.0,
        .y = 735.0
    },
    {
        .x = 5505.0,
        .y = 915.0
    },
    {
        .x = 5400.0,
        .y = 1110.0
    },
    {
        .x = 5190.0,
        .y = 1185.0
    },
    {
        .x = 4935.0,
        .y = 1230.0
    },
    {
        .x = 4680.0,
        .y = 1230.0
    },
    {
        .x = 4410.0,
        .y = 1215.0
    },
    {
        .x = 4125.0,
        .y = 1215.0
    },
    {
        .x = 3900.0,
        .y = 1200.0
    },
    {
        .x = 3645.0,
        .y = 1185.0
    },
    {
        .x = 3405.0,
        .y = 1110.0
    },
    {
        .x = 3255.0,
        .y = 930.0
    },
    {
        .x = 3285.0,
        .y = 675.0
    },
    {
        .x = 3480.0,
        .y = 495.0
    },
    {
        .x = 3690.0,
        .y = 630.0
    },
};

level_t level28 = {
    .spawnX = 585.0,
    .spawnY = 7395.0,
    .numboxes = 21,
    .boxes = &level28_boxes[0],
    .nummelons = 29,
    .melons = &level28_melons[0],
};

#endif
