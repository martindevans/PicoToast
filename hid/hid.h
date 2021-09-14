#include <stdint.h>

#include "tusb.h"

#define MAX_REPORT  4

static uint8_t const keycode2ascii[128][2] =  { HID_KEYCODE_TO_ASCII };

// Each HID instance can has multiple reports
static uint8_t _report_count[CFG_TUH_HID];
static tuh_hid_report_info_t _report_info_arr[CFG_TUH_HID][MAX_REPORT];

static void process_kbd_report(hid_keyboard_report_t const *report);
static void process_mouse_report(hid_mouse_report_t const * report);

CFG_TUSB_MEM_SECTION static hid_keyboard_report_t usb_keyboard_report;

void process_kbd_report(hid_keyboard_report_t const *p_new_report);

static inline void hid_task() {
    // uint8_t const addr = 1;

    // if (tuh_hid_keyboard_is_mounted(addr)) {
    //     if (!tuh_hid_keyboard_is_busy(addr)) {
    //         process_kbd_report(&usb_keyboard_report);
    //         tuh_hid_keyboard_get_report(addr, &usb_keyboard_report);
    //     }
    // }
}

// Invoked when device with hid interface is mounted
// Report descriptor is also available for use. tuh_hid_parse_report_descriptor()
// can be used to parse common/simple enough descriptor.
void tuh_hid_mount_cb(uint8_t dev_addr, uint8_t instance, uint8_t const* desc_report, uint16_t desc_len)
{
    printf("HID device address = %d, instance = %d is mounted\r\n", dev_addr, instance);

    // Interface protocol
    const char* protocol_str[] = { "None", "Keyboard", "Mouse" }; // hid_protocol_type_t
    uint8_t const interface_protocol = tuh_hid_interface_protocol(dev_addr, instance);

    // Parse report descriptor with built-in parser
    _report_count[instance] = tuh_hid_parse_report_descriptor(_report_info_arr[instance], MAX_REPORT, desc_report, desc_len);
    printf("HID has %u reports and interface protocol = %s\r\n", _report_count[instance], protocol_str[interface_protocol]);
}

// Invoked when received report from device via interrupt endpoint
void tuh_hid_report_received_cb(uint8_t dev_addr, uint8_t instance, uint8_t const* report, uint16_t len)
{
  (void) dev_addr;

  uint8_t const rpt_count = _report_count[instance];
  tuh_hid_report_info_t* rpt_info_arr = _report_info_arr[instance];
  tuh_hid_report_info_t* rpt_info = NULL;

  if (rpt_count == 1 && rpt_info_arr[0].report_id == 0)
  {
    // Simple report without report ID as 1st byte
    rpt_info = &rpt_info_arr[0];
  }
  else
  {
    // Composite report, 1st byte is report ID, data starts from 2nd byte
    uint8_t const rpt_id = report[0];

    // Find report id in the arrray
    for(uint8_t i=0; i<rpt_count; i++)
    {
      if (rpt_id == rpt_info_arr[i].report_id )
      {
        rpt_info = &rpt_info_arr[i];
        break;
      }
    }

    report++;
    len--;
  }

  if (!rpt_info)
  {
    printf("Couldn't find the report info for this report !\r\n");
    return;
  }

  if (rpt_info->usage_page == HID_USAGE_PAGE_DESKTOP)
  {
    switch (rpt_info->usage)
    {
      case HID_USAGE_DESKTOP_KEYBOARD:
        TU_LOG1("HID receive keyboard report\r\n");
        // Assume keyboard follow boot report layout
        process_kbd_report((hid_keyboard_report_t const*)report);
        break;

      default:
        break;
    }
  }
}

void tuh_hid_keyboard_mounted_cb(uint8_t dev_addr) {
    //tuh_hid_keyboard_get_report(dev_addr, &usb_keyboard_report);
}

void tuh_hid_keyboard_unmounted_cb(uint8_t dev_addr) {
    (void)dev_addr;
}

// invoked ISR context
void tuh_hid_keyboard_isr(uint8_t dev_addr, xfer_result_t event) {
    (void)dev_addr;
    (void)event;
}