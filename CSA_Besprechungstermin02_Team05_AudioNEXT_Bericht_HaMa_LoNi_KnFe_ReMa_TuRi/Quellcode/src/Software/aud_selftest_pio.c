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
#define AUD_DRAIN_TIMEOUT 200000u

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


// BEGIN NEW Felix Knoll
/* Aktiviert Sampling und prüft: SEN-Bit ist gesetzt und
   keine Overruns direkt nach dem Enable. */
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


/* Prüft linken Kanal: Sample kommt an, SLA wird nach Auslesen gelöscht,
   Wert liegt im 24-Bit-Bereich. */
XStatus AUD_TestSamplingL(AUD_Data *InstancePtr)
{
    XStatus Status = XST_SUCCESS;
    int32_t sample;
    uint32_t timeout;

    xil_printf("******************************\n\r");
    xil_printf("*AUD_TESTSAMPLINGL\n\r");
    xil_printf("******************************\n\r");

    timeout = AUD_DRAIN_TIMEOUT;
    while (!AUD_LAvailable(InstancePtr) && timeout > 0) timeout--;
    if (timeout == 0u) {
        xil_printf("FAIL: L-Sample kam nicht an\n\r");
        Status = XST_FAILURE;
    }

    sample = AUD_GetL(InstancePtr);
    timeout = AUD_DRAIN_TIMEOUT;
    while (AUD_LAvailable(InstancePtr) && timeout > 0u) {
        sample = AUD_GetL(InstancePtr);
        timeout--;
    }
    
    if (timeout == 0u) {
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


/* Prüft rechten Kanal: Sample kommt an, SRA wird nach Auslesen gelöscht,
   Wert liegt im 24-Bit-Bereich. */
XStatus AUD_TestSamplingR(AUD_Data *InstancePtr)
{
    XStatus Status = XST_SUCCESS;
    int32_t sample;
    uint32_t timeout;

    xil_printf("******************************\n\r");
    xil_printf("*AUD_TESTSAMPLINGR\n\r");
    xil_printf("******************************\n\r");

    timeout = AUD_DRAIN_TIMEOUT;
    while (!AUD_RAvailable(InstancePtr) && timeout > 0) timeout--;
    if (timeout == 0u) {
        xil_printf("FAIL: R-Sample kam nicht an\n\r");
        Status = XST_FAILURE;
    }

    sample = AUD_GetR(InstancePtr);
    timeout = AUD_DRAIN_TIMEOUT;
    while (AUD_RAvailable(InstancePtr) && timeout > 0u) {
        sample = AUD_GetR(InstancePtr);
        timeout--;
    }
    
    if (timeout == 0u) {
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
// END NEW Felix Knoll

