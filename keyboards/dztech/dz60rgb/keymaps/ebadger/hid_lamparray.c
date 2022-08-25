#include "lamparray.h"
#include "hid_lamparray.h"
#include <LUFA/Drivers/USB/USB.h>
#include <LUFA/Platform/Platform.h>
#include "hidusages.h"

uint32_t rgb_timer = 0;
uint16_t _lastLampID = 0;
bool hid_lamparray_auto_mode = true;
bool _dirty = false;

static const LAMP_INFO_IN s_lampAttributes[] =
{
    // All lengths in micrometers.
    // All times in microseconds.
    // UpdateLatency for all Lamps set to 4msec as it just seems reasonable.
            //X    Y    Key
        { 261900,13700, HID_BACKSPACE }, // backspace
        { 233900,13700, HID_EQUAL },  // +
        { 215140,13700, HID_HYPHEN },  // -
        { 196380,13700, HID_0  },
        { 177620,13700, HID_9  },
        { 158860,13700, HID_8  },
        { 140100,13700, HID_7  },
        { 121340,13700, HID_6  },
        { 102580,13700, HID_5  },
        { 83820,13700,  HID_4  },
        { 65060,13700,  HID_3  },
        { 46300,13700,  HID_2  },
        { 27000,4300,   HID_1  },
        { 8000,4300,    HID_ESCAPE }, // escape
        { 267200,32800, HID_BACKSLASH }, // backslash
        { 243500,32800, HID_RIGHT_BRACKET }, // ]
        { 224740,32800, HID_LEFT_BRACKET }, // [
        { 205980,32800, HID_P  },
        { 187220,32800, HID_O  },
        { 168460,32800, HID_I  },
        { 149700,32800, HID_U  },
        { 130940,32800, HID_Y  },
        { 112180,32800, HID_T  },
        { 93420,32800,  HID_R  },
        { 74660,32800,  HID_E  },
        { 55900,32800,  HID_W  },
        { 37140,32800,  HID_Q  },
        { 12300,32800,  HID_TAB }, // tab
        { 260000,52100, HID_ENTER }, // return
        { 228900,52100, HID_APOSTROPHE },
        { 210140,52100, HID_SEMICOLON  },
        { 191380,52100, HID_L  },
        { 172620,52100, HID_K  },
        { 153860,52100, HID_J  },
        { 135100,52100, HID_H  },
        { 116340,52100, HID_G  },
        { 97580,52100,  HID_F  },
        { 78820,52100,  HID_D  },
        { 60060,52100,  HID_S  },
        { 41300,52100,  HID_A  },
        { 15100,52100,  HID_CAPS }, // caps
        { 271200,71200, HID_DELETE }, // delete
        { 252600,71200, HID_UP }, // up
        { 226500,71200, HID_RIGHT_SHIFT }, // shift
        { 200400,71200, HID_GREATERTHAN  },
        { 181640,71200, HID_LESSTHAN  },
        { 162880,71200, HID_M  },
        { 144120,71200, HID_N  },
        { 125360,71200, HID_B  },
        { 106600,71200, HID_V  },
        { 87840,71200,  HID_C  },
        { 69080,71200,  HID_X  },
        { 50320,71200,  HID_Z  },
        { 19600,71200,  HID_LEFT_SHIFT }, // shift
        { 271200,90600, HID_RIGHT }, // right
        { 252600,90600, HID_DOWN }, // down
        { 234000,90600, HID_LEFT }, //left
        { 215400,90600, HID_RIGHT_CONTROL }, //ctrl
        { 196800,90600, HID_RIGHT_ALT }, // alt
        { 128500,90600, HID_SPACE }, // space
        { 57500,90600,  HID_LEFT_ALT }, // alt
        { 33800,90600,  HID_LEFT_GUI }, // super?
        { 10200,90600,  HID_LEFT_CONTROL }, // ctrl

};

void lamparray_housekeeping(void)
{
    if (rgb_timer == 0 || timer_elapsed32(rgb_timer) >= RGB_MATRIX_LED_FLUSH_LIMIT)
    {
        if (_dirty)
        {
            rgb_matrix_update_pwm_buffers();
            _dirty = false;
        }
        rgb_timer = timer_read32();
    }
}

