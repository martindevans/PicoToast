#include <stdint.h>

#include "common/tusb_common.h"

CFG_TUSB_MEM_SECTION static hid_keyboard_report_t usb_keyboard_report;

void process_kbd_report(hid_keyboard_report_t const *p_new_report);

static inline void hid_task() {
    uint8_t const addr = 1;

    if (tuh_hid_keyboard_is_mounted(addr)) {
        if (!tuh_hid_keyboard_is_busy(addr)) {
            process_kbd_report(&usb_keyboard_report);
            tuh_hid_keyboard_get_report(addr, &usb_keyboard_report);
        }
    }
}

void tuh_hid_keyboard_mounted_cb(uint8_t dev_addr) {
    tuh_hid_keyboard_get_report(dev_addr, &usb_keyboard_report);
}

void tuh_hid_keyboard_unmounted_cb(uint8_t dev_addr) {
    (void)dev_addr;
}

// invoked ISR context
void tuh_hid_keyboard_isr(uint8_t dev_addr, xfer_result_t event) {
    (void)dev_addr;
    (void)event;
}