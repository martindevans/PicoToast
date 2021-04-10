/*
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "sprite.h"

#include "pico/platform.h" // for __not_in_flash

// Note some of the sprite routines are quite large (unrolled), so trying to
// keep everything in separate sections so the linker can garbage collect
// unused sprite code. In particular we usually need 8bpp xor 16bpp functions!
#define __ram_func(foo) __not_in_flash_func(foo)
