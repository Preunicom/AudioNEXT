
//begin edit Nicolas Lonthoff
#include "vis_core.h"

/************************** Constant Definitions ***************************/

/* Fractional digit pairs for 7.2 fixed-point: 0->.00  1->.25  2->.50  3->.75 */
static const char FRAC_TAB[4][2] = {
    {'0', '0'},
    {'2', '5'},
    {'5', '0'},
    {'7', '5'}
};

/* Ring buffer: one RMS sample per display column, oldest at head, newest before head */
static uint16_t s_history[VIS_CORE_COLS];
static uint16_t s_history_sum = 0;
static u8       s_history_head = 0;


/************************** Static Helper Functions ***************************/

/*
 * Return bar height in rows (0-VIS_CORE_ROWS) for a given rms_7p2 value.
 * 100% (rms=400) maps to VIS_CORE_ROWS; values above 100% are capped.
 */
static u8 calc_bar_height(uint16_t rms_7p2)
{
    if (rms_7p2 >= VIS_CORE_RMS_100PCT)
        return (u8)VIS_CORE_ROWS;
    return (u8)((u32)rms_7p2 * VIS_CORE_ROWS / VIS_CORE_RMS_100PCT);
}

/*
 * Write a string of exactly len characters starting at (x, y) with the given colour.
 * Caller must ensure x + len <= VIS_CORE_COLS.
 */
static void write_str(VIS_Data *InstancePtr, u8 x, u8 y,
                      const char *str, u8 len,
                      u8 cr, u8 cg, u8 cb)
{
    u8 i;
    for (i = 0; i < len; i++) {
        VIS_WriteChar(InstancePtr, x + i, y, (u8)str[i], cr, cg, cb);
    }
}


/*
 * Return bar cell color based on absolute row position.
 * Top third (rows 0..9) -> red, middle (rows 10..19) -> yellow, bottom (rows 20..29) -> green.
 */
static void bar_color(u8 y, u8 *cr, u8 *cg, u8 *cb)
{
    if (y < VIS_CORE_ROWS / 3U) {
        *cr = VIS_CORE_CR_RED;    *cg = VIS_CORE_CG_RED;    *cb = VIS_CORE_CB_RED;
    } else if (y < (2U * VIS_CORE_ROWS / 3U)) {
        *cr = VIS_CORE_CR_YELLOW; *cg = VIS_CORE_CG_YELLOW; *cb = VIS_CORE_CB_YELLOW;
    } else {
        *cr = VIS_CORE_CR_GREEN;  *cg = VIS_CORE_CG_GREEN;  *cb = VIS_CORE_CB_GREEN;
    }
}


/************************** Public Functions ***************************/

/*
 * Format rms_7p2 into buf as "  Level: XXX.YY %" (17 chars).
 * buf must be at least 18 bytes.
 * Public for unit testing (no hardware access).
 */
void VIS_Core_FormatRmsText(uint16_t rms_7p2, char *buf)
{
    u8 integer = (u8)(rms_7p2 >> 2);
    u8 frac    = (u8)(rms_7p2 & 0x3U);

    buf[0]  = ' ';  buf[1]  = ' ';
    buf[2]  = 'L';  buf[3]  = 'e';  buf[4]  = 'v';
    buf[5]  = 'e';  buf[6]  = 'l';  buf[7]  = ':';  buf[8]  = ' ';
    buf[9]  = '0' + (char)(integer / 100);
    buf[10] = '0' + (char)((integer / 10) % 10);
    buf[11] = '0' + (char)(integer % 10);
    buf[12] = '.';
    buf[13] = FRAC_TAB[frac][0];
    buf[14] = FRAC_TAB[frac][1];
    buf[15] = ' ';  buf[16] = '%';
    buf[17] = '\0';
}


/*
 * Calculate bar width (0-80) from rms_7p2.
 * 100% (rms=400) -> 80 columns; capped at 80 above 100%.
 * Public for unit testing (no hardware access).
 */
u8 VIS_Core_CalcBarWidth(uint16_t rms_7p2)
{
    if (rms_7p2 >= VIS_CORE_RMS_100PCT)
        return (u8)VIS_CORE_COLS;
    return (u8)((u32)rms_7p2 * VIS_CORE_COLS / VIS_CORE_RMS_100PCT);
}


