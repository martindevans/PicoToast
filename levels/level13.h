#ifndef _level13_H
#define _level13_H

#include "level.h"

box_t level13_boxes[] = {
    {
        .x = 6420,
        .y = 270,
        .w = 180,
        .h = 180,
        .exit_zone = true,
        .has_enemy = false
    },
    {
        .x = 7140,
        .y = 90,
        .w = 120,
        .h = 8310,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 45,
        .y = 30,
        .w = 12915,
        .h = 75,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 7230,
        .y = 9555,
        .w = 5685,
        .h = 240,
        .exit_zone = false,
        .has_enemy = true
    },
    {
        .x = 60,
        .y = 9615,
        .w = 5685,
        .h = 240,
        .exit_zone = false,
        .has_enemy = true
    },
    {
        .x = 7245,
        .y = 8160,
        .w = 1065,
        .h = 240,
        .exit_zone = false,
        .has_enemy = true
    },
    {
        .x = 4695,
        .y = 3510,
        .w = 1050,
        .h = 225,
        .exit_zone = false,
        .has_enemy = true
    },
    {
        .x = 4680,
        .y = 5970,
        .w = 1050,
        .h = 240,
        .exit_zone = false,
        .has_enemy = true
    },
    {
        .x = 4665,
        .y = 8130,
        .w = 1065,
        .h = 240,
        .exit_zone = false,
        .has_enemy = true
    },
    {
        .x = 7245,
        .y = 5955,
        .w = 1065,
        .h = 240,
        .exit_zone = false,
        .has_enemy = true
    },
    {
        .x = 7245,
        .y = 3510,
        .w = 1065,
        .h = 225,
        .exit_zone = false,
        .has_enemy = true
    },
    {
        .x = 7245,
        .y = 1455,
        .w = 1065,
        .h = 240,
        .exit_zone = false,
        .has_enemy = true
    },
    {
        .x = 4695,
        .y = 1380,
        .w = 1035,
        .h = 210,
        .exit_zone = false,
        .has_enemy = true
    },
    {
        .x = 90,
        .y = 6600,
        .w = 495,
        .h = 495,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 3720,
        .y = 3645,
        .w = 495,
        .h = 495,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 120,
        .y = 930,
        .w = 495,
        .h = 495,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 12375,
        .y = 6720,
        .w = 495,
        .h = 495,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 8820,
        .y = 3630,
        .w = 495,
        .h = 495,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 12375,
        .y = 795,
        .w = 495,
        .h = 495,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 5715,
        .y = 105,
        .w = 90,
        .h = 8265,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 5880,
        .y = 1965,
        .w = 105,
        .h = 120,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 6960,
        .y = 1965,
        .w = 90,
        .h = 120,
        .exit_zone = false,
        .has_enemy = false
    },
};

melon_t level13_melons[] = {
    {
        .x = 12555.0,
        .y = 390.0
    },
    {
        .x = 315.0,
        .y = 435.0
    },
    {
        .x = 8985.0,
        .y = 3300.0
    },
    {
        .x = 3945.0,
        .y = 3270.0
    },
    {
        .x = 12540.0,
        .y = 6450.0
    },
    {
        .x = 315.0,
        .y = 6270.0
    },
    {
        .x = 7245.0,
        .y = 8880.0
    },
    {
        .x = 5625.0,
        .y = 8925.0
    },
};

level_t level13 = {
    .spawnX = 5925.0,
    .spawnY = 870.0,
    .numboxes = 22,
    .boxes = &level13_boxes[0],
    .nummelons = 8,
    .melons = &level13_melons[0],
};

#endif