bool CALLBACK_HID_Device_CreateHIDReport(USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo,
                                         uint8_t* const ReportID,
                                         const uint8_t ReportType,
                                         void* ReportData,
                                         uint16_t* const ReportSize)
{

#ifdef CONSOLE_ENABLE
    dprintf("CALLBACK_HID_Device_CreateHIDReport ReportID=%d, ReportType=%d\n", *ReportID, ReportType);
#endif

    switch(*ReportID)
    {
        case hid_lamparray_attributes_report_id:
        {
            hid_lamparray_attributes_report_t report = {};
            //report.report_id = *ReportID;
            report.lamp_count = DRIVER_LED_TOTAL;
            report.bounding_box.x = 282000; // 282mm
            report.bounding_box.y = 95000;  // 95mm
            report.bounding_box.z = 2000;   // 2mm
            report.array_kind = 1; // LampArrayKind Keyboard
            report.min_update_interval = 4000;  // 4ms
            *ReportSize = sizeof(report);
            _lastLampID = 0;
            memcpy(ReportData, &report, *ReportSize);

            break;
        }



        case hid_lamparray_attributes_response_report_id :
        {
            const LAMP_INFO_IN *p = &s_lampAttributes[_lastLampID];

            hid_lamparray_attributes_response_report_t report = {};
            //report.report_id = *ReportID;
            report.lamp_id = _lastLampID;
            report.position.x = p->PositionX;
            report.position.y = p->PositionY;
            report.position.z = 0x00;
            report.update_latency = 4000;
            report.lamp_purposes = 2;
            report.level_counts.red = 0xFF;
            report.level_counts.green = 0xFF;
            report.level_counts.blue = 0xFF;
            report.level_counts.intensity = 0x1;
            report.is_programmable = 1;
            report.input_binding = p->LampKey;

            _lastLampID++;

            if (_lastLampID  >= DRIVER_LED_TOTAL)
            {
                _lastLampID = 0;
            }

            *ReportSize = sizeof(report);
            memcpy(ReportData, &report, *ReportSize);

            break;
        }
    }

	return false;
}

void CALLBACK_HID_Device_ProcessHIDReport(USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo,
                                          const uint8_t ReportID,
                                          const uint8_t ReportType,
                                          const void* ReportData,
                                          const uint16_t ReportSize)
{

#ifdef CONSOLE_ENABLE
    dprintf("CALLBACK_HID_Device_ProcessHIDReport ReportID=%d, ReportType=%d\n", ReportID, ReportType);
#endif

    switch (ReportID)
    {
        case hid_lamparray_attributes_request_report_id :
        {
            hid_lamparray_attributes_request_report_t *report = (hid_lamparray_attributes_request_report_t *)ReportData;
            //report.report_id = *ReportID;
            _lastLampID = report->lamp_id;

            if (_lastLampID  >= DRIVER_LED_TOTAL)
            {
                _lastLampID = 0;
            }

            break;
        }
        case hid_lamparray_multi_update_report_id:
        {
            if (hid_lamparray_auto_mode)
                {
                    return;
                }

            hid_lamparray_multi_update_report_t *report = (hid_lamparray_multi_update_report_t *)ReportData;
#ifdef CONSOLE_ENABLE
            dprintf("multi update=%d\n", report->lamp_count);
#endif

            for (int i = 0; i < report->lamp_count; i++)
            {
                if (report->lamp_ids[i] < DRIVER_LED_TOTAL && report->lamp_ids[i] >= 0)
                {
                    rgb_matrix_set_color(report->lamp_ids[i], report->colors[i].red, report->colors[i].green, report->colors[i].blue);
                }
            }

            _dirty = true;

            break;
        }

        case hid_lamparray_range_update_report_id:
        {
            if (hid_lamparray_auto_mode)
            {
                return;
            }

            hid_lamparray_range_update_report_t *report = (hid_lamparray_range_update_report_t *)ReportData;
#ifdef CONSOLE_ENABLE
            dprintf("range update start=%d end=%d\n", report->lamp_id_start, report->lamp_id_end);
#endif

            for (int i = report->lamp_id_start; i <= report->lamp_id_end && i < DRIVER_LED_TOTAL; i++)
            {
                rgb_matrix_set_color(i, report->color.red, report->color.green, report->color.blue);
            }

            _dirty = true;

            break;
        }

        case hid_lamparray_control_report_id:
        {
            hid_lamparray_control_report_t *report = (hid_lamparray_control_report_t *)ReportData;
            if (hid_lamparray_auto_mode && report->autonomous_mode == 0)
            {
                rgb_matrix_disable_noeeprom();
                rgb_timer = timer_read32();
                hid_lamparray_auto_mode = false;
#ifdef CONSOLE_ENABLE
                dprintf("autonomous mode=%d\n", hid_lamparray_auto_mode);
#endif
            }
            else if (!hid_lamparray_auto_mode && report->autonomous_mode == 1)
            {
                rgb_matrix_enable_noeeprom();
                rgb_timer = timer_read32();
                hid_lamparray_auto_mode = true;
#ifdef CONSOLE_ENABLE
                dprintf("autonomous mode=%d\n", hid_lamparray_auto_mode);
#endif
            }

            break;
        }
    }
}
