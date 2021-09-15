#ifdef SCANLINE_RENDER_MONO

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
static uint16_t max_scanlines = 0;
static mutex_t frame_number_lock;

static void core1_func() {
    sem_acquire_blocking(&video_setup_complete);

    uint32_t last_frame_num = 10000000;
    while (true)
    {
        mutex_enter_blocking(&frame_number_lock);
        uint32_t fn = frame_number;
        mutex_exit(&frame_number_lock);

        if (fn != last_frame_num)
        {
            last_frame_num = fn;
            async_update_logic(fn);
        }
    }
}

void init_scanline_rendering(const scanvideo_mode_t *mode)
{
    hard_assert(mode->width + 4 <= PICO_SCANVIDEO_MAX_SCANLINE_BUFFER_WORDS * 2);

    max_scanlines = mode->height;
    
    mutex_init(&frame_number_lock);
    sem_init(&video_setup_complete, 0, 1);
    multicore_launch_core1(core1_func);
    scanvideo_setup(mode);
    scanvideo_timing_enable(true);
    sem_release(&video_setup_complete);
}

void __time_critical_func(render_loop)() {
    int core_num = get_core_num();

    int dma_channels[SCANLINE_RENDER_PARALLEL_DMA];
    for (size_t i = 0; i < SCANLINE_RENDER_PARALLEL_DMA; i++)
        dma_channels[i] = dma_claim_unused_channel(true);

    while (true)
    {
        // Count down the scanline number for this frame. Once there are no scanlines remaining trigger the
        // game update on core 0
        if (scanline_counter == max_scanlines)
        {
            // Now do the update
            frame_update_logic(frame_number);

            // Update rendering state
            mutex_enter_blocking(&frame_number_lock);
            frame_number++;
            mutex_exit(&frame_number_lock);
            scanline_counter = 0;
        }
        else
        {
            scanline_counter++;
        }

        scanvideo_scanline_buffer_t *scanline_buffer = scanvideo_begin_scanline_generation(true);
        render_scanline(scanline_buffer, dma_channels, SCANLINE_RENDER_PARALLEL_DMA);
        scanvideo_end_scanline_generation(scanline_buffer);
    }

    // Unclaim all those DMA channels again
    for (size_t i = 0; i < SCANLINE_RENDER_PARALLEL_DMA; i++)
        dma_channel_unclaim(true);
}

#endif