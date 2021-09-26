#ifndef _level23_H
#define _level23_H

#include "level.h"

box_t level23_boxes[] = {
    {
        .x = 5280,
        .y = 15,
        .w = 1665,
        .h = 150,
        .exit_zone = true,
        .has_enemy = false
    },
    {
        .x = 1020,
        .y = 8700,
        .w = 10455,
        .h = 360,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 6420,
        .y = 2265,
        .w = 180,
        .h = 5595,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 5505,
        .y = 2235,
        .w = 180,
        .h = 5595,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 6450,
        .y = 7335,
        .w = 5865,
        .h = 525,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = 8775,
        .y = 7335,
        .w = 5865,
        .h = 525,
        .exit_zone = false,
        .has_enemy = false
    },
    {
        .x = -165,
        .y = 7320,
        .w = 5865,
        .h = 525,
        .exit_zone = false,
        .has_enemy = false
    },
};

melon_t level23_melons[] = {
    {
        .x = 7260.0,
        .y = 8340.0
    },
    {
        .x = 7860.0,
        .y = 8325.0
    },
    {
        .x = 8550.0,
        .y = 8325.0
    },
    {
        .x = 9570.0,
        .y = 8310.0
    },
    {
        .x = 9015.0,
        .y = 8325.0
    },
    {
        .x = 10125.0,
        .y = 8325.0
    },
    {
        .x = 10815.0,
        .y = 8340.0
    },
    {
        .x = 5085.0,
        .y = 8400.0
    },
    {
        .x = 4320.0,
        .y = 8430.0
    },
    {
        .x = 3570.0,
        .y = 8385.0
    },
    {
        .x = 2910.0,
        .y = 8370.0
    },
    {
        .x = 2415.0,
        .y = 8355.0
    },
    {
        .x = 1740.0,
        .y = 8310.0
    },
    {
        .x = 1200.0,
        .y = 8265.0
    },
};

level_t level23 = {
    .spawnX = 5730.0,
    .spawnY = 7800.0,
    .numboxes = 7,
    .boxes = &level23_boxes[0],
    .nummelons = 14,
    .melons = &level23_melons[0],
};

#endif
