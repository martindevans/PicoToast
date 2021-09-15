#ifndef _SPRITE_H_DMA
#define _SPRITE_H_DMA

#include "pico/types.h"
#include "hardware/dma.h"

#include "sprite.h"

typedef struct rect_fill {
    int16_t x;
    int16_t y;
    uint16_t width;
    uint16_t height;
    
    uint16_t outline_colour;
    uint16_t fill_colour;
} rect_fill_t;

static int cmp_rect_fill_x(const void *a, const void *b) {
    rect_fill_t *aa = (rect_fill_t*)a;
    rect_fill_t *bb = (rect_fill_t*)b;
    return aa->x - bb->x;
}

typedef struct font_map {
    uint16_t min_char;
    uint16_t max_char;
    char invalid_char;

    uint8_t char_height;
    uint8_t space_width;

    const uint16_t* font_pixels;
    const uint16_t* font_metadata;
} font_map_t;

void sprite_rfill16_dma(uint16_t *dst, const rect_fill_t *fill, uint16_t raster_y, uint16_t raster_width, int dma_channel);

// Fill in a span of the buffer with a single colour.
// - dst: write destination
// - colour: colour to write into dst
// - offset: start offset in dst
// - len: number of pixels of dst to write
// - dma_channel: A channel to use for DMA
void sprite_fill16_dma(uint16_t *dst, uint16_t colour, uint offset, uint len, int dma_channel);

// Copy a sprite into the buffer
// - dst: write destination
// - sprite: sprite to copy into dst
// - raster_y: y position of the scanline
// - raster_width: total size of the `dst` buffer
// - dma_channel: A channel to use for DMA
void sprite_sprite16_dma(uint16_t *dst, const sprite_t *sprite, uint16_t raster_y, uint16_t raster_width, int dma_channel);

// Copy multiple sprites using multiple simultaneous DMA transfers
// - dst: write destination
// - sprites: sprites to copy into dst
// - sprite_count: How many sprites to copy from `sprites`
// - raster_y: y position of the scanline
// - raster_width: total size of the `dst` buffer
// - dma_channels: buffer of DMA channels available to use
// - dma_channel_count: number of DMA channels in the buffer
void sprite_sprite16_dma_multiple(uint16_t *dst, const sprite_t *sprites, size_t sprite_count, uint16_t raster_y, uint16_t raster_width, int *dma_channels, size_t dma_channel_count);

// copy from src to dest using a DMA channel
// - dst: write to
// - src: read from
// - len: number of 16 bit pixels to copy
// - dma_channel: A channel to use for DMA
void sprite_blit16_dma(uint16_t *dst, const uint16_t *src, uint len, int dma_channel);

void sprite_string_dma(uint16_t *dst, int16_t x, int16_t y, char *chars, uint16_t chars_len, font_map_t *font, uint16_t raster_y, uint16_t raster_width, int dma_channel);

#endif