#ifndef _SPRITE2_H
#define _SPRITE2_H

#include "pico/types.h"

#include "pico/scanvideo.h"
#include "pico/scanvideo/composable_scanline.h"

typedef struct {
    const uint16_t *pixels;
    const uint32_t *metadata;
    uint8_t size_x;
    uint8_t size_y;
} image_data_t;

typedef struct {
    int16_t x;
    int16_t y;
    image_data_t data;
} sprite_t;

typedef struct {
    int16_t tex_offs_x;
    int16_t tex_offs_y;
    int16_t size_x;
    bool span_discontinuous;
} intersect_t;

static int cmp_sprite_x(const void *a, const void *b) {
    sprite_t *aa = (sprite_t*)a;
    sprite_t *bb = (sprite_t*)b;
    return aa->x - bb->x;
}

// Always-inline else the compiler does rash things like passing structs in memory
static inline intersect_t get_sprite_intersect(const sprite_t *sp, uint16_t raster_y, uint raster_w)
{
    uint8_t size_x = sp->data.size_x;
    uint8_t size_y = sp->data.size_y;
    intersect_t isct = {0};

    // Calculate offset to the correct row. Early out if it's not within the bounds of the sprite
    isct.tex_offs_y = ((int16_t)raster_y) - sp->y;
    if (isct.tex_offs_y < 0 || isct.tex_offs_y >= size_y)
        return isct;

    // Calculate offset to correct column
    isct.tex_offs_x = 0;
    isct.size_x = size_x;

    // Check if it's off the left of the screen. Adjust offset and width to compensate.
    if (sp->x < 0)
    {
        isct.tex_offs_x -= sp->x;
        isct.size_x += sp->x;
    }

    // Check if it's off the right of the screen. Adjust size to compensate
    int16_t overhang = (sp->x + size_x) - raster_w;
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
    // Get the simple intersection. If it's zero width early out right away.
    intersect_t isct = get_sprite_intersect(sprite, raster_y, raster_width);
    if (isct.size_x <= 0)
        return isct;

    // Use metadata to narrow down span
    const uint32_t meta = sprite->data.metadata[isct.tex_offs_y];
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

#endif