#pragma once
#ifdef HID_LAMPARRAY_ENABLE
#include "quantum.h"

#define HID_LAMPARRAY_REPORT_1 hid_lamparray_attributes_report_t
#define HID_LAMPARRAY_REPORT_2 hid_lamparray_attributes_request_report_t
#define HID_LAMPARRAY_REPORT_3 hid_lamparray_attributes_response_report_t
#define HID_LAMPARRAY_REPORT_4 hid_lamparray_multi_update_report_t
#define HID_LAMPARRAY_REPORT_5 hid_lamparray_range_update_report_t
#define HID_LAMPARRAY_REPORT_6 hid_lamparray_control_report_t

#define HID_LAMPARRAY_REPORTS 6

enum hid_lamparray_report_id {
    hid_lamparray_attributes_report_id = 1,
    hid_lamparray_attributes_request_report_id = 2,
    hid_lamparray_attributes_response_report_id = 3,
    hid_lamparray_multi_update_report_id = 4,
    hid_lamparray_range_update_report_id = 5,
    hid_lamparray_control_report_id = 6
};


typedef struct {
    uint8_t     red;
    uint8_t     green;
    uint8_t     blue;
    uint8_t     intensity;
} __attribute__((aligned(1))) hid_lamparray_color_t;

typedef struct {
    uint32_t    x;
    uint32_t    y;
    uint32_t    z;
} __attribute__((aligned(1))) hid_lamparray_position_t;

typedef struct {
    uint8_t     report_id;
    uint16_t    lamp_count;
    struct {
        uint32_t    x;
        uint32_t    y;
        uint32_t    z;
    } bounding_box;
    uint32_t    array_kind;
    uint32_t    min_update_interval;
} __attribute__((packed, aligned(1))) hid_lamparray_attributes_report_t;
#define HID_LAMPARRAY_ATTRIBUTES_REPORT_SIZE sizeof(hid_lamparray_attributes_report_t)

typedef struct {
    uint8_t     report_id;
    uint16_t    lamp_id;
} __attribute__((packed, aligned(1))) hid_lamparray_attributes_request_report_t;
#define HID_LAMPARRAY_ATTRIBUTES_REQUEST_REPORT_SIZE sizeof(hid_lamparray_attributes_request_report_t)

typedef struct {
    uint8_t     report_id;
    uint16_t    lamp_id;
    struct {
        uint32_t    x;
        uint32_t    y;
        uint32_t    z;
    } position;
    uint32_t    update_latency;
    uint32_t    lamp_purposes;
    struct {
        uint8_t     red;
        uint8_t     green;
        uint8_t     blue;
        uint8_t     intensity;
    } level_counts;
    uint8_t    is_programmable;
    uint8_t    input_binding;
} __attribute__((packed, aligned(1))) hid_lamparray_attributes_response_report_t;
#define HID_LAMPARRAY_ATTRIBUTES_RESPONSE_REPORT_SIZE sizeof(hid_lamparray_attributes_response_report_t)

typedef struct {
    uint8_t             report_id;
    uint8_t             lamp_count;
    uint8_t             flags;
    uint16_t            lamp_ids[8];
    hid_lamparray_color_t     colors[8];
} __attribute__((packed, aligned(1))) hid_lamparray_multi_update_report_t;
#define HID_LAMPARRAY_MULTI_UPDATE_REPORT_SIZE sizeof(hid_lamparray_multi_update_report_t)

typedef struct PACKED {
    uint8_t     report_id;
    uint8_t     flags;
    uint16_t    lamp_id_start;
    uint16_t    lamp_id_end;
    hid_lamparray_color_t color;
} __attribute__((packed, aligned(1))) hid_lamparray_range_update_report_t;
#define HID_LAMPARRAY_RANGE_UPDATE_REPORT_SIZE sizeof(hid_lamparray_range_update_report_t)

typedef struct {
    uint8_t     report_id;
    uint8_t     autonomous_mode;
} __attribute__((packed, aligned(1))) hid_lamparray_control_report_t;
#define HID_LAMPARRAY_CONTROL_REPORT_SIZE sizeof(hid_lamparray_control_report_t)

