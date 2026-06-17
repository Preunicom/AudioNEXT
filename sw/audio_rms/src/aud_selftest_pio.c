/***************************** Include Files *******************************/
#include "aud_driver.h"
#include "aud_driver_i.h"
#include "aud_selftest_pio.h"

/************************** Constant Definitions ***************************/


/************************** Function Definitions ***************************/

XStatus AUD_TestRegister(AUD_Data *InstancePtr, uint32_t AddrOffset,
                        uint32_t ExpectedReadRegValWriteFF,
                        uint32_t ExpectedReadRegValWrite00)
{
    XStatus Status = XST_SUCCESS;
    uint32_t val;

    AUD_mWriteReg(InstancePtr->BaseAddress, AddrOffset, 0xffffffff);
    val = AUD_mReadReg(InstancePtr->BaseAddress, AddrOffset);
    if (val != ExpectedReadRegValWriteFF) {
        xil_printf("Mismatch: A:0x%0x : Erwartet 0x%x -> Gelesen 0x%x\n\r",
                   AddrOffset, ExpectedReadRegValWriteFF, val);
        Status = XST_FAILURE;
    }

    AUD_mWriteReg(InstancePtr->BaseAddress, AddrOffset, 0x00000000);
    val = AUD_mReadReg(InstancePtr->BaseAddress, AddrOffset);
    if (val != ExpectedReadRegValWrite00) {
        xil_printf("Mismatch: A:0x%0x : Erwartet 0x%x -> Gelesen 0x%x\n\r",
                   AddrOffset, ExpectedReadRegValWrite00, val);
        Status = XST_FAILURE;
    }

    return Status;
}


XStatus AUD_TestRegisters(AUD_Data *InstancePtr)
{
    XStatus Status = XST_SUCCESS, Statustmp = XST_SUCCESS;

    xil_printf("******************************\n\r");
    xil_printf("*AUD_TESTREGISTERS\n\r");
    xil_printf("******************************\n\r");

    // IDR/VERR read-only, CTRL nur Bit 0 beschreibbar
    Statustmp = AUD_TestRegister(InstancePtr, IDR_ADDR_OFFSET, AUDIO_ID_VALUE, AUDIO_ID_VALUE);
    Status |= Statustmp;

    Statustmp = AUD_TestRegister(InstancePtr, VERR_ADDR_OFFSET, AUDIO_VERSION_VALUE, AUDIO_VERSION_VALUE);
    Status |= Statustmp;

    Statustmp = AUD_TestRegister(InstancePtr, CTRL_ADDR_OFFSET, CTRL_SEN_MASK, 0x00000000);
    Status |= Statustmp;

    if (Status == XST_SUCCESS)
        xil_printf("AUD_TESTREGISTERS erfolgreich\n\r");
    else
        xil_printf("AUD_TESTREGISTERS fehlgeschlagen\n\r");

    return Status;
}


XStatus AUD_TestSampling(AUD_Data *InstancePtr)
{
    XStatus Status = XST_SUCCESS;
    int32_t sample;
    uint32_t timeout;

    xil_printf("******************************\n\r");
    xil_printf("*AUD_TESTSAMPLING\n\r");
    xil_printf("******************************\n\r");

    AUD_EnableSampling(InstancePtr);
    if ((AUD_mReadReg(InstancePtr->BaseAddress, CTRL_ADDR_OFFSET) & CTRL_SEN_MASK) == 0) {
        xil_printf("FAIL: Sampling nicht aktiviert\n\r");
        Status = XST_FAILURE;
    }

    if (AUD_GetOverruns(InstancePtr) != 0) {
        xil_printf("FAIL: Overrun direkt nach Enable\n\r");
        Status = XST_FAILURE;
    }

    timeout = 200000u;
    while (!AUD_LAvailable(InstancePtr) && timeout > 0) timeout--;
    if (!AUD_LAvailable(InstancePtr)) {
        xil_printf("FAIL: L-Sample kam nicht an\n\r");
        Status = XST_FAILURE;
    }

    sample = AUD_GetL(InstancePtr);
    if (AUD_LAvailable(InstancePtr)) {
        xil_printf("FAIL: SLA nicht geloescht nach AUD_GetL\n\r");
        Status = XST_FAILURE;
    }
    if (sample < -(1 << 23) || sample > ((1 << 23) - 1)) {
        xil_printf("FAIL: L-Sample ausserhalb 24-Bit-Bereich: %d\n\r", (int)sample);
        Status = XST_FAILURE;
    }
    #ifdef AUD_DEBUG_P
    xil_printf("L: %d\n\r", (int)sample);
    #endif

    timeout = 200000u;
    while (!AUD_RAvailable(InstancePtr) && timeout > 0) timeout--;
    if (!AUD_RAvailable(InstancePtr)) {
        xil_printf("FAIL: R-Sample kam nicht an\n\r");
        Status = XST_FAILURE;
    }

    sample = AUD_GetR(InstancePtr);
    if (AUD_RAvailable(InstancePtr)) {
        xil_printf("FAIL: SRA nicht geloescht nach AUD_GetR\n\r");
        Status = XST_FAILURE;
    }
    if (sample < -(1 << 23) || sample > ((1 << 23) - 1)) {
        xil_printf("FAIL: R-Sample ausserhalb 24-Bit-Bereich: %d\n\r", (int)sample);
        Status = XST_FAILURE;
    }
    #ifdef AUD_DEBUG_P
    xil_printf("R: %d\n\r", (int)sample);
    #endif

    if (Status == XST_SUCCESS)
        xil_printf("AUD_TestSampling erfolgreich\n\r");
    else
        xil_printf("AUD_TestSampling fehlgeschlagen\n\r");

    return Status;
}
