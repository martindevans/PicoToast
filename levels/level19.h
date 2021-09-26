#ifndef _level19_H
#define _level19_H

#include "level.h"

box_t level19_boxes[] = {
    {
        .x = 8310,
        .y = 2265,
        .w = 120,
        .h = 2925,
        .exit_zone = true,
        .has_enemy = false
    },
    {
        .x = 4770,
        .y = 7440,
        .w = 1935,
        .h = 315,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 5790,
        .y = 9840,
        .w = 120,
        .h = 15,
        .exit_zone = false,
        .has_enemy = false
    },
};

melon_t level19_melons[] = {
    {
        .x = 7935.0,
        .y = 4860.0
    },
    {
        .x = 7905.0,
        .y = 2805.0
    },
    {
        .x = 8775.0,
        .y = 3615.0
    },
    {
        .x = 5745.0,
        .y = 7950.0
    },
};

level_t level19 = {
    .spawnX = 12135.0,
    .spawnY = 195.0,
    .numboxes = 3,
    .boxes = &level19_boxes[0],
    .nummelons = 4,
    .melons = &level19_melons[0],
};

#endif
