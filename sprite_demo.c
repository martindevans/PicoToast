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

#include "bsp/board.h"
#include "tusb.h"
#include "hid/hid.h"

#define VGA_MODE vga_mode_640x480_60

#include "sprite.h"
#include "sprite_dma.h"
#include "physics/aabb.h"
#include "sprite/scanline_rendering.h"

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
#include "levels/level11.h"
#include "levels/level12.h"
#include "levels/level13.h"
#include "levels/level14.h"
#include "levels/level15.h"
#include "levels/level16.h"
#include "levels/level17.h"
#include "levels/level18.h"
#include "levels/level19.h"
#include "levels/level20.h"
#include "levels/level21.h"
#include "levels/level22.h"
#include "levels/level23.h"
#include "levels/level24.h"
#include "levels/level25.h"
#include "levels/level26.h"
#include "levels/level27.h"
#include "levels/level28.h"
#include "levels/level29.h"
#include "levels/level30.h"

#include "content/melon.h"
#include "content/ninja_left.h"
#include "content/ninja_right.h"
#include "content/uparrow.h"
#include "content/leftarrow.h"
#include "content/rightarrow.h"
#include "content/heroic_toast_left.h"
#include "content/heroic_toast_right.h"
#include "content/SaikyoBlack.h"

#include "hardware/structs/vreg_and_chip_reset.h"

#define DIMDIV 22
#define GRAVITY 1
#define RESTITUTION 0.4
#define FRICTION 0.08
#define GRIP_FRICTION 0.06
#define PLAYER_INPUT_SIDEWAYS 3.5
#define PLAYER_INPUT_SIDEWAYS_AIR 1.1
#define PLAYER_INPUT_JUMP 60
#define WALL_JUMP_KICK_X 20
#define WALL_JUMP_KICK_Y 50
#define MIN_BOUNCE_YVEL 2

#define LED_PIN 25

#define LEVEL_COUNT 26
static level_t *levels[] = {
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
    &level11,
    &level12,
    &level13,
    &level14,
    //&level15,
    &level16,
    &level17,
    &level18,
    //&level19,
    &level20,
    &level21,
    //&level22,
    &level23,
    //&level24,
    &level25,
    &level26,
    &level27,
    &level28,
    &level29,
    &level30,
};
static level_t *level = &level1;
static int32_t current_level_index = 0;
static bool load_pending = false;

static float ninja_xvel = 0;
static float ninja_yvel = 0;
static float ninja_xpos = 0;
static float ninja_ypos = 0;
static sprite_t ninja = {
    .x = 0,
    .y = 0,
    .data = ninja_left_32x32
};

static sprite_t up_arrow = {
    .x = 0,
    .y = 0,
    .data = uparrow_32x12,
};
static sprite_t left_arrow = {
    .x = 0,
    .y = 0,
    .data = leftarrow_12x32,
};
static sprite_t right_arrow = {
    .x = 0,
    .y = 0,
    .data = rightarrow_12x32,
};

static uint32_t melons_remaining;
static sprite_t *melons = NULL;
static sprite_t *enemies = NULL;
static rect_fill_t *walls = NULL;

static uint64_t render_micros = 0;

#define DEBUG_STR_MAX_LEN 150
static int debug_str_length = 0;
static char debug_str[DEBUG_STR_MAX_LEN];

static const uint VSYNC_PIN = PICO_SCANVIDEO_COLOR_PIN_BASE + PICO_SCANVIDEO_COLOR_PIN_COUNT + 1;
static const uint button_pins[] = {0, 6, 11};
static uint32_t button_state = 0;

