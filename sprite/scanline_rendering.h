#include "pico/scanvideo.h"
#include "pico/scanvideo/composable_scanline.h"
#include "pico/multicore.h"
#include "pico/sync.h"
#include "hardware/dma.h"

#ifndef VGA_MODE
    #error "Must define VGA_MODE"
#endif

static uint16_t frame_number;
static uint16_t scanline_counter = 0;
static mutex_t scanline_countdown_lock;
static uint32_t core0_scanlines = 0;
static uint32_t core1_scanlines = 0;

void frame_update_logic(uint32_t frame_number);
void async_update_logic();
void render_scanline(struct scanvideo_scanline_buffer *dest, int dma_channel);

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

void __time_critical_func(render_loop)() {
    int core_num = get_core_num();
    int dma_channel = dma_claim_unused_channel(true);

    while (true) {

        // Count down the scanline number for this frame. Once there are no scanlines remaining trigger the
        // game update on core 0
        mutex_enter_blocking(&scanline_countdown_lock);
        if (scanline_counter == VGA_MODE.height) {
            if (core_num == 0) {
                // Release the lock so that the other core can check the countdown
                mutex_exit(&scanline_countdown_lock);
                
                // Wait for a message from the other core telling us that it has finished
                // rendering the last frame.
                multicore_fifo_pop_blocking();

                // Now do the update
                frame_update_logic(frame_number);

                // Update rendering state
                frame_number++;
                scanline_counter = 0;
                core0_scanlines = 0;
                core1_scanlines = 0;
                __dmb();

                // Send a message to the other core telling it that game logic is complete
                multicore_fifo_push_blocking(0);

            } else {
                mutex_exit(&scanline_countdown_lock);
                
                // Send a message to the other core telling it that we're ready for the game update
                // to proceed
                multicore_fifo_push_blocking(0);

                // Run some game logic while the other core is doing the update
                async_update_logic();

                // Wait for a message from the other core telling us that game logic is complete
                multicore_fifo_pop_blocking();
            }
        } else {
            scanline_counter++;
            if (core_num == 0) {
                core0_scanlines++;
            } else {
                core1_scanlines++;
            }
            mutex_exit(&scanline_countdown_lock);
        }

        scanvideo_scanline_buffer_t *scanline_buffer = scanvideo_begin_scanline_generation(true);
        render_scanline(scanline_buffer, dma_channel);
        scanvideo_end_scanline_generation(scanline_buffer);
    }

    dma_channel_unclaim(dma_channel);
}

static struct semaphore video_setup_complete;