/***************************** Include Files *******************************/
#include "aud_driver.h"
#include "aud_driver_i.h"
#include "aud_selftest_pio.h"

/* Sicherheits-Timeout (Iterationen) fuer das Leer-Lesen der FIFOs.
   Kein HW-Timer im BSP verfuegbar (XTime_GetTime nicht gelinkt), daher
   Iterationszaehler statt echter Sekunden. Erfolg (FIFO leer) tritt in
   Mikrosekunden ein; dieser Wert begrenzt nur den Fehlerfall.
   Bei Debug-Build (-O0) ca. 3 us/Iteration -> ~10 Mio ≈ 30 s.
   Wert bei Bedarf anpassen. */
#define AUD_DRAIN_TIMEOUT 100000u

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
    // EDIT CODE BEGIN Richard Tuch
    Statustmp = AUD_TestRegister(InstancePtr, IDR_ADDR_OFFSET, AUDIO_ID_VALUE, AUDIO_ID_VALUE);
    Status |= Statustmp;

    Statustmp = AUD_TestRegister(InstancePtr, VERR_ADDR_OFFSET, AUDIO_VERSION_VALUE, AUDIO_VERSION_VALUE);
    Status |= Statustmp;
    // EDIT CODE END Richard Tuch

    // BEGIN NEW Felix Knoll
    Statustmp = AUD_TestRegister(InstancePtr, CTRL_ADDR_OFFSET, CTRL_SEN_MASK, 0x00000000);
    Status |= Statustmp;
    // END NEW Felix Knoll

    if (Status == XST_SUCCESS)
        xil_printf("AUD_TESTREGISTERS erfolgreich\n\r");
    else
        xil_printf("AUD_TESTREGISTERS fehlgeschlagen\n\r");

    return Status;
}


// BEGIN NEW Felix Knoll
XStatus AUD_TestSampling(AUD_Data *InstancePtr)
{
    XStatus Status = XST_SUCCESS;
    int32_t sample;
    uint32_t timeout;

    xil_printf("******************************\n\r");
    xil_printf("*AUD_TESTSAMPLING\n\r");
    xil_printf("******************************\n\r");

    AUD_EnableSampling(InstancePtr);
    xil_printf("DBG: nach EnableSampling\n\r");
    if ((AUD_mReadReg(InstancePtr->BaseAddress, CTRL_ADDR_OFFSET) & CTRL_SEN_MASK) == 0) {
        xil_printf("FAIL: Sampling nicht aktiviert\n\r");
        Status = XST_FAILURE;
    }

    if (AUD_GetOverruns(InstancePtr) != 0) {
        xil_printf("FAIL: Overrun direkt nach Enable\n\r");
        Status = XST_FAILURE;
    }

    xil_printf("DBG: warte auf L-Sample...\n\r");
    timeout = 200000u;
    while (!AUD_LAvailable(InstancePtr) && timeout > 0) timeout--;
    xil_printf("DBG: L-wait fertig, timeout=%u SLA=%d\n\r",
               (unsigned)timeout, AUD_LAvailable(InstancePtr));
    if (!AUD_LAvailable(InstancePtr)) {
        xil_printf("FAIL: L-Sample kam nicht an\n\r");
        Status = XST_FAILURE;
    }

    sample = AUD_GetL(InstancePtr);
    xil_printf("DBG: nach erstem AUD_GetL, SLA=%d\n\r", AUD_LAvailable(InstancePtr));
    /* Durchgehend lesen bis Leser schneller ist als FIFO schreibt und SLA
       geloescht wird (FIFO leer). Kein Delay. Abbruch nach Timeout. */
    {
        uint32_t reads = 0u;
        uint32_t changes = 0u;
        int ever_zero = 0;
        int32_t prev = sample;
        int32_t vmin = sample, vmax = sample;
        uint32_t i;
        for (i = 0u; i < 2000u; i++) {
            if (!AUD_LAvailable(InstancePtr)) { ever_zero = 1; break; }
            sample = AUD_GetL(InstancePtr);
            reads++;
            if (sample != prev) { changes++; prev = sample; }
            if (sample < vmin) vmin = sample;
            if (sample > vmax) vmax = sample;
        }
        xil_printf("DBG: L reads=%u changes=%u SLA0=%d finalSLA=%d min=%d max=%d ovr=%u\n\r",
                   (unsigned)reads, (unsigned)changes, ever_zero,
                   AUD_LAvailable(InstancePtr), (int)vmin, (int)vmax,
                   (unsigned)AUD_GetOverruns(InstancePtr));
    }
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
    /* Durchgehend lesen bis Leser schneller ist als FIFO schreibt und SRA
       geloescht wird (FIFO leer). Kein Delay. Abbruch nach Timeout. */
    {
        uint32_t drain = AUD_DRAIN_TIMEOUT;
        while (AUD_RAvailable(InstancePtr) && drain > 0u) {
            sample = AUD_GetR(InstancePtr);
            drain--;
        }
    }
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
// END NEW Felix Knoll

