#include <alloca.h>

#include "sprite.h"
#include "sprite_dma.h"
#include "hardware/dma.h"

#define __ram_func(foo) __not_in_flash_func(foo)

void __ram_func(sprite_rfill16_dma)(uint16_t *dst, const rect_fill_t *fill, uint16_t raster_y, uint16_t raster_width, int dma_channel)
{
    // Sanity check fill size
    if (fill->width < 2)
        return;

    // Check if the scanline intersects the fill zone (up/down)
    int16_t t = MAX(0, fill->y);
    int16_t b = fill->y + fill->height;
    if (t == b || raster_y < t || raster_y > b)
        return;

    // Check if the zone is on screen (left/right)
    int16_t l = MAX(0, fill->x);
    int16_t r = MIN(raster_width - 1, fill->x + fill->width);
    if (l >= r || r < 0)
        return;

    // Wait for previous DMA job to finish
    dma_channel_wait_for_finish_blocking(dma_channel);

    // If this is the top or bottom line, fill entirely with the outline colour
    // Otherwise draw two pixels of outline (left and right sides) and fill the middle
    if (raster_y == t || raster_y == b)
    {
        sprite_fill16_dma(dst, fill->outline_colour, l, r - l + 1, dma_channel);
    }
    else
    {
        dst[l] = fill->outline_colour;
        l++;
        dst[r] = fill->outline_colour;
        r--;
        if (l < r)
            sprite_fill16_dma(dst, fill->fill_colour, l, r - l + 1, dma_channel);
    }
}

void __ram_func(sprite_blit16_dma)(uint16_t *dst, const uint16_t *src, uint len, int dma_channel)
{
    dma_channel_wait_for_finish_blocking(dma_channel);

    if (len == 0)
        return;

    // Configure DMA to copy across one pixel at a time
    dma_channel_config c = dma_channel_get_default_config(dma_channel);
    channel_config_set_transfer_data_size(&c, DMA_SIZE_16);
    channel_config_set_read_increment(&c, true);
    channel_config_set_write_increment(&c, true);
    dma_channel_configure(
        dma_channel,            // Channel to be configured
        &c,                     // The configuration we just created
        dst,                    // The initial write address
        src,                    // The read address
        len,                    // Number of transfers
        true                    // Start immediately
    );
}

void __ram_func(sprite_fill16_dma)(uint16_t *dst, uint16_t colour, uint offset, uint len, int dma_channel)
{
    dma_channel_wait_for_finish_blocking(dma_channel);

    // Ensure the first 16 bits of the buffer are written to.
    // Early out if this is a tiny transfer
    switch (len) {
        case 0:
            return;
        case 1:
            dst[offset] = colour;
            return;
        default:
            dst[offset] = colour;
            break;
    }

    // Configure DMA to copy 32 bits of data at a time
    dma_channel_config c = dma_channel_get_default_config(dma_channel);
    channel_config_set_transfer_data_size(&c, DMA_SIZE_16);
    channel_config_set_read_increment(&c, false);
    channel_config_set_write_increment(&c, true);
    dma_channel_configure(
        dma_channel,            // Channel to be configured
        &c,                     // The configuration we just created
        &dst[offset + 1],       // The initial write address
        &dst[offset],           // The read address
        len - 1,                // Number of transfers
        true                    // Start immediately
    );
}

void __ram_func(sprite_sprite16_dma)(uint16_t *dst, const sprite_t *sprite, uint16_t raster_y, uint16_t raster_width, int dma_channel)
{
    int width = sprite->data.size_x;
    const uint16_t *img = sprite->data.pixels;

    intersect_t isct = calculate_sprite_span(sprite, raster_y, raster_width, sizeof(uint16_t));
    if (isct.size_x <= 0)
        return;

    int16_t x = sprite->x + isct.tex_offs_x;
    dma_channel_wait_for_finish_blocking(dma_channel);
    
    // If there's alpha in the row fall back to CPU blitting. Otherwise blast the data out with DMA.
    if (!isct.span_discontinuous)
        sprite_blit16_dma(dst + x, img + isct.tex_offs_x + isct.tex_offs_y * width, isct.size_x, dma_channel);
    else
        sprite_blit16_alpha(dst + x, img + isct.tex_offs_x + isct.tex_offs_y * width, isct.size_x);
}

void __ram_func(sprite_string_dma)(uint16_t *dst, int16_t x, int16_t y, char *chars, uint16_t chars_len, font_map_t *font, uint16_t raster_y, uint16_t raster_width, int dma_channel)
{
    // Sanity check font, render nothing for null fonts
    if (font == NULL)
        return;

    // Calculate which row to render, exit out if it's not within the bounds of the font
    int16_t row = raster_y - y;
    if (row < 0 || row >= font->char_height)
        return;

    // Render all of the characters one by one
    for (size_t i = 0; i < chars_len; i++) {

        // Skip spaces
        char c = chars[i];
        if (c == ' ') {
            x += font->space_width;
            continue;
        }

        // Fall back to the default character for invalid characters
        if (c < font->min_char || c > font->max_char)
            c = font->invalid_char;

        // Get the 2 words of metadata for this character
        const uint16_t *meta_ptr = &font->font_metadata[(c - font->min_char) * 2 * font->char_height + row * 2];

        // XXXXXXXXXXXX XXXX
        // ^ 12 bits: pixel offset (location of the first pixel for this row of this character)
        //              ^ 4 bits: alpha prelude length
        uint16_t meta0 = meta_ptr[0];
        uint16_t px_offset = meta0 >> 4;
        uint16_t prelude = meta0 & 15;

        // XXXXXXX X XXXXXXXX
        // ^ 7 bits: Number of pixels to copy
        //         ^ 1 bit: is the pixel span discontinuous
        //           ^ 8 bits: number of pixels to move the cursor over to draw the next character
        uint16_t meta1 = meta_ptr[1];
        int16_t pixel_copy_count = meta1 >> 9;
        bool discontinuous = (meta1 & 256) != 0;
        uint8_t char_width = meta1 & 0xFF;

        // Calculate the pointers to copy to/from
        const uint16_t *char_src = font->font_pixels + px_offset;

        // make sure rendering isn't off the left/right side of the screen.
        // If it is adjust the position/counts to compensate
        if (x < 0) {
            // If the entire character is off screen skip it completely
            if (char_width <= -x) {
                pixel_copy_count = 0;
            } else {
                if (x + prelude > 0) {
                    // Nothing needs to be done! The prelude (x offset to start of copy) already covers the off screen portion
                } else {
                    uint8_t extra = -(x + prelude);
                    prelude += extra;
                    char_src += extra;
                    pixel_copy_count -= extra;
                }
            }
        }

        // Copy across as many pixels as necessary
        if (pixel_copy_count > 0) {
            uint16_t *char_dst = dst + x + prelude;
            if (discontinuous) {
                sprite_blit16_alpha(char_dst, char_src, pixel_copy_count);
            } else {
                sprite_blit16_dma(char_dst, char_src, pixel_copy_count, dma_channel);
            }
        }

        // Offset the next character
        x += char_width;
    }
}