void hid_lamparray_recv(uint8_t *data, uint8_t length);
void get_lamp_attributes(uint8_t lamp_id, uint8_t *responseBuffer);
void fill_lamp_attributes(hid_lamparray_attributes_response_report_t *report);
uint8_t get_led_binding(uint8_t lamp_id);
bool hid_lamparray_auto_mode;

uint8_t led_keymap[DRIVER_LED_TOTAL];
void map_keybindings(void);

#define HID_LAMPARRAY_DESC { \
            0x05, 0x59, /* USAGE_PAGE (LightingAndIllumination) */ \
            0x09, 0x01, /* USAGE (LampArray) */ \
            0xa1, 0x01, /* COLLECTION (Application) */ \
            0x85, 0x01, /* REPORT_ID (1) */ \
            0x09, 0x02, /* USAGE (LampArrayAttributesReport) */ \
            0xa1, 0x02, /* COLLECTION (Logical) */ \
            0x09, 0x03, /* USAGE (LampCount) */ \
            0x15, 0x00, /* LOGICAL_MINIMUM (0) */ \
            0x27, 0xff, 0xff, 0x00, 0x00, /* LOGICAL_MAXIMUM (65535) */\
            0x75, 0x10, /* REPORT_SIZE (16) */ \
            0x95, 0x01, /* REPORT_COUNT (1) */ \
            0xb1, 0x03, /* FEATURE (Cnst,Var,Abs) */ \
            0x09, 0x04, /* USAGE (BoundingBoxWidthInMicrometers) */ \
            0x09, 0x05, /* USAGE (BoundingBoxHeightInMicrometers) */ \
            0x09, 0x06, /* USAGE (BoundingBoxDepthInMicrometers) */ \
            0x09, 0x07, /* USAGE (LampArrayKind) */ \
            0x09, 0x08, /* USAGE (MinUpdateIntervalInMicroseconds) */ \
            0x15, 0x00, /* LOGICAL_MINIMUM (0) */ \
            0x27, 0xff, 0xff, 0xff, 0x7f, /* LOGICAL_MAXIMUM (2147483647) */ \
            0x75, 0x20, /* REPORT_SIZE (32) */ \
            0x95, 0x05, /* REPORT_COUNT (5) */ \
            0xb1, 0x03, /* FEATURE (Cnst,Var,Abs) */ \
            0xc0, /* END_COLLECTION*/ \
            0x85, 0x02, /* REPORT_ID (2) */ \
            0x09, 0x20, /* USAGE (LampAttributesRequestReport) */ \
            0xa1, 0x02, /* COLLECTION (Logical) */ \
            0x09, 0x21, /* USAGE (LampId) */ \
            0x15, 0x00, /* LOGICAL_MINIMUM (0) */ \
            0x27, 0xff, 0xff, 0x00, 0x00, /* LOGICAL_MAXIMUM (65535) */ \
            0x75, 0x10, /* REPORT_SIZE (16) */ \
            0x95, 0x01, /* REPORT_COUNT (1) */ \
            0xb1, 0x02, /* FEATURE (Data,Var,Abs) */ \
            0xc0, /* END_COLLECTION*/ \
            0x85, 0x03, /* REPORT_ID (3) */ \
            0x09, 0x22, /* USAGE (LampAttributesReponseReport) */ \
            0xa1, 0x02, /* COLLECTION (Logical) */ \
            0x09, 0x21, /* USAGE (LampId) */ \
            0x15, 0x00, /* LOGICAL_MINIMUM (0) */ \
            0x27, 0xff, 0xff, 0x00, 0x00, /* LOGICAL_MAXIMUM (65535) */ \
            0x75, 0x10, /* REPORT_SIZE (16) */ \
            0x95, 0x01, /* REPORT_COUNT (1) */ \
            0xb1, 0x02, /* FEATURE (Data,Var,Abs) */ \
            0x09, 0x23, /* USAGE (PositionXInMicrometers) */ \
            0x09, 0x24, /* USAGE (PositionYInMicrometers) */ \
            0x09, 0x25, /* USAGE (PositionZInMicrometers) */ \
            0x09, 0x27, /* USAGE (UpdateLatencyInMicroseconds) */ \
            0x09, 0x26, /* USAGE (LampPurposes) */ \
            0x15, 0x00, /* LOGICAL_MINIMUM (0) */ \
            0x27, 0xff, 0xff, 0xff, 0x7f, /* LOGICAL_MAXIMUM (2147483647) */ \
            0x75, 0x20, /* REPORT_SIZE (32) */ \
            0x95, 0x05, /* REPORT_COUNT (5) */ \
            0xb1, 0x02, /* FEATURE (Data,Var,Abs) */ \
            0x09, 0x28, /* USAGE (RedLevelCount) */ \
            0x09, 0x29, /* USAGE (GreenLevelCount) */ \
            0x09, 0x2a, /* USAGE (BlueLevelCount) */ \
            0x09, 0x2b, /* USAGE (IntensityLevelCount) */ \
            0x09, 0x2c, /* USAGE (IsProgrammable) */ \
            0x09, 0x2d, /* USAGE (InputBinding) */ \
            0x15, 0x00, /* LOGICAL_MINIMUM (0) */ \
            0x26, 0xff, 0x00, /* LOGICAL_MAXIMUM (255) */ \
            0x75, 0x08, /* REPORT_SIZE (8) */ \
            0x95, 0x06, /* REPORT_COUNT (6) */ \
            0xb1, 0x02, /* FEATURE (Data,Var,Abs) */ \
            0xc0, /* END_COLLECTION*/ \
            0x85, 0x04, /* REPORT_ID (4) */ \
            0x09, 0x50, /* USAGE (LampMultiUpdateReport) */ \
            0xa1, 0x02, /* COLLECTION (Logical) */ \
            0x09, 0x03, /* USAGE (LampCount) */ \
            0x09, 0x55, /* USAGE (LampUpdateFlags) */ \
            0x15, 0x00, /* LOGICAL_MINIMUM (0) */ \
            0x25, 0x08, /* LOGICAL_MAXIMUM (8) */ \
            0x75, 0x08, /* REPORT_SIZE (8) */ \
            0x95, 0x02, /* REPORT_COUNT (2) */ \
            0xb1, 0x02, /* FEATURE (Data,Var,Abs) */ \
            0x09, 0x21, /* USAGE (LampId) */ \
            0x15, 0x00, /* LOGICAL_MINIMUM (0) */ \
            0x27, 0xff, 0xff, 0x00, 0x00, /* LOGICAL_MAXIMUM (65535) */ \
            0x75, 0x10, /* REPORT_SIZE (16) */ \
            0x95, 0x08, /* REPORT_COUNT (8) */ \
            0xb1, 0x02, /* FEATURE (Data,Var,Abs) */ \
            0x09, 0x51, /* USAGE (RedUpdateChannel) */ \
            0x09, 0x52, /* USAGE (GreenUpdateChannel) */ \
            0x09, 0x53, /* USAGE (BlueUpdateChannel) */ \
            0x09, 0x54, /* USAGE (IntensityUpdateChannel) */ \
            0x09, 0x51, /* USAGE (RedUpdateChannel) */ \
            0x09, 0x52, /* USAGE (GreenUpdateChannel) */ \
            0x09, 0x53, /* USAGE (BlueUpdateChannel) */ \
            0x09, 0x54, /* USAGE (IntensityUpdateChannel) */ \
            0x09, 0x51, /* USAGE (RedUpdateChannel) */ \
            0x09, 0x52, /* USAGE (GreenUpdateChannel) */ \
            0x09, 0x53, /* USAGE (BlueUpdateChannel) */ \
            0x09, 0x54, /* USAGE (IntensityUpdateChannel) */ \
            0x09, 0x51, /* USAGE (RedUpdateChannel) */ \
            0x09, 0x52, /* USAGE (GreenUpdateChannel) */ \
            0x09, 0x53, /* USAGE (BlueUpdateChannel) */ \
            0x09, 0x54, /* USAGE (IntensityUpdateChannel) */ \
            0x09, 0x51, /* USAGE (RedUpdateChannel) */ \
            0x09, 0x52, /* USAGE (GreenUpdateChannel) */ \
            0x09, 0x53, /* USAGE (BlueUpdateChannel) */ \
            0x09, 0x54, /* USAGE (IntensityUpdateChannel) */ \
            0x09, 0x51, /* USAGE (RedUpdateChannel) */ \
            0x09, 0x52, /* USAGE (GreenUpdateChannel) */ \
            0x09, 0x53, /* USAGE (BlueUpdateChannel) */ \
            0x09, 0x54, /* USAGE (IntensityUpdateChannel) */ \
            0x09, 0x51, /* USAGE (RedUpdateChannel) */ \
            0x09, 0x52, /* USAGE (GreenUpdateChannel) */ \
            0x09, 0x53, /* USAGE (BlueUpdateChannel) */ \
            0x09, 0x54, /* USAGE (IntensityUpdateChannel) */ \
            0x09, 0x51, /* USAGE (RedUpdateChannel) */ \
            0x09, 0x52, /* USAGE (GreenUpdateChannel) */ \
            0x09, 0x53, /* USAGE (BlueUpdateChannel) */ \
            0x09, 0x54, /* USAGE (IntensityUpdateChannel) */ \
            0x15, 0x00, /* LOGICAL_MINIMUM (0) */ \
            0x26, 0xff, 0x00, /* LOGICAL_MAXIMUM (255) */ \
            0x75, 0x08, /* REPORT_SIZE (8) */ \
            0x95, 0x20, /* REPORT_COUNT (32) */ \
            0xb1, 0x02, /* FEATURE (Data,Var,Abs) */ \
            0xc0, /* END_COLLECTION*/ \
            0x85, 0x05, /* REPORT_ID (5) */ \
            0x09, 0x60, /* USAGE (LampRangeUpdateReport) */ \
            0xa1, 0x02, /* COLLECTION (Logical) */ \
            0x09, 0x55, /* USAGE (LampUpdateFlags) */ \
            0x15, 0x00, /* LOGICAL_MINIMUM (0) */ \
            0x25, 0x08, /* LOGICAL_MAXIMUM (8) */ \
            0x75, 0x08, /* REPORT_SIZE (8) */ \
            0x95, 0x01, /* REPORT_COUNT (1) */ \
            0xb1, 0x02, /* FEATURE (Data,Var,Abs) */ \
            0x09, 0x61, /* USAGE (LampIdStart) */ \
            0x09, 0x62, /* USAGE (LampIdEnd) */ \
            0x15, 0x00, /* LOGICAL_MINIMUM (0) */ \
            0x27, 0xff, 0xff, 0x00, 0x00, /* LOGICAL_MAXIMUM (65535) */ \
            0x75, 0x10, /* REPORT_SIZE (16) */ \
            0x95, 0x02, /* REPORT_COUNT (2) */ \
            0xb1, 0x02, /* FEATURE (Data,Var,Abs) */ \
            0x09, 0x51, /* USAGE (RedUpdateChannel) */ \
            0x09, 0x52, /* USAGE (GreenUpdateChannel) */ \
            0x09, 0x53, /* USAGE (BlueUpdateChannel) */ \
            0x09, 0x54, /* USAGE (IntensityUpdateChannel) */ \
            0x15, 0x00, /* LOGICAL_MINIMUM (0) */ \
            0x26, 0xff, 0x00, /* LOGICAL_MAXIMUM (255) */ \
            0x75, 0x08, /* REPORT_SIZE (8) */ \
            0x95, 0x04, /* REPORT_COUNT (4) */ \
            0xb1, 0x02, /* FEATURE (Data,Var,Abs) */ \
            0xc0, /* END_COLLECTION*/ \
            0x85, 0x06, /* REPORT_ID (6) */ \
            0x09, 0x70, /* USAGE (LampArrayControlReport) */ \
            0xa1, 0x02, /* COLLECTION (Logical) */ \
            0x09, 0x71, /* USAGE (AutonomousMode) */ \
            0x15, 0x00, /* LOGICAL_MINIMUM (0) */ \
            0x25, 0x01, /* LOGICAL_MAXIMUM (1) */ \
            0x75, 0x08, /* REPORT_SIZE (8) */ \
            0x95, 0x01, /* REPORT_COUNT (1) */ \
            0xb1, 0x02, /* FEATURE (Data,Var,Abs) */ \
            0xc0, /* END_COLLECTION*/ \
            0xc0 /* END_COLLECTION*/ \
            }
#endif