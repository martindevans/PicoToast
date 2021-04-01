/*
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _SPRITE_H
#define _SPRITE_H

#include "pico/types.h"
#include "affine_transform.h"

#include "pico/scanvideo.h"
#include "pico/scanvideo/composable_scanline.h"

// Helper functions to:
// - Get a scanbuf into a state where a region of it can be directly rendered to,
//   and return a pointer to this region
// - After rendering, manipulate this scanbuffer into a form where PIO can
//   yeet it out on VGA

static inline uint16_t *raw_scanline_prepare(struct scanvideo_scanline_buffer *dest, uint width) {
    assert(width >= 3);
    assert(width % 2 == 0);
    // +1 for the black pixel at the end, -3 because the program outputs n+3 pixels.
    dest->data[0] = COMPOSABLE_RAW_RUN | (width + 1 - 3 << 16);
    // After user pixels, 1 black pixel then discard remaining FIFO data
    dest->data[width / 2 + 2] = 0x0000u | (COMPOSABLE_EOL_ALIGN << 16);
    dest->data_used = width / 2 + 2;
    assert(dest->data_used <= dest->data_max);
    return (uint16_t *) &dest->data[1];
}

static inline void raw_scanline_finish(struct scanvideo_scanline_buffer *dest) {
    // Need to pivot the first pixel with the count so that PIO can keep up
    // with its 1 pixel per 2 clocks
    uint32_t first = dest->data[0];
    uint32_t second = dest->data[1];
    dest->data[0] = (first & 0x0000ffffu) | ((second & 0x0000ffffu) << 16);
    dest->data[1] = (second & 0xffff0000u) | ((first & 0xffff0000u) >> 16);
    dest->status = SCANLINE_OK;
}

typedef struct sprite {
    int16_t x;
    int16_t y;
    const void *img;
    uint8_t log_size; // always square
} sprite_t;

typedef struct {
    int16_t tex_offs_x;
    int16_t tex_offs_y;
    int16_t size_x;
    bool span_discontinuous;
} intersect_t;

// Always-inline else the compiler does rash things like passing structs in memory
static inline intersect_t get_sprite_intersect(const sprite_t *sp, uint16_t raster_y, uint raster_w)
{
    int size = 1u << sp->log_size;
    intersect_t isct = {0};

    // Calculate offset to the correct row. Early out if it's not within the bounds of the sprite
    isct.tex_offs_y = ((int16_t)raster_y) - sp->y;
    if (isct.tex_offs_y < 0 || isct.tex_offs_y >= size)
        return isct;

    // Calculate offset to correct column
    isct.tex_offs_x = 0;
    isct.size_x = size;

    // Check if it's off the left of the screen. Adjust offset and width to compensate.
    if (sp->x < 0)
    {
        isct.tex_offs_x -= sp->x;
        isct.size_x += sp->x;
    }

    // Check if it's off the right of the screen. Adjust size to compensate
    int16_t overhang = (sp->x + size) - raster_w;
    if (overhang > 0)
    {
        isct.size_x -= overhang;
    }

    //int16_t x_start_clipped = MAX(0, sp->x);
    //isct.tex_offs_x = x_start_clipped - sp->x;
    //isct.size_x = MIN(sp->x + size, (int16_t)raster_w) - x_start_clipped;
    return isct;
}

// Sprites may have an array of metadata on the end. One word per line, encodes first opaque pixel, last opaque pixel, and whether the span in between is solid. This allows fewer 
static inline intersect_t intersect_with_metadata(intersect_t isct, uint32_t meta) {
    int span_end = meta & 0xffff;
    int span_start = (meta >> 16) & 0x7fff;
    int isct_new_start = MAX(isct.tex_offs_x, span_start);
    int isct_new_end = MIN(isct.tex_offs_x + isct.size_x, span_end);
    isct.tex_offs_x = isct_new_start;
    isct.size_x = isct_new_end - isct_new_start;
    return isct;
}

static inline intersect_t calculate_sprite_span(const sprite_t *sprite, uint raster_y, uint raster_width, int pixel_bytes) {
    uint16_t size = 1u << sprite->log_size;

    // Get the simple intersection. If it's zero width early out right away.
    intersect_t isct = get_sprite_intersect(sprite, raster_y, raster_width);
    if (isct.size_x <= 0)
        return isct;

    // If the sprite has opacity metadata we may be able to narrow down the intersection a bit
    const uint8_t *img = sprite->img;

    // Metadata is one word per row, concatenated to end of pixel data
    uint32_t meta = ((uint32_t *)(sprite->img + size * size * pixel_bytes))[isct.tex_offs_y];
    isct = intersect_with_metadata(isct, meta);
    isct.span_discontinuous = !!!(meta & (1u << 31));

    return isct;
}

// ----------------------------------------------------------------------------
// Functions from sprite.S

// Constant-colour span
void sprite_fill8(uint8_t *dst, uint8_t colour, uint len);
void sprite_fill16(uint16_t *dst, uint16_t colour, uint len);

// Block image transfers
void sprite_blit8(uint8_t *dst, const uint8_t *src, uint len);
void sprite_blit8_alpha(uint8_t *dst, const uint8_t *src, uint len);
void sprite_blit16(uint16_t *dst, const uint16_t *src, uint len);
void sprite_blit16_alpha(uint16_t *dst, const uint16_t *src, uint len);

// These are just inner loops, and require INTERP0 to be configured before calling:
void sprite_ablit8_loop(uint8_t *dst, uint len);
void sprite_ablit8_alpha_loop(uint8_t *dst, uint len);
void sprite_ablit16_loop(uint16_t *dst, uint len);
void sprite_ablit16_alpha_loop(uint16_t *dst, uint len);

// ----------------------------------------------------------------------------
// Functions from sprite.c

// Render the intersection of a sprite with the current scanline:
void sprite_sprite8(uint8_t *scanbuf, const sprite_t *sp, uint raster_y, uint raster_w);
void sprite_sprite16(uint16_t *scanbuf, const sprite_t *sp, uint raster_y, uint raster_w);

// As above, but apply an affine transform on sprite texture lookups (SLOW, even with interpolator)
void sprite_asprite8(uint8_t *scanbuf, const sprite_t *sp, const affine_transform_t atrans, uint raster_y, uint raster_w);
void sprite_asprite16(uint16_t *scanbuf, const sprite_t *sp, const affine_transform_t atrans, uint raster_y, uint raster_w);

#endif
