#ifndef _level12_H
#define _level12_H

#include "level.h"

box_t level12_boxes[] = {
    {
        .x = 150,
        .y = 105,
        .w = 495,
        .h = 495,
        .exit_zone = true,
        .has_enemy = false
    },
    {
        .x = 300,
        .y = 9195,
        .w = 495,
        .h = 495,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 2925,
        .y = 7320,
        .w = 495,
        .h = 495,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 5685,
        .y = 5340,
        .w = 495,
        .h = 495,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 8415,
        .y = 3375,
        .w = 495,
        .h = 495,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 11565,
        .y = 1215,
        .w = 495,
        .h = 495,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 5385,
        .y = 90,
        .w = 1035,
        .h = 4305,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 12015,
        .y = 9090,
        .w = 495,
        .h = 495,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 9090,
        .y = 8355,
        .w = 495,
        .h = 495,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 6570,
        .y = 7500,
        .w = 495,
        .h = 495,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 30,
        .y = 15,
        .w = 12915,
        .h = 105,
        .exit_zone = false,
        .has_enemy = false
    },
};

melon_t level12_melons[] = {
    {
        .x = 11550.0,
        .y = 810.0
    },
    {
        .x = 11925.0,
        .y = 810.0
    },
    {
        .x = 11535.0,
        .y = 480.0
    },
    {
        .x = 11895.0,
        .y = 480.0
    },
    {
        .x = 11550.0,
        .y = 195.0
    },
    {
        .x = 11865.0,
        .y = 210.0
    },
    {
        .x = 12180.0,
        .y = 8790.0
    },
    {
        .x = 12165.0,
        .y = 8445.0
    },
    {
        .x = 12180.0,
        .y = 8130.0
    },
    {
        .x = 12225.0,
        .y = 7800.0
    },
    {
        .x = 12210.0,
        .y = 7440.0
    },
    {
        .x = 12240.0,
        .y = 7065.0
    },
    {
        .x = 12225.0,
        .y = 6720.0
    },
    {
        .x = 12195.0,
        .y = 6330.0
    },
    {
        .x = 12150.0,
        .y = 6045.0
    },
};

level_t level12 = {
    .spawnX = 255.0,
    .spawnY = 8430.0,
    .numboxes = 11,
    .boxes = &level12_boxes[0],
    .nummelons = 15,
    .melons = &level12_melons[0],
};

#endif
