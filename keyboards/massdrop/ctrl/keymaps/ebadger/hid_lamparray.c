#ifdef HID_LAMPARRAY_ENABLE
#include "hid_lamparray.h"
#include "md_rgb_matrix.h"

bool hid_lamparray_auto_mode = true;
int process_count = 0;
uint32_t rgb_timer = 0;

float maximum_y_position = 0;
float minimum_x_position = 0;

void map_keybindings(void) {
    uint8_t led_i[8];
    for (int i = 0; i < DRIVER_LED_TOTAL; i++) {
        led_keymap[i] = 0;

        if (led_map[i].y > maximum_y_position) {
            maximum_y_position = led_map[i].y;
        }

        if (led_map[i].x < minimum_x_position) {
            minimum_x_position = led_map[i].x;
        }
    }

    for (int col = 0; col < MATRIX_COLS; col++) {
        for (int row = 0; row < MATRIX_ROWS; row++) {
            int count = rgb_matrix_map_row_column_to_led(row, col, led_i);
            for (int l = 0; l < count; l++) {
                uint16_t key = keymaps[0][row][col];
                if (IS_ANY(key))
                    led_keymap[led_i[l]] = key & 0xFF;
            }
        }
    }
}

uint8_t get_led_binding(uint8_t lamp_id) {
    return led_keymap[lamp_id];
}

void fill_lamp_attributes(hid_lamparray_attributes_response_report_t *report) {
    report->position.x = (uint32_t)((led_map[report->lamp_id].x + minimum_x_position) * 25400);
    report->position.y = (uint32_t)((led_map[report->lamp_id].y - maximum_y_position) * -25400);
    report->input_binding = get_led_binding(report->lamp_id);
    report->lamp_purposes = 0x02;
    if (g_led_config.flags[report->lamp_id] & LED_FLAG_KEYLIGHT)
        report->lamp_purposes = 0x01;
    if (g_led_config.flags[report->lamp_id] & LED_FLAG_MODIFIER)
        report->lamp_purposes = 0x01;
    if (g_led_config.flags[report->lamp_id] & LED_FLAG_INDICATOR)
        report->lamp_purposes = report->lamp_purposes & 0x08;
    report->generic_input_binding_usage_page = 0x07;
    report->generic_input_binding_usage_id = get_led_binding(report->lamp_id);
}

void hid_lamparray_recv(uint8_t *data, uint8_t length) {
    switch (*data) {
        case hid_lamparray_multi_update_report_id: {
            if (hid_lamparray_auto_mode)
                return;
            hid_lamparray_multi_update_report_t *report = (hid_lamparray_multi_update_report_t *)data;
            for (int i = 0; i < report->lamp_count; i++) {
                if (report->lamp_ids[i] < DRIVER_LED_TOTAL && report->lamp_ids[i] >= 0) {
                    rgb_matrix_set_color(report->lamp_ids[i], report->colors[i].red, report->colors[i].green, report->colors[i].blue);
                    process_count++;
                }
            }

            rgb_timer = timer_read32();

        break;
        }
        case hid_lamparray_range_update_report_id: {
            if (hid_lamparray_auto_mode)
                return;
            hid_lamparray_range_update_report_t *report = (hid_lamparray_range_update_report_t *)data;
            for (int i = report->lamp_id_start; i <= report->lamp_id_end && i < DRIVER_LED_TOTAL; i++) {
                    rgb_matrix_set_color(i, report->color.red, report->color.green, report->color.blue);
                    process_count++;

            }

            rgb_timer = timer_read32();

            break;
        }
        case hid_lamparray_control_report_id: {
            hid_lamparray_control_report_t *report = (hid_lamparray_control_report_t *)data;
            if (hid_lamparray_auto_mode && report->autonomous_mode == 0) {
                rgb_matrix_disable_noeeprom();
                rgb_timer = timer_read32();
                hid_lamparray_auto_mode = false;
            } else if (!hid_lamparray_auto_mode && report->autonomous_mode == 1) {
                rgb_matrix_enable_noeeprom();
                rgb_timer = timer_read32();
                hid_lamparray_auto_mode = true;
            }
        break;
        }
    };
}
#endif