void __time_critical_func(render_scanline)(struct scanvideo_scanline_buffer *dest, int *dma_channels, size_t dma_channels_count)
{
    uint64_t start_us = time_us_64();

    uint16_t l = scanvideo_scanline_number(dest->scanline_id);
    uint16_t *colour_buf = raw_scanline_prepare(dest, VGA_MODE.width);

    // Begin DMA fill of the background.
    const uint16_t bgcol = PICO_SCANVIDEO_PIXEL_FROM_RGB8(0x40, 0xc0, 0xff);
    sprite_fill16_dma(colour_buf, bgcol, 0, VGA_MODE.width, dma_channels[0]);

    // Draw all the walls
    for (size_t i = 0; i < level->numboxes; i++) {
        sprite_rfill16_dma(colour_buf, &walls[i], l, VGA_MODE.width, dma_channels[0]);
    }

    // Draw the melons, allowing multiple parallel DMA transfers of non-overlapping sprites
    dma_channel_wait_for_finish_blocking(dma_channels[0]);
    sprite_sprite16_dma_multiple(colour_buf, melons, level->nummelons, l, VGA_MODE.width, dma_channels, dma_channels_count);
    wait_for_dmas(dma_channels, dma_channels_count);

    // Draw the enemies, allowing multiple parallel DMA transfers of non-overlapping sprites
    dma_channel_wait_for_finish_blocking(dma_channels[0]);
    sprite_sprite16_dma_multiple(colour_buf, enemies, level->numboxes, l, VGA_MODE.width, dma_channels, dma_channels_count);
    wait_for_dmas(dma_channels, dma_channels_count);

    // Draw off screen indicators
    sprite_sprite16_dma(colour_buf, &up_arrow, l, VGA_MODE.width, dma_channels[0]);
    sprite_sprite16_dma(colour_buf, &left_arrow, l, VGA_MODE.width, dma_channels[0]);
    sprite_sprite16_dma(colour_buf, &right_arrow, l, VGA_MODE.width, dma_channels[0]);

    // Draw the character
    sprite_sprite16_dma(colour_buf, &ninja, l, VGA_MODE.width, dma_channels[0]);

    // Draw debug string
    if (debug_str_length > 0) {
        dma_channel_wait_for_finish_blocking(dma_channels[0]);
        sprite_string_dma(colour_buf, 10, 10, &debug_str[0], debug_str_length, &SaikyoBlack, l, VGA_MODE.width, dma_channels, dma_channels_count);
        wait_for_dmas(dma_channels, dma_channels_count);
    }

    // Wait for all DMA jobs to complete
    wait_for_dmas(dma_channels, dma_channels_count);

    // Update counter of total rendering time
    // with multicore rendering this will cause bad things since both cores will read/write
    #if SCANLINE_RENDER_MONO
        uint64_t elapsed_us = time_us_64() - start_us;
        render_micros += elapsed_us;
    #endif

    raw_scanline_finish(dest);
}

static void set_level(level_t *lvl)
{
    level = lvl;

    ninja_xpos = level->spawnX;
    ninja_ypos = level->spawnY;
    ninja.x = (uint16_t)ninja_xpos;
    ninja.y = (uint16_t)ninja_ypos;
    ninja_xvel = 0;
    ninja_yvel = 0;

    melons_remaining = lvl->nummelons;
    if (melons != NULL)
        free(melons);
    melons = malloc(lvl->nummelons * sizeof(sprite_t));
    for (size_t i = 0; i < lvl->nummelons; i++) {
        sprite_t *m = &melons[i];
        m->x = lvl->melons[i].x / DIMDIV - 8;
        m->y = lvl->melons[i].y / DIMDIV - 8;
        m->data = melon_16x16;
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
        e->y = walls[i].y - 23;
        e->data = heroic_toast_left_23x23;

        if (!lvl->boxes[i].has_enemy) {
            e->y = 10000;
        }
    }
}

static inline bool intersects(int16_t x1, int16_t y1, int16_t w1, int16_t h1, int16_t x2, int16_t y2, int16_t w2, int16_t h2)
{
    return x1 <= (x2 + w2) && (x1 + w1) >= x2 && y1 <= (y2 + h2) && (y1 + h1) >= y2;
}

void __time_critical_func(async_update_logic)(uint32_t frame_number)
{
    tuh_task();
    hid_task();
}

static mutex_t hid_input_lock;
static bool hid_jump_pressed = false;
static bool hid_left_pressed = false;
static bool hid_right_pressed = false;
static bool hid_cheat_level_up = false;
void process_kbd_report(hid_keyboard_report_t const *p_new_report)
{
    mutex_enter_blocking(&hid_input_lock);
    hid_jump_pressed = false;
    hid_left_pressed = false;
    hid_right_pressed = false;

    for (size_t i = 0; i < 6; i++) {
        switch (p_new_report->keycode[i]) {
            case HID_KEY_ARROW_UP:
            case HID_KEY_W:
                hid_jump_pressed = true;
                break;
            case HID_KEY_ARROW_LEFT:
            case HID_KEY_A:
                hid_left_pressed = true;
                break;
            case HID_KEY_ARROW_RIGHT:
            case HID_KEY_D:
                hid_right_pressed = true;
                break;
            case HID_KEY_F11:
                hid_cheat_level_up = true;
                break;
        }
    }
    mutex_exit(&hid_input_lock);
}

