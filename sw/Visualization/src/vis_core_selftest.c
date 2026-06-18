
//begin edit Nicolas Lonthoff
#include "vis_core_selftest.h"
#include "xil_printf.h"


/************************** Static Helpers ***************************/

/*
 * Compare two char arrays of length len.
 * Prints the first mismatching position and returns XST_FAILURE.
 */
static XStatus check_str(const char *expected, const char *actual,
                          u8 len, const char *test_name)
{
    u8 i;
    for (i = 0; i < len; i++) {
        if (expected[i] != actual[i]) {
            xil_printf("  FAIL %s: [%u] expected 0x%02x ('%c') got 0x%02x ('%c')\n\r",
                       test_name, (unsigned)i,
                       (unsigned)(u8)expected[i], expected[i],
                       (unsigned)(u8)actual[i],   actual[i]);
            return XST_FAILURE;
        }
    }
    return XST_SUCCESS;
}

/*
 * Check a single bar-width result and print on mismatch.
 */
static XStatus check_bar_width(uint16_t rms, u8 expected, const char *label)
{
    u8 got = VIS_Core_CalcBarWidth(rms);
    if (got != expected) {
        xil_printf("  FAIL %s: rms=%u -> expected width %u, got %u\n\r",
                   label, (unsigned)rms, (unsigned)expected, (unsigned)got);
        return XST_FAILURE;
    }
    return XST_SUCCESS;
}


/************************** Test Functions ***************************/

/*
 * Unit test: VIS_Core_FormatRmsText
 *
 * Verifies the 17-character output "  Level: XXX.YY %" for boundary
 * values of the 7.2 fixed-point input.
 * No hardware required.
 */
XStatus VIS_Core_TestFormat(void)
{
    XStatus Status = XST_SUCCESS, tmp;
    char buf[18];

    xil_printf("******************************\n\r");
    xil_printf("* VIS_CORE_TESTFORMAT\n\r");
    xil_printf("******************************\n\r");

    /* All four fractional variants at integer 0 */
    VIS_Core_FormatRmsText(0, buf);
    tmp = check_str("  Level: 000.00 %", buf, 17, "rms=0 (0.00%)");
    Status |= tmp;

    VIS_Core_FormatRmsText(1, buf);
    tmp = check_str("  Level: 000.25 %", buf, 17, "rms=1 (0.25%)");
    Status |= tmp;

    VIS_Core_FormatRmsText(2, buf);
    tmp = check_str("  Level: 000.50 %", buf, 17, "rms=2 (0.50%)");
    Status |= tmp;

    VIS_Core_FormatRmsText(3, buf);
    tmp = check_str("  Level: 000.75 %", buf, 17, "rms=3 (0.75%)");
    Status |= tmp;

    /* Integer increment: 4 = 1.00% */
    VIS_Core_FormatRmsText(4, buf);
    tmp = check_str("  Level: 001.00 %", buf, 17, "rms=4 (1.00%)");
    Status |= tmp;

    /* Exact 100% */
    VIS_Core_FormatRmsText(400, buf);
    tmp = check_str("  Level: 100.00 %", buf, 17, "rms=400 (100.00%)");
    Status |= tmp;

    /* Maximum representable value: 127.75% */
    VIS_Core_FormatRmsText(511, buf);
    tmp = check_str("  Level: 127.75 %", buf, 17, "rms=511 (127.75%)");
    Status |= tmp;

    if (Status == XST_SUCCESS)
        xil_printf("VIS_CORE_TESTFORMAT passed\n\r");
    else
        xil_printf("VIS_CORE_TESTFORMAT FAILED\n\r");

    return Status;
}


/*
 * Unit test: VIS_Core_CalcBarWidth
 *
 * Verifies bar column count for key points in the 0-511 input range.
 * Expected: 100% (rms=400) fills all 80 columns; values above 100% are capped.
 * No hardware required.
 */
XStatus VIS_Core_TestBarWidth(void)
{
    XStatus Status = XST_SUCCESS, tmp;

    xil_printf("******************************\n\r");
    xil_printf("* VIS_CORE_TESTBARWIDTH\n\r");
    xil_printf("******************************\n\r");

    /* Silence */
    tmp = check_bar_width(0,   0,  "rms=0   (0%)");
    Status |= tmp;

    /* 25%: 100 * 80 / 400 = 20 */
    tmp = check_bar_width(100, 20, "rms=100 (25%)");
    Status |= tmp;

    /* 50%: 200 * 80 / 400 = 40 */
    tmp = check_bar_width(200, 40, "rms=200 (50%)");
    Status |= tmp;

    /* Just below 100%: 399 * 80 / 400 = 79 (integer division) */
    tmp = check_bar_width(399, 79, "rms=399 (~100%)");
    Status |= tmp;

    /* Exact 100%: cap triggers, returns 80 */
    tmp = check_bar_width(400, 80, "rms=400 (100%)");
    Status |= tmp;

    /* Over-range: capped at 80 */
    tmp = check_bar_width(511, 80, "rms=511 (>100%)");
    Status |= tmp;

    if (Status == XST_SUCCESS)
        xil_printf("VIS_CORE_TESTBARWIDTH passed\n\r");
    else
        xil_printf("VIS_CORE_TESTBARWIDTH FAILED\n\r");

    return Status;
}


/*
 * Integration test: VIS_Core_RenderLoudness
 *
 * Calls the full render pipeline for representative loudness values and
 * verifies it completes without hanging. Requires display hardware.
 * Visual output on screen is the final verification.
 */
XStatus VIS_Core_TestRender(VIS_Data *InstancePtr)
{
    xil_printf("******************************\n\r");
    xil_printf("* VIS_CORE_TESTRENDER\n\r");
    xil_printf("******************************\n\r");

    VIS_Core_Clear(InstancePtr);

    VIS_Core_RenderLoudness(InstancePtr, 0);
    xil_printf("  Rendered rms=0   (0.00%%  - silent, empty bar)\n\r");

    VIS_Core_RenderLoudness(InstancePtr, 200);
    xil_printf("  Rendered rms=200 (50.00%% - green bar, half width)\n\r");

    VIS_Core_RenderLoudness(InstancePtr, 240);
    xil_printf("  Rendered rms=240 (60.00%% - yellow bar threshold)\n\r");

    VIS_Core_RenderLoudness(InstancePtr, 320);
    xil_printf("  Rendered rms=320 (80.00%% - red bar threshold)\n\r");

    VIS_Core_RenderLoudness(InstancePtr, 400);
    xil_printf("  Rendered rms=400 (100.00%% - full red bar)\n\r");

    VIS_Core_RenderLoudness(InstancePtr, 511);
    xil_printf("  Rendered rms=511 (127.75%% - over-range, capped bar)\n\r");

    xil_printf("VIS_CORE_TESTRENDER passed (verify display output visually)\n\r");
    return XST_SUCCESS;
}
//end edit Nicolas Lonthoff
