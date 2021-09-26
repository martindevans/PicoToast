#ifndef _level27_H
#define _level27_H

#include "level.h"

box_t level27_boxes[] = {
    {
        .x = 5565,
        .y = 9585,
        .w = 675,
        .h = 120,
        .exit_zone = true,
        .has_enemy = false
    },
    {
        .x = 465,
        .y = 2490,
        .w = 675,
        .h = 120,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 2580,
        .y = 2505,
        .w = 675,
        .h = 120,
        .exit_zone = false,
        .has_enemy = true
    },
    {
        .x = 4425,
        .y = 2535,
        .w = 675,
        .h = 120,
        .exit_zone = false,
        .has_enemy = true
    },
    {
        .x = 6330,
        .y = 2520,
        .w = 675,
        .h = 120,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 8055,
        .y = 2490,
        .w = 675,
        .h = 120,
        .exit_zone = false,
        .has_enemy = true
    },
    {
        .x = 9360,
        .y = 2505,
        .w = 675,
        .h = 120,
        .exit_zone = false,
        .has_enemy = true
    },
    {
        .x = 11265,
        .y = 2490,
        .w = 675,
        .h = 120,
        .exit_zone = false,
        .has_enemy = true
    },
};

melon_t level27_melons[] = {
    {
        .x = 5910.0,
        .y = 8805.0
    },
    {
        .x = 6000.0,
        .y = 7695.0
    },
    {
        .x = 6000.0,
        .y = 6660.0
    },
    {
        .x = 6030.0,
        .y = 5865.0
    },
    {
        .x = 6060.0,
        .y = 4830.0
    },
    {
        .x = 6090.0,
        .y = 3915.0
    },
    {
        .x = 6165.0,
        .y = 3270.0
    },
    {
        .x = 6570.0,
        .y = 2055.0
    },
};

level_t level27 = {
    .spawnX = 390.0,
    .spawnY = 330.0,
    .numboxes = 8,
    .boxes = &level27_boxes[0],
    .nummelons = 8,
    .melons = &level27_melons[0],
};

#endif