void __time_critical_func(frame_update_logic)(uint32_t frame_number)
{
    // Toggle LED
    gpio_put(LED_PIN, (frame_number & 8) == 8);

    // Load new level if necessary
    if (current_level_index >= LEVEL_COUNT) {
        current_level_index = 0;
    }
    if (load_pending) {
        set_level(levels[current_level_index]);
        load_pending = false;
        return;
    }

    // Move all the enemies every other frame (allows them to move 0.5 units/frame)
    if (frame_number & 1) {
        for (size_t i = 0; i < level->numboxes; i++) {
            sprite_t *e = &enemies[i];
            rect_fill_t *w = &walls[i];

            int16_t dir = e->data.pixels == heroic_toast_left_23x23.pixels ? -1 : 1;
            e->x += dir;

            if (dir < 0) {
                if (e->x <= w->x) {
                    e->data = heroic_toast_right_23x23;
                    e->x = w->x;
                }
            } else {
                if (e->x + e->data.size_x >= w->x + w->width) {
                    e->data = heroic_toast_left_23x23;
                    e->x = w->x + w->width - e->data.size_x;
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
        if (intersects(e->x, e->y, e->data.size_x, e->data.size_y, ninja.x, ninja.y, ninja.data.size_x, ninja.data.size_y)) {
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
            melons_remaining--;
        }
    }

    // Read player input from hardware buttons
    bool in_jump = (button_state & 2) != 0;
    bool in_left = (button_state & 4) != 0;
    bool in_right = (button_state & 1) != 0;

    // Read play input from keyboard
    mutex_enter_blocking(&hid_input_lock);
    in_jump |= hid_jump_pressed;
    in_left |= hid_left_pressed;
    in_right |= hid_right_pressed;

    if (hid_cheat_level_up) {
        current_level_index++;
        load_pending = hid_cheat_level_up;
        hid_cheat_level_up = false;
    }
    mutex_exit(&hid_input_lock);

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

        // Check if the next level should be loaded
        if (i == 0 && melons_remaining == 0) {
            current_level_index++;
            load_pending = true;
            break;
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
        ninja_yvel = GRAVITY;
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
            ninja.data = ninja_left_32x32;
        } else {
            ninja.data = ninja_right_32x32;
        }
    }
    ninja.x = (int16_t)(ninja_xpos / DIMDIV);
    ninja.y = (int16_t)(ninja_ypos / DIMDIV);

    // Update off screen indicators
    if (ninja.y < 0) {
        up_arrow.x = ninja.x;
        up_arrow.y = 0;
    } else {
        up_arrow.x = -100;
    }
    if (ninja.x < 0) {
        left_arrow.x = 0;
        left_arrow.y = ninja.y;
    } else {
        left_arrow.x = -100;
    }
    if (ninja.x + ninja.data.size_x > VGA_MODE.width) {
        right_arrow.x = VGA_MODE.width - right_arrow.data.size_x;
        right_arrow.y = ninja.y;
    } else {
        right_arrow.x = -100;
    }

    debug_str_length = snprintf(
        debug_str,
        DEBUG_STR_MAX_LEN,
        "F:%u L:%u R:%u T:%u B:%u LVL:%u RUS:%u",
        frame_number,
        touching_left_wall,
        touching_right_wall,
        touching_roof,
        touching_floor,
        current_level_index,
        (unsigned int)render_micros
    );
    render_micros = 0;
}

static void __time_critical_func(vga_board_button_irq_handler)() {
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

static void vga_board_init_buttons()
{
    gpio_set_irq_enabled(VSYNC_PIN, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true);
    irq_set_exclusive_handler(IO_IRQ_BANK0, vga_board_button_irq_handler);
    irq_set_enabled(IO_IRQ_BANK0, true);
}

int main(void)
{
    // Set PSU into PWM mode for reduced ripple (and reduced efficiency)
    gpio_set_dir(23, true);
    gpio_pull_up(23);

    // Setup LED to blink
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    // Overclock
    vreg_set_voltage(VREG_VOLTAGE_1_30);
    set_sys_clock_khz(250000, true);

    // Setup serial communication and buttons
    stdio_init_all();
    vga_board_init_buttons();

    // Initialise USB system
    board_init();
    tusb_init();

    // Load a level
    current_level_index = 0;
    set_level(levels[current_level_index]);

    // Initialise audio system
    //audio_producer_queue = init_audio();

    // Initialise video system
    mutex_init(&hid_input_lock);
    init_scanline_rendering(&VGA_MODE);

    // Enter the infinite video loop
    render_loop();
}

