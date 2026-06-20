#include "vis_selftest.h"
#include "vis_driver_i.h"
#include "vis_driver_int.h"
#include "xil_printf.h"


/*****************************************************************************
 * PIO register selftests (originally vis_selftest_pio.c)
 *****************************************************************************/

XStatus VIS_TestRegister(VIS_Data *InstancePtr, uint32_t AddrOffset,
                         uint32_t ExspectedReadRegValWriteFF,
                         uint32_t ExspectedReadRegValWrite00)
{
    XStatus Status = XST_SUCCESS;
    uint32_t val;

    VIS_mWriteReg(InstancePtr->BaseAddress, AddrOffset, 0xffffffff);
    val = VIS_mReadReg(InstancePtr->BaseAddress, AddrOffset);
    if (val != ExspectedReadRegValWriteFF) {
        xil_printf("Value mismatch: A:0x%0x : Expected 0x%x -> Got 0x%x\n\r",
                   AddrOffset, ExspectedReadRegValWriteFF, val);
        Status = XST_FAILURE;
    }

    VIS_mWriteReg(InstancePtr->BaseAddress, AddrOffset, 0x00000000);
    val = VIS_mReadReg(InstancePtr->BaseAddress, AddrOffset);
    if (val != ExspectedReadRegValWrite00) {
        xil_printf("Value mismatch: A:0x%0x : Expected 0x%x -> Got 0x%x\n\r",
                   AddrOffset, ExspectedReadRegValWrite00, val);
        Status = XST_FAILURE;
    }

    return Status;
}


XStatus VIS_TestRegisters(VIS_Data *InstancePtr)
{
    XStatus Status = XST_SUCCESS, Statustmp = XST_SUCCESS;
    uint32_t expectedval;

    xil_printf("******************************\n\r");
    xil_printf("*VIS_TESTREGISTERS\n\r");
    xil_printf("******************************\n\r");

    Statustmp = VIS_TestRegister(InstancePtr, GCSR_ADDR_OFFSET, 0x00000000, 0x00000000);
    Status |= Statustmp;
    expectedval = GIER_GIE_MASK;
    Statustmp = VIS_TestRegister(InstancePtr, GIER_ADDR_OFFSET, expectedval, 0x00000000);
    Status |= Statustmp;
    expectedval = IPIER_FIE_MASK;
    Statustmp = VIS_TestRegister(InstancePtr, IPIER_ADDR_OFFSET, expectedval, 0x00000000);
    Status |= Statustmp;
    Statustmp = VIS_TestRegister(InstancePtr, IPISR_ADDR_OFFSET, 0x00000000, 0x00000000); // W1C
    Status |= Statustmp;
    Statustmp = VIS_TestRegister(InstancePtr, IDR_ADDR_OFFSET, 0x0000D15C, 0x0000D15C);
    Status |= Statustmp;
    Statustmp = VIS_TestRegister(InstancePtr, VERR_ADDR_OFFSET, 0x00000001, 0x00000001);
    Status |= Statustmp;
    expectedval = CTRL_VEN_MASK | CTRL_WD_MASK;
    Statustmp = VIS_TestRegister(InstancePtr, CTRL_ADDR_OFFSET, expectedval, 0x00000000);
    Status |= Statustmp;
    Statustmp = VIS_TestRegister(InstancePtr, STATUS_ADDR_OFFSET, 0x00000000, 0x00000000);
    Status |= Statustmp;
    expectedval = ADDRR_XA_MASK | ADDRR_YA_MASK;
    Statustmp = VIS_TestRegister(InstancePtr, ADDRR_ADDR_OFFSET, expectedval, 0x00000000);
    Status |= Statustmp;
    expectedval = VDATR_CHAR_MASK;
    Statustmp = VIS_TestRegister(InstancePtr, VDATR_ADDR_OFFSET, expectedval, 0x00000000);
    Status |= Statustmp;
    expectedval = COLR_COL_MASK;
    Statustmp = VIS_TestRegister(InstancePtr, COLR_ADDR_OFFSET, expectedval, 0x00000000);
    Status |= Statustmp;

    if (Status == XST_SUCCESS)
        xil_printf("VIS_TESTREGISTERS was successful\n\r");
    else
        xil_printf("VIS_TESREGISTERS failed\n\r");

    return Status;
}


/*****************************************************************************
 * Interrupt selftest handler (originally vis_selftest_int.c)
 *****************************************************************************/

void VIS_AppHandler_SelfTest(void *CallBackRef)
{
    (void)CallBackRef;
    /* Application-specific handler called by VIS_InterruptHandler — no-op */
}


/*****************************************************************************
 * Core selftests (originally vis_core_selftest.c)
 *****************************************************************************/

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


XStatus VIS_Core_TestFormat(void)
{
    XStatus Status = XST_SUCCESS, tmp;
    char buf[18];

    xil_printf("******************************\n\r");
    xil_printf("* VIS_CORE_TESTFORMAT\n\r");
    xil_printf("******************************\n\r");

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

    VIS_Core_FormatRmsText(4, buf);
    tmp = check_str("  Level: 001.00 %", buf, 17, "rms=4 (1.00%)");
    Status |= tmp;

    VIS_Core_FormatRmsText(400, buf);
    tmp = check_str("  Level: 100.00 %", buf, 17, "rms=400 (100.00%)");
    Status |= tmp;

    VIS_Core_FormatRmsText(511, buf);
    tmp = check_str("  Level: 127.75 %", buf, 17, "rms=511 (127.75%)");
    Status |= tmp;

    if (Status == XST_SUCCESS)
        xil_printf("VIS_CORE_TESTFORMAT passed\n\r");
    else
        xil_printf("VIS_CORE_TESTFORMAT FAILED\n\r");

    return Status;
}


XStatus VIS_Core_TestBarWidth(void)
{
    XStatus Status = XST_SUCCESS, tmp;

    xil_printf("******************************\n\r");
    xil_printf("* VIS_CORE_TESTBARWIDTH\n\r");
    xil_printf("******************************\n\r");

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
