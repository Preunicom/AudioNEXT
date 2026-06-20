/******************************************************************************
* Copyright (C) 2023 Advanced Micro Devices, Inc. All Rights Reserved.
* SPDX-License-Identifier: MIT
******************************************************************************/
/*
 * audio_rms_main.c: Audio-RMS Lautstärkemessung
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"

// BEGIN NEW Felix Knoll
#include <stdint.h>
#include "aud_driver.h"
#include "aud_selftest_pio.h"
#include "audio_rms.h"

extern int audio_rms_test(void);
// END NEW Felix Knoll

// BEGIN NEW Nicolas Lonthoff
#include "vis_driver.h"
#include "vis_core.h"
// END NEW Nicolas Lonthoff

int main()
{
    init_platform();

    // BEGIN NEW Felix Knoll
    AUD_Data AUD_Inst;
    AUD_Data *AUD_InstPtr = &AUD_Inst;
    XStatus Status;

    Status = AUD_Init(AUD_InstPtr, AUD_BASEADDRESS);
    if (Status != XST_SUCCESS) {
        xil_printf("error during AUD_Init(). Check/Debug manually.\n\r");
    }

    // BEGIN NEW Nicolas Lonthoff
    VIS_Data VIS_Inst;
    VIS_Data *VIS_InstPtr = &VIS_Inst;

    Status = VIS_Init(VIS_InstPtr, VIS_BASEADDRESS);
    if (Status != XST_SUCCESS) {
        xil_printf("error during VIS_Init(). Check/Debug manually.\n\r");
    }
    // END NEW Nicolas Lonthoff

    ///Selftests
    AUD_TestRegisters(AUD_InstPtr);
    AUD_TestSampling(AUD_InstPtr);
    audio_rms_test();

    ///Main loop: RMS polling
    AUD_EnableSampling(AUD_InstPtr);
    rms_reset();

    /* Polling
     * 32000 Samples pro Kanal entsprechen 3 Hz Ausgaberate */
    uint32_t block_overruns = 0;
    while (1) {
        block_overruns += AUD_GetOverruns(AUD_InstPtr);

        if (AUD_LAvailable(AUD_InstPtr)) {
            int32_t l = AUD_GetL(AUD_InstPtr);
            int32_t r = 0;
            if (AUD_RAvailable(AUD_InstPtr)) {
                r = AUD_GetR(AUD_InstPtr);
            }
            rms_add(l, r);

            if (rms_block_full()) {
                VIS_Core_RenderLoudness(VIS_InstPtr, rms_value_fp72());
                if (block_overruns > 0) {
                    xil_printf("%u Overruns\n\r", block_overruns);
                }
                block_overruns = 0;
                rms_reset();
            }
        }
    }
    // END NEW Felix Knoll

    cleanup_platform();
    return 0;
}
