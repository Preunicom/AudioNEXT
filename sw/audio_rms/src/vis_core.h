
//begin edit Nicolas Lonthoff
#ifndef VIS_CORE_H
#define VIS_CORE_H

#include <stdint.h>
#include "vis_driver.h"

/************************** Constant Definitions ***************************/
/* Display dimensions */
#define VIS_CORE_COLS  80U
#define VIS_CORE_ROWS  30U

/* Color presets (4-bit per channel, 0-15) */
#define VIS_CORE_CR_WHITE    15U
#define VIS_CORE_CG_WHITE    15U
#define VIS_CORE_CB_WHITE    15U

#define VIS_CORE_CR_GREEN     0U
#define VIS_CORE_CG_GREEN    15U
#define VIS_CORE_CB_GREEN     0U

#define VIS_CORE_CR_YELLOW   15U
#define VIS_CORE_CG_YELLOW   15U
#define VIS_CORE_CB_YELLOW    0U

#define VIS_CORE_CR_RED      15U
#define VIS_CORE_CG_RED       0U
#define VIS_CORE_CB_RED       0U

/* Layout: row indices for each display element */
#define VIS_CORE_ROW_HEADER   1U
#define VIS_CORE_ROW_TEXT    12U
#define VIS_CORE_ROW_BAR     14U
#define VIS_CORE_ROW_SCALE   15U

/* Project title shown in the top-left corner of the display */
#define VIS_CORE_TITLE      "  AudioNEXT - RMS Loudness Meter  "
#define VIS_CORE_TITLE_LEN  34U

/* RMS input: 7.2 fixed-point (bits 8:2 = integer 0-127, bits 1:0 = frac .00/.25/.50/.75)
 * Represents loudness in %; 100.00% is encoded as 400 (= 100 * 4). */
#define VIS_CORE_RMS_100PCT  400U

/* Bar colour thresholds (in 7.2 fp: 60% = 240, 80% = 320) */
#define VIS_CORE_RMS_YELLOW_THR  240U
#define VIS_CORE_RMS_RED_THR     320U


/************************** Function Prototypes ****************************/
/* Clear all 80x30 cells to space and wait for one full frame */
void VIS_Core_Clear(VIS_Data *InstancePtr);

/* Render loudness as text value + colour bar; syncs to display frame */
void VIS_Core_RenderLoudness(VIS_Data *InstancePtr, uint16_t rms_7p2);

/* Testable pure-logic helpers (no hardware access) */
void VIS_Core_FormatRmsText(uint16_t rms_7p2, char *buf);
u8   VIS_Core_CalcBarWidth(uint16_t rms_7p2);


#endif // VIS_CORE_H
//end edit Nicolas Lonthoff
