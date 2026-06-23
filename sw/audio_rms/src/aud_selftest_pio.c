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

/* Testet ein einzelnes Register: schreibt 0xFFFFFFFF und 0x00000000 und
   vergleicht den gelesenen Wert mit dem erwarteten. Fuer read-only Register
   muessen beide Erwartungswerte den konstanten Registerinhalt enthalten. */
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


/* Testet IDR (ID), VERR (Version) und CTRL (Sampling Enable) Register
   auf korrekte Lese-/Schreibbarkeit. IDR und VERR sind read-only und
   muessen konstante Werte liefern. CTRL erlaubt nur Bit 0 (SEN). */
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

    // BEGIN NEW Richard Tuch / Felix Knoll
    Statustmp = AUD_TestRegister(InstancePtr, CTRL_ADDR_OFFSET, CTRL_SEN_MASK, 0x00000000);
    Status |= Statustmp;
    // END NEW Richard Tuch / Felix Knoll

    if (Status == XST_SUCCESS)
        xil_printf("AUD_TESTREGISTERS erfolgreich\n\r");
    else
        xil_printf("AUD_TESTREGISTERS fehlgeschlagen\n\r");

    return Status;
}


// BEGIN NEW Richard Tuch / Felix Knoll
/* Aktiviert das Sampling (SEN-Bit im CTRL-Register) und prueft:
   1. Ob das SEN-Bit nach dem Schreiben tatsaechlich gesetzt ist.
   2. Ob keine Data-Overruns (DOL/DOR) direkt nach dem Enable vorliegen.
   Voraussetzung: Sampling war vorher deaktiviert. */
XStatus AUD_TestSamplingEnable(AUD_Data *InstancePtr)
{
    XStatus Status = XST_SUCCESS;

    xil_printf("******************************\n\r");
    xil_printf("*AUD_TESTSAMPLINGENABLE\n\r");
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

    if (Status == XST_SUCCESS)
        xil_printf("AUD_TestSamplingEnable erfolgreich\n\r");
    else
        xil_printf("AUD_TestSamplingEnable fehlgeschlagen\n\r");

    return Status;
}


/* Testet den linken Audio-Kanal:
   1. Wartet bis SLA (Sample Left Available) im STATUS-Register gesetzt wird.
   2. Liest das erste L-Sample ueber AUD_GetL (ADATLR-Register).
   3. Drainet den FIFO: liest solange weitere Samples bis SLA geloescht ist.
   4. Prueft ob SLA nach dem Drain tatsaechlich 0 ist.
   5. Prueft ob der Sample-Wert im gueltigen 24-Bit-Bereich liegt.
   Voraussetzung: Sampling muss vorher aktiviert sein (AUD_TestSamplingEnable). */
XStatus AUD_TestSamplingL(AUD_Data *InstancePtr)
{
    XStatus Status = XST_SUCCESS;
    int32_t sample;
    uint32_t timeout;

    xil_printf("******************************\n\r");
    xil_printf("*AUD_TESTSAMPLINGL\n\r");
    xil_printf("******************************\n\r");

    timeout = 200000u;
    while (!AUD_LAvailable(InstancePtr) && timeout > 0) timeout--;
    if (!AUD_LAvailable(InstancePtr)) {
        xil_printf("FAIL: L-Sample kam nicht an\n\r");
        Status = XST_FAILURE;
    }

    sample = AUD_GetL(InstancePtr);
    {
        uint32_t drain = AUD_DRAIN_TIMEOUT;
        while (AUD_LAvailable(InstancePtr) && drain > 0u) {
            sample = AUD_GetL(InstancePtr);
            drain--;
        }
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

    if (Status == XST_SUCCESS)
        xil_printf("AUD_TestSamplingL erfolgreich\n\r");
    else
        xil_printf("AUD_TestSamplingL fehlgeschlagen\n\r");

    return Status;
}


/* Testet den rechten Audio-Kanal:
   1. Wartet bis SRA (Sample Right Available) im STATUS-Register gesetzt wird.
   2. Liest das erste R-Sample ueber AUD_GetR (ADATRR-Register).
   3. Drainet den FIFO: liest solange weitere Samples bis SRA geloescht ist.
   4. Prueft ob SRA nach dem Drain tatsaechlich 0 ist.
   5. Prueft ob der Sample-Wert im gueltigen 24-Bit-Bereich liegt.
   Voraussetzung: Sampling muss vorher aktiviert sein (AUD_TestSamplingEnable). */
XStatus AUD_TestSamplingR(AUD_Data *InstancePtr)
{
    XStatus Status = XST_SUCCESS;
    int32_t sample;
    uint32_t timeout;

    xil_printf("******************************\n\r");
    xil_printf("*AUD_TESTSAMPLINGR\n\r");
    xil_printf("******************************\n\r");

    timeout = 200000u;
    while (!AUD_RAvailable(InstancePtr) && timeout > 0) timeout--;
    if (!AUD_RAvailable(InstancePtr)) {
        xil_printf("FAIL: R-Sample kam nicht an\n\r");
        Status = XST_FAILURE;
    }

    sample = AUD_GetR(InstancePtr);
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
        xil_printf("AUD_TestSamplingR erfolgreich\n\r");
    else
        xil_printf("AUD_TestSamplingR fehlgeschlagen\n\r");

    return Status;
}
// END NEW Richard Tuch / Felix Knoll

