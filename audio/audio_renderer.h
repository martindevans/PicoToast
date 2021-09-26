#ifndef _AUDIOSYS_RENDERER_H
#define _AUDIOSYS_RENDERER_H

#include "pico/audio_i2s.h"

#include "hardware/dma.h"
#include "hardware/claim.h"

#ifndef AUDIOSYS_SAMPLE_RATE
    #define AUDIOSYS_SAMPLE_RATE 24000
#endif

#ifndef AUDIOSYS_SAMPLES_PER_BUFFER
    #define AUDIOSYS_SAMPLES_PER_BUFFER 200
#endif

#ifndef AUDIOSYS_NUM_BUFFERS
    #define AUDIOSYS_NUM_BUFFERS 5
#endif

// Initialise the audio system and return a producer queue
// May return NULL if audio initialisation fails!
audio_buffer_pool_t *init_audio()
{
    static audio_format_t audio_format = {
        .sample_freq = AUDIOSYS_SAMPLE_RATE,
        .format = AUDIO_BUFFER_FORMAT_PCM_U8,
        .channel_count = 1,
    };

    static audio_buffer_format_t producer_format = {
        .format = &audio_format,
        .sample_stride = 1
    };

    audio_buffer_pool_t *producer_pool = audio_new_producer_pool(&producer_format, AUDIOSYS_NUM_BUFFERS, AUDIOSYS_SAMPLES_PER_BUFFER);

    audio_i2s_config_t config = {
        .data_pin = PICO_AUDIO_I2S_DATA_PIN,
        .clock_pin_base = PICO_AUDIO_I2S_CLOCK_PIN_BASE,
        .dma_channel = dma_claim_unused_channel(true),
        .pio_sm = 0,
    };

    const audio_format_t *output_format = audio_i2s_setup(&audio_format, &config);
    if (!output_format) {
        return NULL;
    }

    bool ok = audio_i2s_connect(producer_pool);
    if (!ok) {
        return NULL;
    }

    audio_i2s_set_enabled(true);

    return producer_pool;
}

#endif