#ifndef SCANLINE_RENDER_MONO

#include "pico.h"
#include "pico/scanvideo.h"
#include "pico/scanvideo/composable_scanline.h"
#include "pico/multicore.h"
#include "pico/sync.h"
#include "hardware/dma.h"

#include "scanline_rendering.h"

static struct semaphore video_setup_complete;

static uint32_t frame_number;
static uint16_t scanline_counter = 0;
static mutex_t scanline_countdown_lock;
static uint32_t core0_scanlines = 0;
static uint32_t core1_scanlines = 0;
static uint16_t max_scanlines = 0;

static void core1_func() {
    sem_acquire_blocking(&video_setup_complete);
    render_loop();
}

void init_scanline_rendering(const scanvideo_mode_t *mode) {
    max_scanlines = mode->height;
    sem_init(&video_setup_complete, 0, 1);
    mutex_init(&scanline_countdown_lock);
    multicore_launch_core1(core1_func);
    hard_assert(mode->width + 4 <= PICO_SCANVIDEO_MAX_SCANLINE_BUFFER_WORDS * 2);
    scanvideo_setup(mode);
    scanvideo_timing_enable(true);
    sem_release(&video_setup_complete);
}

enum {
    MSGC0_GAME_LOGIC_DONE = 1,
} msg_from_core0;

enum {
    MSGC1_RENDERING_DONE = 1,
} msg_from_core1;

void __time_critical_func(render_loop)() {
    int core_num = get_core_num();
    int dma_channel = dma_claim_unused_channel(true);

    while (true)
    {
        // Count down the scanline number for this frame. Once there are no scanlines remaining trigger the
        // game update on core 0
        mutex_enter_blocking(&scanline_countdown_lock);
        if (scanline_counter == max_scanlines)
        {
            if (core_num == 0)
            {
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
                multicore_fifo_push_blocking(MSGC0_GAME_LOGIC_DONE);
            }
            else
            {
                mutex_exit(&scanline_countdown_lock);
                
                uint32_t saved_frame_number = frame_number;

                // Send a message to the other core telling it that we're ready for the game update
                // to proceed
                multicore_fifo_push_blocking(MSGC1_RENDERING_DONE);

                // Run some game logic while the other core is doing the update
                async_update_logic(saved_frame_number);
                __dmb();

                // Wait for a message from the other core telling us that game logic is complete
                multicore_fifo_pop_blocking();
            }
        }
        else
        {
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

#endif