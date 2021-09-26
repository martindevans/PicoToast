#ifndef _level30_H
#define _level30_H

#include "level.h"

box_t level30_boxes[] = {
    {
        .x = 1005,
        .y = 6825,
        .w = 165,
        .h = 90,
        .exit_zone = true,
        .has_enemy = false
    },
    {
        .x = 1065,
        .y = 7785,
        .w = 11385,
        .h = 120,
        .exit_zone = false,
        .has_enemy = true
    },
    {
        .x = 1065,
        .y = 7785,
        .w = 4665,
        .h = 105,
        .exit_zone = false,
        .has_enemy = true
    },
    {
        .x = 3495,
        .y = 7785,
        .w = 4650,
        .h = 90,
        .exit_zone = false,
        .has_enemy = true
    },
    {
        .x = 5715,
        .y = 7785,
        .w = 4665,
        .h = 105,
        .exit_zone = false,
        .has_enemy = true
    },
    {
        .x = 7755,
        .y = 7815,
        .w = 4665,
        .h = 75,
        .exit_zone = false,
        .has_enemy = true
    },
    {
        .x = 5055,
        .y = 3750,
        .w = 495,
        .h = 495,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 6720,
        .y = 4800,
        .w = 495,
        .h = 495,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 1815,
        .y = 7785,
        .w = 5025,
        .h = 120,
        .exit_zone = false,
        .has_enemy = true
    },
    {
        .x = 6255,
        .y = 7785,
        .w = 5220,
        .h = 120,
        .exit_zone = false,
        .has_enemy = true
    },
    {
        .x = 2340,
        .y = 7785,
        .w = 2520,
        .h = 120,
        .exit_zone = false,
        .has_enemy = true
    },
};

melon_t level30_melons[] = {
    {
        .x = 11505.0,
        .y = 6480.0
    },
    {
        .x = 8820.0,
        .y = 5280.0
    },
    {
        .x = 10350.0,
        .y = 4470.0
    },
    {
        .x = 6975.0,
        .y = 4470.0
    },
    {
        .x = 6285.0,
        .y = 6315.0
    },
    {
        .x = 4770.0,
        .y = 5265.0
    },
    {
        .x = 3645.0,
        .y = 6255.0
    },
    {
        .x = 5295.0,
        .y = 4725.0
    },
    {
        .x = 4185.0,
        .y = 4725.0
    },
    {
        .x = 2220.0,
        .y = 7095.0
    },
    {
        .x = 2235.0,
        .y = 5805.0
    },
    {
        .x = 1020.0,
        .y = 5250.0
    },
    {
        .x = 1020.0,
        .y = 3360.0
    },
    {
        .x = 1830.0,
        .y = 2220.0
    },
    {
        .x = 240.0,
        .y = 2235.0
    },
};

level_t level30 = {
    .spawnX = 840.0,
    .spawnY = 315.0,
    .numboxes = 11,
    .boxes = &level30_boxes[0],
    .nummelons = 15,
    .melons = &level30_melons[0],
};

#endif