void VIS_Core_Clear(VIS_Data *InstancePtr)
{
    u8 x, y;

    for (y = 0; y < VIS_CORE_ROWS; y++) {
        for (x = 0; x < VIS_CORE_COLS; x++) {
            VIS_WriteChar(InstancePtr, x, y, (u8)' ',
                          VIS_CORE_CR_WHITE, VIS_CORE_CG_WHITE, VIS_CORE_CB_WHITE);
        }
    }

    /* Reset history so the next render starts from a blank slate */
    for (x = 0; x < VIS_CORE_COLS; x++)
        s_history[x] = 0U;
    s_history_head = 0U;

    /* Write static content that doesn't change between renders */
    write_str(InstancePtr, 0, VIS_CORE_ROW_HEADER,
              VIS_CORE_TITLE, VIS_CORE_TITLE_LEN,
              VIS_CORE_CR_WHITE, VIS_CORE_CG_WHITE, VIS_CORE_CB_WHITE);

    write_str(InstancePtr, 0,  VIS_CORE_ROW_TEXT, "  Level: ", 9U,
              VIS_CORE_CR_WHITE, VIS_CORE_CG_WHITE, VIS_CORE_CB_WHITE);
    VIS_WriteChar(InstancePtr, 12, VIS_CORE_ROW_TEXT, (u8)'.',
                  VIS_CORE_CR_WHITE, VIS_CORE_CG_WHITE, VIS_CORE_CB_WHITE);
    write_str(InstancePtr, 15, VIS_CORE_ROW_TEXT, " %", 2U,
              VIS_CORE_CR_WHITE, VIS_CORE_CG_WHITE, VIS_CORE_CB_WHITE);

    VIS_PollFDP(InstancePtr);
}


/*
 * Scrolling bar-chart renderer.
 *
 * Each call appends rms_7p2 to a ring buffer and redraws all 80 columns.
 * Column x=0 holds the oldest sample, x=79 the newest.
 * Each column is a vertical bar growing from the bottom; the bar character
 * is coloured green (bottom third), yellow (middle third), or red (top third).
 * Cells in VIS_CORE_ROW_HEADER (cols 0..TITLE_LEN-1) and VIS_CORE_ROW_TEXT
 * (cols 0..16) are skipped — their static content was written once in VIS_Core_Clear.
 * Only the five digit characters of the percentage value are updated each call.
 */
void VIS_Core_RenderLoudness(VIS_Data *InstancePtr, uint16_t rms_7p2)
{
    // Wait for previous frame to be fully displayed before starting to write the next one, otherwise we get tearing artifacts and/or dropped frames.
    VIS_PollFDP(InstancePtr);

    u8 x, y;

    s_history[s_history_head] = rms_7p2;
    s_history_sum += rms_7p2;
    s_history_head = (u8)((s_history_head + 1U) % VIS_CORE_COLS);

    for (x = 0; x < VIS_CORE_COLS; x++) {
        u8 idx   = (u8)(((u16)s_history_head + x) % VIS_CORE_COLS);
        u8 bar_h = calc_bar_height(s_history[idx]);

        for (y = 0; y < VIS_CORE_ROWS; y++) {
            /* Skip cells reserved for the static title and text label */
            if ((y == VIS_CORE_ROW_HEADER && x < VIS_CORE_TITLE_LEN) ||
                (y == VIS_CORE_ROW_TEXT   && x < 17U))
                continue;

            if ((VIS_CORE_ROWS - 1U - y) < bar_h) {
                u8 cr, cg, cb;
                bar_color(y, &cr, &cg, &cb);
                VIS_WriteChar(InstancePtr, x, y, (u8)'|', cr, cg, cb);
            } else {
                VIS_WriteChar(InstancePtr, x, y, (u8)' ',
                              VIS_CORE_CR_WHITE, VIS_CORE_CG_WHITE, VIS_CORE_CB_WHITE);
            }
        }
    }


    /* Update the five dynamic digit characters of the percentage value every 8 frames */
    if (s_history_head % 8 == 0U) {

        // Divide s_history_sum by 8 through 3 right shifts to get the average RMS value over the last 8 samples.
        // Then reset s_history_sum to 0 for the next 8 samples.
        uint16_t average_rms_7p2 = (uint16_t)(s_history_sum >> 3);
        s_history_sum = 0U;

        u8 integer_part = (u8)(average_rms_7p2 >> 2);
        u8 frac_idx     = (u8)(average_rms_7p2 & 0x3U);

        VIS_WriteChar(InstancePtr,  9, VIS_CORE_ROW_TEXT,
                    (u8)('0' + integer_part / 100),
                    VIS_CORE_CR_WHITE, VIS_CORE_CG_WHITE, VIS_CORE_CB_WHITE);
        VIS_WriteChar(InstancePtr, 10, VIS_CORE_ROW_TEXT,
                    (u8)('0' + (integer_part / 10) % 10),
                    VIS_CORE_CR_WHITE, VIS_CORE_CG_WHITE, VIS_CORE_CB_WHITE);
        VIS_WriteChar(InstancePtr, 11, VIS_CORE_ROW_TEXT,
                    (u8)('0' + integer_part % 10),
                    VIS_CORE_CR_WHITE, VIS_CORE_CG_WHITE, VIS_CORE_CB_WHITE);
        VIS_WriteChar(InstancePtr, 13, VIS_CORE_ROW_TEXT,
                    (u8)FRAC_TAB[frac_idx][0],
                    VIS_CORE_CR_WHITE, VIS_CORE_CG_WHITE, VIS_CORE_CB_WHITE);
        VIS_WriteChar(InstancePtr, 14, VIS_CORE_ROW_TEXT,
                    (u8)FRAC_TAB[frac_idx][1],
                    VIS_CORE_CR_WHITE, VIS_CORE_CG_WHITE, VIS_CORE_CB_WHITE);
    }
}
//end edit Nicolas Lonthoff
