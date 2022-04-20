#include "ctrl.h"
#include "raw_hid.h"
#include "via.h"
#include "hid_lamparray.h"
#include "config.h"

enum via_qmk_rgbmatrix_lighting_value {
    id_qmk_rgbmatrix_flags          = 0xC0,
    id_qmk_rgbmatrix_mode           = 0xC1,
    id_qmk_rgbmatrix_speed          = 0xC2,
    id_qmk_rgbmatrix_hsv            = 0xC3,
    id_qmk_rgbmatrix_led_color      = 0xC4,
    id_qmk_rgbmatrix_key_color      = 0xC5
};

void raw_hid_receive_kb(uint8_t *data, uint8_t length);
void via_qmk_rgbmatrix_set_value(uint8_t *data);
void via_qmk_rgb_matrix_set_led_color(uint8_t *value_data, uint8_t count);
void via_qmk_rgb_matrix_set_key_color(uint8_t *value_data, uint8_t count);