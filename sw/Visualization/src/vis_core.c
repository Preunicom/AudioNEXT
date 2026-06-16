
//begin edit Nicolas Lonthoff
#include "vis_core.h"

/************************** Constant Definitions ***************************/
static const char HDR_STR[] = "  AudioNEXT - RMS Loudness Meter";
#define HDR_LEN 32U

/* Fractional digit pairs for 7.2 fixed-point: 0->.00  1->.25  2->.50  3->.75 */
static const char FRAC_TAB[4][2] = {
    {'0', '0'},
    {'2', '5'},
    {'5', '0'},
    {'7', '5'}
};


/************************** Static Helper Functions ***************************/

/*
 * Write exactly VIS_CORE_COLS characters on one row.
 * Uses the first str_len chars from str, then pads the rest with spaces.
 */
static void write_row(VIS_Data *inst, u8 row, const char *str, u8 str_len,
                      u8 cr, u8 cg, u8 cb)
{
    u8 x;
    for (x = 0; x < VIS_CORE_COLS; x++) {
        u8 c = (x < str_len) ? (u8)str[x] : (u8)' ';
        VIS_WriteChar(inst, x, row, c, cr, cg, cb);
    }
}


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


/*
 * Write the bar row: bar_width filled '|' cells, rest empty ' '.
 * Colour: green < 60%, yellow 60-80%, red >= 80%.
 * Bar maps 100% (rms_7p2=400) to 80 columns.
 */
static void write_bar(VIS_Data *inst, u8 row, uint16_t rms_7p2)
{
    u8 bar_width;
    u8 cr, cg, cb;
    u8 x;

    bar_width = VIS_Core_CalcBarWidth(rms_7p2);

    if (rms_7p2 >= VIS_CORE_RMS_RED_THR) {
        cr = VIS_CORE_CR_RED;    cg = VIS_CORE_CG_RED;    cb = VIS_CORE_CB_RED;
    } else if (rms_7p2 >= VIS_CORE_RMS_YELLOW_THR) {
        cr = VIS_CORE_CR_YELLOW; cg = VIS_CORE_CG_YELLOW; cb = VIS_CORE_CB_YELLOW;
    } else {
        cr = VIS_CORE_CR_GREEN;  cg = VIS_CORE_CG_GREEN;  cb = VIS_CORE_CB_GREEN;
    }

    for (x = 0; x < VIS_CORE_COLS; x++) {
        u8 c = (x < bar_width) ? (u8)'|' : (u8)' ';
        VIS_WriteChar(inst, x, row, c, cr, cg, cb);
    }
}


/*
 * Write the scale row: "0%" at the left edge, "100%" at the right edge,
 * spaces in between.
 */
static void write_scale(VIS_Data *inst, u8 row)
{
    static const char LEFT[]  = "0%";
    static const char RIGHT[] = "100%";
    u8 x;

    for (x = 0; x < VIS_CORE_COLS; x++) {
        u8 c;
        if (x < 2U) {
            c = (u8)LEFT[x];
        } else if (x >= (VIS_CORE_COLS - 4U)) {    /* last 4 cols: "100%" */
            c = (u8)RIGHT[x - (VIS_CORE_COLS - 4U)];
        } else {
            c = (u8)' ';
        }
        VIS_WriteChar(inst, x, row, c,
                      VIS_CORE_CR_WHITE, VIS_CORE_CG_WHITE, VIS_CORE_CB_WHITE);
    }
}


/************************** Public Functions ***************************/

void VIS_Core_Clear(VIS_Data *InstancePtr)
{
    u8 x, y;
    for (y = 0; y < VIS_CORE_ROWS; y++) {
        for (x = 0; x < VIS_CORE_COLS; x++) {
            VIS_WriteChar(InstancePtr, x, y, (u8)' ',
                          VIS_CORE_CR_WHITE, VIS_CORE_CG_WHITE, VIS_CORE_CB_WHITE);
        }
    }
    VIS_PollFDP(InstancePtr);
}


void VIS_Core_RenderLoudness(VIS_Data *InstancePtr, uint16_t rms_7p2)
{
    char text_buf[18];

    write_row(InstancePtr, VIS_CORE_ROW_HEADER,
              HDR_STR, HDR_LEN,
              VIS_CORE_CR_WHITE, VIS_CORE_CG_WHITE, VIS_CORE_CB_WHITE);

    VIS_Core_FormatRmsText(rms_7p2, text_buf);
    write_row(InstancePtr, VIS_CORE_ROW_TEXT,
              text_buf, 17U,
              VIS_CORE_CR_WHITE, VIS_CORE_CG_WHITE, VIS_CORE_CB_WHITE);

    write_bar(InstancePtr, VIS_CORE_ROW_BAR, rms_7p2);

    write_scale(InstancePtr, VIS_CORE_ROW_SCALE);

    VIS_PollFDP(InstancePtr);
}
//end edit Nicolas Lonthoff
