/*
 * Copyright (c) 2020 Raspmelon Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "pico.h"
#include "pico/error.h"
#include "hardware/uart.h"
#include "hardware/gpio.h"
#include "pico/scanvideo.h"
#include "pico/scanvideo/composable_scanline.h"
#include "pico/multicore.h"
#include "pico/sync.h"
#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "hardware/watchdog.h"
#include "hardware/vreg.h"
#include "hardware/interp.h"
#include "hardware/dma.h"
#include "hardware/irq.h"

#include "sprite.h"
#include "sprite_dma.h"
#include "physics/aabb.h"

#include "levels/level1.h"
#include "levels/level2.h"
#include "levels/level3.h"
#include "levels/level4.h"
#include "levels/level5.h"
#include "levels/level6.h"
#include "levels/level7.h"
#include "levels/level8.h"
#include "levels/level9.h"
#include "levels/level10.h"

#include "content/melon.h"
#include "content/ninja_left.h"
#include "content/ninja_right.h"
#include "content/heroic_toast_left.h"
#include "content/heroic_toast_right.h"
#include "content/SaikyoBlack.h"

#include "hardware/structs/vreg_and_chip_reset.h"

#define VGA_MODE vga_mode_640x480_60
#define N_BERRIES 50

#define DIMDIV 22
#define GRAVITY 1
#define RESTITUTION 0.4
#define FRICTION 0.1
#define GRIP_FRICTION 0.15
#define PLAYER_INPUT_SIDEWAYS 4
#define PLAYER_INPUT_SIDEWAYS_AIR 1
#define PLAYER_INPUT_JUMP 60
#define WALL_JUMP_KICK_X 20
#define WALL_JUMP_KICK_Y 40
#define MIN_BOUNCE_YVEL 2

font_map_t SaikyoBlack = {
    .min_char = 33,
    .max_char = 96,
    .invalid_char = '?',
    .char_height = 9,
    .space_width = 8,
    .font_pixels = &font_SaikyoBlack_pixels[0],
    .font_metadata = &font_SaikyoBlack_metadata[0]
};

level_t *levels[] = {
    &level1,
    &level2,
    &level3,
    &level4,
    &level5,
    &level6,
    &level7,
    &level8,
    &level9,
    &level10,
};
level_t *level = &level10;

float ninja_xvel = 0;
float ninja_yvel = 0;
float ninja_xpos = 0;
float ninja_ypos = 0;
sprite_t ninja;

sprite_t *melons = NULL;
sprite_t *enemies = NULL;
rect_fill_t *walls = NULL;

#define DEBUG_STR_MAX_LEN 50
int debug_str_length = 0;
char debug_str[DEBUG_STR_MAX_LEN];

uint16_t frame_number;
uint16_t scanline_counter = 0;
mutex_t scanline_countdown_lock;
uint32_t core0_scanlines = 0;
uint32_t core1_scanlines = 0;

static const uint VSYNC_PIN = PICO_SCANVIDEO_COLOR_PIN_BASE + PICO_SCANVIDEO_COLOR_PIN_COUNT + 1;
static const uint button_pins[] = {0, 6, 11};
static uint32_t button_state = 0;

static void frame_update_logic();
static void render_scanline(struct scanvideo_scanline_buffer *dest, int dma_channel);

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
                
                // Send a message to the other core telling it that we're waiting.
                multicore_fifo_push_blocking(0);

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

struct semaphore video_setup_complete;

void core1_func() {
    sem_acquire_blocking(&video_setup_complete);
    render_loop();
}

void __time_critical_func(render_scanline)(struct scanvideo_scanline_buffer *dest, int channel) {
    uint16_t l = scanvideo_scanline_number(dest->scanline_id);
    uint16_t *colour_buf = raw_scanline_prepare(dest, VGA_MODE.width);

    // Begin DMA fill of the background.
    const uint16_t bgcol = PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x40, 0xc0, 0xff);
    sprite_fill16_dma(colour_buf, bgcol, 0, VGA_MODE.width, channel);

    // Draw all the walls
    for (size_t i = 0; i < level->numboxes; i++) {
        sprite_rfill16_dma(colour_buf, &walls[i], l, VGA_MODE.width, channel);
    }

    // Draw the melons
    for (size_t i = 0; i < level->nummelons; i++) {
        if (melons[i].y < VGA_MODE.height) {
            sprite_sprite16_dma(colour_buf, &melons[i], l, VGA_MODE.width, channel);
        }
    }

    // Draw the enemies
    for (size_t i = 0; i < level->numboxes; i++) {
        sprite_sprite16_dma(colour_buf, &enemies[i], l, VGA_MODE.width, channel);
    }

    // Draw the character
    sprite_sprite16_dma(colour_buf, &ninja, l, VGA_MODE.width, channel);

    // Draw debug string
    if (debug_str_length > 0) {
        sprite_string_dma(colour_buf, 10, 10, &debug_str[0], debug_str_length, &SaikyoBlack, l, VGA_MODE.width, channel);
    }

    // Wait for all DMA job to complete
    dma_channel_wait_for_finish_blocking(channel);

    raw_scanline_finish(dest);
}

int cmp_sprite_x(const void *a, const void *b) {
    sprite_t *aa = (sprite_t*)a;
    sprite_t *bb = (sprite_t*)b;
    return aa->x - bb->x;
}

int cmp_fill_x(const void *a, const void *b) {
    rect_fill_t *aa = (rect_fill_t*)a;
    rect_fill_t *bb = (rect_fill_t*)b;
    return aa->x - bb->x;
}

void set_level(level_t *lvl) {
    level = lvl;

    ninja_xpos = level->spawnX;
    ninja_ypos = level->spawnY;
    ninja.x = (uint16_t)ninja_xpos;
    ninja.y = (uint16_t)ninja_ypos;
    ninja_xvel = 0;
    ninja_yvel = 0;

    if (melons != NULL)
        free(melons);
    melons = malloc(lvl->nummelons * sizeof(sprite_t));
    for (size_t i = 0; i < lvl->nummelons; i++) {
        sprite_t *m = &melons[i];
        m->x = lvl->melons[i].x / DIMDIV - 8;
        m->y = lvl->melons[i].y / DIMDIV - 8;
        m->log_size = 4;
        m->img = &melon_16x16[0];
    }

    // Sort melons into X order to make collision checks faster (once we find a melon to the
    // right of the player we can stop looking)
    qsort(melons, lvl->nummelons, sizeof(sprite_t), cmp_sprite_x);

    if (walls != NULL)
        free(walls);
    walls = malloc(lvl->numboxes * sizeof(rect_fill_t));
    for (size_t i = 0; i < lvl->numboxes; i++) {
        rect_fill_t *w = &walls[i];
        w->fill_colour = lvl->boxes[i].exit_zone ? PICO_SCANVIDEO_PIXEL_FROM_RGB8(255, 0, 0) : PICO_SCANVIDEO_PIXEL_FROM_RGB8(0, 0, 0);
        w->outline_colour = PICO_SCANVIDEO_PIXEL_FROM_RGB8(0, 0, 0);
        w->x = lvl->boxes[i].x / DIMDIV;
        w->y = lvl->boxes[i].y / DIMDIV;
        w->width = lvl->boxes[i].w / DIMDIV;
        w->height = lvl->boxes[i].h / DIMDIV;
    }

    if (enemies != NULL)
        free(enemies);
    enemies = malloc(lvl->numboxes * sizeof(sprite_t));
    for (size_t i = 0; i < lvl->numboxes; i++) {
        sprite_t *e = &enemies[i];
        box_t *b = &lvl->boxes[i];

        e->x = walls[i].x + walls[i].width / 2 - 8;
        e->y = walls[i].y - 32;
        e->log_size = 5;
        e->img = &heroic_toast_left_32x32[0];

        if (!lvl->boxes[i].has_enemy) {
            e->y = 10000;
        }
    }

}

inline bool intersects(int16_t x1, int16_t y1, int16_t w1, int16_t h1, int16_t x2, int16_t y2, int16_t w2, int16_t h2) {
    return x1 <= (x2 + w2) && (x1 + w1) >= x2 && y1 <= (y2 + h2) && (y1 + h1) >= y2;
}

void __time_critical_func(frame_update_logic)(uint32_t frame_number) {

    // Move all the enemies every other frame (allows them to move 0.5 units/frame)
    if (frame_number & 1) {
        for (size_t i = 0; i < level->numboxes; i++) {
            sprite_t *e = &enemies[i];
            rect_fill_t *w = &walls[i];

            int16_t dir = e->img == &heroic_toast_left_32x32[0] ? -1 : 1;
            e->x += dir;

            if (dir < 0) {
                if (e->x <= w->x) {
                    e->img = &heroic_toast_right_32x32[0];
                    e->x = w->x;
                }
            } else {
                if (e->x + 32 >= w->x + w->width) {
                    e->img = &heroic_toast_left_32x32[0];
                    e->x = w->x + w->width - 32;
                }
            }
        }
    }

    // Check if the player intersects any enemies
    bool dead = false;
    for (size_t i = 0; i < level->numboxes; i++) {
        box_t *b = &level->boxes[i];
        if (!b->has_enemy)
            continue;

        sprite_t *e = &enemies[i];
        if (intersects(e->x + 5, e->y + 10, 32-10, 32-10, ninja.x, ninja.y, 32, 32)) {
            dead = true;
            break;
        }
    }

    // Check if the player is off bottom of screen, if so move back to the spawn
    if (dead || ninja.y > VGA_MODE.height) {
        ninja_xpos = level->spawnX;
        ninja_ypos = level->spawnY;
        ninja_xvel = 0;
        ninja_yvel = 0;
        dead = false;
    }

    // Check if the player intersects any melons
    int16_t ninja_right = ninja.x + 32;
    int16_t ninja_bottom = ninja.y + 32;
    for (size_t i = 0; i < level->nummelons; i++) {
        // Melons are sorted left to right, so once a melon has been located to the right
        // of the ninja we can early exit the loop
        if (melons[i].x > ninja_right)
            break;

        // Melons off the bottom of the screen are irrelevant
        if (melons[i].y > VGA_MODE.height)
            continue;

        // Check if the hitboxes intersect
        if (intersects(melons[i].x, melons[i].y, 16, 16, ninja.x, ninja.y, 32, 32)) {
            melons[i].y = VGA_MODE.height + 1;
        }
    }

    // Read serial input to switch levels
    int c = PICO_ERROR_TIMEOUT;
    do
    {
        c = getchar_timeout_us(0);

        if (c >= '0' && c <= '9') {
            set_level(levels[c - 48]);
            return;
        }
    } while (c != PICO_ERROR_TIMEOUT);

    // Read player input from hardware buttons
    bool in_jump = (button_state & 2) != 0;
    bool in_left = (button_state & 4) != 0;
    bool in_right = (button_state & 1) != 0;

    // Check if the player intersects any walls
    bool touching_floor = false;
    bool touching_left_wall = false;
    bool touching_right_wall = false;
    bool touching_roof = false;
    int32_t ninja_top = ninja.y;
    int32_t ninja_left = ninja.x;
    bool touching_any = false;
    for (size_t i = 0; i < level->numboxes; i++) {
        rect_fill_t *w = &walls[i];

        //if (!intersects(w->x, w->y, w->width, w->height, ninja.x, ninja.y, 32, 32))
        //    continue;
        //touching_any = true;

        // Swept collision detection between wall and player
        hit_t hit;
        float hx = w->width / (float)2;
        float hy = w->height / (float)2;
        AABB_t wall_aabb = {
            .pos = {
                .x = w->x + hx,
                .y = w->y + hy
            },
            .half = {
                .x = hx,
                .y = hy,
            }
        };
        AABB_t player = {
            .pos = {
                .x = ninja.x + 16,
                .y = ninja.y + 16
            },
            .half = {
                .x = 16,
                .y = 16
            }
        };
        if (!intersect_AABB_AABB(&hit, &wall_aabb, &player)) {
            continue;
        }

        touching_any = true;

        // Teleport player out of collision
        ninja_xpos += hit.delta.x * DIMDIV;
        ninja_ypos += hit.delta.y * DIMDIV;

        // Determine which surface was hit and bounce accordingly
        if (abs(hit.normal.x) > 0) {
            if (hit.normal.x < 0) {
                touching_right_wall = true;
            } else {
                touching_left_wall = true;
            }
        } else {
            if (hit.normal.y < 0) {
                touching_floor = true;
            } else {
                touching_roof = true;
            }
        }
    }

    // Bounce
    if (touching_floor) {
        ninja_yvel = -ninja_yvel * RESTITUTION;
        if (fabsf(ninja_yvel) < MIN_BOUNCE_YVEL)
            ninja_yvel = 0;
    }
    if (touching_roof) {
        ninja_yvel = 0;
    }
    if (touching_left_wall || touching_right_wall) {
        ninja_xvel = -ninja_xvel * RESTITUTION;
    }

    // Wall grip
    bool wall_gripping = false;
    if (touching_left_wall && in_left) {
        ninja_xvel = 0;
        ninja_yvel *= (1 - GRIP_FRICTION);
        wall_gripping = true;
    }
    if (touching_right_wall && in_right) {
        ninja_xvel = 0;
        ninja_yvel *= (1 - GRIP_FRICTION);
        wall_gripping = true;
    }

    // Jump
    if (touching_floor && in_jump)
        ninja_yvel -= PLAYER_INPUT_JUMP;
    if (touching_left_wall && in_jump) {
        ninja_xvel += WALL_JUMP_KICK_X;
        ninja_yvel -= WALL_JUMP_KICK_Y;
    }
    if (touching_right_wall && in_jump) {
        ninja_xvel -= WALL_JUMP_KICK_X;
        ninja_yvel -= WALL_JUMP_KICK_Y;
    }

    // Friction & sideways movement
    if (touching_floor) {
        ninja_xvel *= 1 - FRICTION;
        if (in_left && !touching_left_wall)
            ninja_xvel -= PLAYER_INPUT_SIDEWAYS;
        if (in_right && !touching_right_wall)
            ninja_xvel += PLAYER_INPUT_SIDEWAYS;
    } else {
        if (in_left && !touching_left_wall)
            ninja_xvel -= PLAYER_INPUT_SIDEWAYS_AIR;
        if (in_right && !touching_right_wall)
            ninja_xvel += PLAYER_INPUT_SIDEWAYS_AIR;
    }

    // Gravity
    if (!touching_floor) {
        if (wall_gripping) {
            ninja_yvel += GRAVITY / (float)2;
        } else {
            ninja_yvel += GRAVITY;
        }
    }

    // Integrate velocity
    ninja_xpos += ninja_xvel;
    ninja_ypos += ninja_yvel;

    // Update sprite graphic
    if (fabsf(ninja_xvel) > 0) {
        if (ninja_xvel < 0) {
            ninja.img = ninja_left_32x32;
        } else {
            ninja.img = ninja_right_32x32;
        }
    }

    // update sprite position
    ninja.x = (int16_t)(ninja_xpos / DIMDIV);
    ninja.y = (int16_t)(ninja_ypos / DIMDIV);

    float render_load = (((float)MIN(core0_scanlines, core1_scanlines)) / ((float)MAX(core0_scanlines, core1_scanlines))) * 100;
    debug_str_length = snprintf(debug_str, DEBUG_STR_MAX_LEN, "LOAD:%u%% L:%u R:%u T:%u B:%u", (uint32_t)render_load, touching_left_wall, touching_right_wall, touching_roof, touching_floor);
}

void __time_critical_func(vga_board_button_irq_handler)() {
    int vsync_current_level = gpio_get(VSYNC_PIN);
    gpio_acknowledge_irq(VSYNC_PIN, vsync_current_level ? GPIO_IRQ_EDGE_RISE : GPIO_IRQ_EDGE_FALL);

    // Note v_sync_polarity == 1 means active-low because anything else would be confusing
    if (vsync_current_level != VGA_MODE.default_timing->v_sync_polarity) {
        for (int i = 0; i < count_of(button_pins); ++i) {
            gpio_pull_down(button_pins[i]);
            gpio_set_oeover(button_pins[i], GPIO_OVERRIDE_LOW);
        }
    } else {
        uint32_t state = 0;
        for (int i = 0; i < count_of(button_pins); ++i) {
            state |= gpio_get(button_pins[i]) << i;
            gpio_set_oeover(button_pins[i], GPIO_OVERRIDE_NORMAL);
        }
        button_state = state;
    }
}

void vga_board_init_buttons() {
    gpio_set_irq_enabled(VSYNC_PIN, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true);
    irq_set_exclusive_handler(IO_IRQ_BANK0, vga_board_button_irq_handler);
    irq_set_enabled(IO_IRQ_BANK0, true);
}

int main(void) {

    // Overclock
    vreg_set_voltage(VREG_VOLTAGE_1_20);
    set_sys_clock_khz(250000, true);

    // Setup serial communication and buttons
    stdio_init_all();
    vga_board_init_buttons();

    // Load a level
    ninja.x = 0;
    ninja.y = 0;
    ninja.img = ninja_left_32x32;
    ninja.log_size = 5;
    set_level(levels[0]);

    // Initialise video system
    sem_init(&video_setup_complete, 0, 1);
    mutex_init(&scanline_countdown_lock);
    multicore_launch_core1(core1_func);
    hard_assert(VGA_MODE.width + 4 <= PICO_SCANVIDEO_MAX_SCANLINE_BUFFER_WORDS * 2);
    scanvideo_setup(&VGA_MODE);
    scanvideo_timing_enable(true);
    sem_release(&video_setup_complete);

    // Enter the infinite video loop
    render_loop();
}

