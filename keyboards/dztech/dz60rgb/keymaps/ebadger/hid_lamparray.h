#pragma once
#ifdef HID_LAMPARRAY_ENABLE
#include "quantum.h"

void lamparray_housekeeping(void);

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

typedef struct _LAMP_INFO_IN {

    int32_t PositionX;
    int32_t PositionY;
    uint8_t LampKey;

} LAMP_INFO_IN, *PLAMP_INFO_IN;

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
//    uint8_t     report_id;
    uint16_t    lamp_count;
    struct {
        int32_t    x;
        int32_t    y;
        int32_t    z;
    } bounding_box;
    int32_t    array_kind;
    int32_t    min_update_interval;
} __attribute__((packed, aligned(1))) hid_lamparray_attributes_report_t;
#define HID_LAMPARRAY_ATTRIBUTES_REPORT_SIZE sizeof(hid_lamparray_attributes_report_t)

typedef struct {
//    uint8_t     report_id;
    uint16_t    lamp_id;
} __attribute__((packed, aligned(1))) hid_lamparray_attributes_request_report_t;
#define HID_LAMPARRAY_ATTRIBUTES_REQUEST_REPORT_SIZE sizeof(hid_lamparray_attributes_request_report_t)

typedef struct {
//    uint8_t     report_id;
    uint16_t        lamp_id;
    struct {
        int32_t    x;
        int32_t    y;
        int32_t    z;
    } position;
    int32_t        update_latency;
    int32_t        lamp_purposes;
    struct {
        uint8_t     red;
        uint8_t     green;
        uint8_t     blue;
        uint8_t     intensity;
    } level_counts;
    uint8_t         is_programmable;
    uint8_t         input_binding;
} __attribute__((packed, aligned(1))) hid_lamparray_attributes_response_report_t;
#define HID_LAMPARRAY_ATTRIBUTES_RESPONSE_REPORT_SIZE sizeof(hid_lamparray_attributes_response_report_t)

typedef struct {
 //   uint8_t             report_id;
    uint8_t                   lamp_count;
    uint8_t                   flags;
    int16_t                   lamp_ids[8];
    hid_lamparray_color_t     colors[8];
} __attribute__((packed, aligned(1))) hid_lamparray_multi_update_report_t;
#define HID_LAMPARRAY_MULTI_UPDATE_REPORT_SIZE sizeof(hid_lamparray_multi_update_report_t)

typedef struct PACKED {
 //   uint8_t     report_id;
    uint8_t     flags;
    uint16_t    lamp_id_start;
    uint16_t    lamp_id_end;
    hid_lamparray_color_t color;
} __attribute__((packed, aligned(1))) hid_lamparray_range_update_report_t;
#define HID_LAMPARRAY_RANGE_UPDATE_REPORT_SIZE sizeof(hid_lamparray_range_update_report_t)

typedef struct {
 //   uint8_t     report_id;
    uint8_t     autonomous_mode;
} __attribute__((packed, aligned(1))) hid_lamparray_control_report_t;
#define HID_LAMPARRAY_CONTROL_REPORT_SIZE sizeof(hid_lamparray_control_report_t)


#endif
