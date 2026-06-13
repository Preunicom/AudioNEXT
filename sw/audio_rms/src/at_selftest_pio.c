/***************************** Include Files *******************************/
#include "at_driver.h"
#include "at_driver_i.h"
#include "at_selftest_pio.h"

/************************** Constant Definitions ***************************/


/************************** Function Definitions ***************************/

XStatus AT_TestRegister(AT_Data *InstancePtr, uint32_t AddrOffset,
                        uint32_t ExpectedReadRegValWriteFF,
                        uint32_t ExpectedReadRegValWrite00)
{
    XStatus Status = XST_SUCCESS;
    uint32_t val;

    AT_mWriteReg(InstancePtr->BaseAddress, AddrOffset, 0xffffffff);
    val = AT_mReadReg(InstancePtr->BaseAddress, AddrOffset);
    if (val != ExpectedReadRegValWriteFF) {
        xil_printf("Mismatch: A:0x%0x : Erwartet 0x%x -> Gelesen 0x%x\n\r",
                   AddrOffset, ExpectedReadRegValWriteFF, val);
        Status = XST_FAILURE;
    }

    AT_mWriteReg(InstancePtr->BaseAddress, AddrOffset, 0x00000000);
    val = AT_mReadReg(InstancePtr->BaseAddress, AddrOffset);
    if (val != ExpectedReadRegValWrite00) {
        xil_printf("Mismatch: A:0x%0x : Erwartet 0x%x -> Gelesen 0x%x\n\r",
                   AddrOffset, ExpectedReadRegValWrite00, val);
        Status = XST_FAILURE;
    }

    return Status;
}


XStatus AT_TestRegisters(AT_Data *InstancePtr)
{
    XStatus Status = XST_SUCCESS, Statustmp = XST_SUCCESS;

    xil_printf("******************************\n\r");
    xil_printf("*AT_TESTREGISTERS\n\r");
    xil_printf("******************************\n\r");

    // IDR/VERR read-only, CTRL nur Bit 0 beschreibbar
    Statustmp = AT_TestRegister(InstancePtr, IDR_ADDR_OFFSET, AUDIO_ID_VALUE, AUDIO_ID_VALUE);
    Status |= Statustmp;

    Statustmp = AT_TestRegister(InstancePtr, VERR_ADDR_OFFSET, AUDIO_VERSION_VALUE, AUDIO_VERSION_VALUE);
    Status |= Statustmp;

    Statustmp = AT_TestRegister(InstancePtr, CTRL_ADDR_OFFSET, CTRL_SEN_MASK, 0x00000000);
    Status |= Statustmp;

    if (Status == XST_SUCCESS)
        xil_printf("AT_TESTREGISTERS erfolgreich\n\r");
    else
        xil_printf("AT_TESTREGISTERS fehlgeschlagen\n\r");

    return Status;
}


XStatus AT_TestSampling(AT_Data *InstancePtr)
{
    XStatus Status = XST_SUCCESS;
    int32_t sample;
    uint32_t timeout;

    xil_printf("******************************\n\r");
    xil_printf("*AT_TESTSAMPLING\n\r");
    xil_printf("******************************\n\r");

    AT_EnableSampling(InstancePtr);
    if ((AT_mReadReg(InstancePtr->BaseAddress, CTRL_ADDR_OFFSET) & CTRL_SEN_MASK) == 0) {
        xil_printf("FAIL: Sampling nicht aktiviert\n\r");
        Status = XST_FAILURE;
    }

    if (AT_GetOverruns(InstancePtr) != 0) {
        xil_printf("FAIL: Overrun direkt nach Enable\n\r");
        Status = XST_FAILURE;
    }

    timeout = 200000u;
    while (!AT_LAvailable(InstancePtr) && timeout > 0) timeout--;
    if (!AT_LAvailable(InstancePtr)) {
        xil_printf("FAIL: L-Sample kam nicht an\n\r");
        Status = XST_FAILURE;
    }

    sample = AT_GetL(InstancePtr);
    if (AT_LAvailable(InstancePtr)) {
        xil_printf("FAIL: SLA nicht geloescht nach AT_GetL\n\r");
        Status = XST_FAILURE;
    }
    if (sample < -(1 << 23) || sample > ((1 << 23) - 1)) {
        xil_printf("FAIL: L-Sample ausserhalb 24-Bit-Bereich: %d\n\r", (int)sample);
        Status = XST_FAILURE;
    }
    #ifdef AT_DEBUG_P
    xil_printf("L: %d\n\r", (int)sample);
    #endif

    timeout = 200000u;
    while (!AT_RAvailable(InstancePtr) && timeout > 0) timeout--;
    if (!AT_RAvailable(InstancePtr)) {
        xil_printf("FAIL: R-Sample kam nicht an\n\r");
        Status = XST_FAILURE;
    }

    sample = AT_GetR(InstancePtr);
    if (AT_RAvailable(InstancePtr)) {
        xil_printf("FAIL: SRA nicht geloescht nach AT_GetR\n\r");
        Status = XST_FAILURE;
    }
    if (sample < -(1 << 23) || sample > ((1 << 23) - 1)) {
        xil_printf("FAIL: R-Sample ausserhalb 24-Bit-Bereich: %d\n\r", (int)sample);
        Status = XST_FAILURE;
    }
    #ifdef AT_DEBUG_P
    xil_printf("R: %d\n\r", (int)sample);
    #endif

    if (Status == XST_SUCCESS)
        xil_printf("AT_TestSampling erfolgreich\n\r");
    else
        xil_printf("AT_TestSampling fehlgeschlagen\n\r");

    return Status;
}
