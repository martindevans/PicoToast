#ifndef _level1_H
#define _level1_H

#include "level.h"

box_t level1_boxes[] = {
    {
        .x = 3420,
        .y = 1950,
        .w = 4260,
        .h = 405,
        .exit_zone = true,
        .has_enemy = false
    },
};

melon_t level1_melons[] = {
    {
        .x = 3750.0,
        .y = 1515.0
    },
    {
        .x = 7290.0,
        .y = 1545.0
    },
};

level_t level1 = {
    .spawnX = 5467.5,
    .spawnY = 1072.5,
    .numboxes = 1,
    .boxes = &level1_boxes[0],
    .nummelons = 2,
    .melons = &level1_melons[0],
};

#endif
