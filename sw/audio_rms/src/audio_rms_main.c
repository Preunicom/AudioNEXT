/******************************************************************************
* Copyright (C) 2023 Advanced Micro Devices, Inc. All Rights Reserved.
* SPDX-License-Identifier: MIT
******************************************************************************/
/*
 * audio_rms_main.c: Audio-RMS Lautstärkemessung
 *
 * STDOUT is AXI Uartlite (0x40600000), baud fixed in HW design to
 * 115200 8N1 (XPAR_AXI_UARTLITE_0_BAUDRATE=0x1c200). Set the serial
 * terminal to 115200 8N1, no flow control. init_uart() does nothing
 * for uartlite since the baud rate is not software-configurable.
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#define __MICROBLAZE__

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"

#include "xil_cache.h"

// BEGIN NEW Felix Knoll
#include <stdint.h>
#include "aud_driver.h"
#include "aud_selftest_pio.h"
#include "audio_rms.h"

#include "vis_driver.h"
#include "vis_driver_i.h"
#include "vis_selftest.h"

extern int audio_rms_test(void);
// END NEW Felix Knoll

// BEGIN NEW Nicolas Lonthoff
#include "vis_driver.h"
#include "vis_core.h"
// END NEW Nicolas Lonthoff

int main()
{
    init_platform();

    // Enable Cache
    Xil_ICacheEnable();
    Xil_DCacheEnable();

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

    ///Visualization selftests (from vis_selftest_main.c)
    VIS_TestRegisters(VIS_InstPtr);
    VIS_Core_TestRender(VIS_InstPtr);

    ///Main loop: RMS polling
    AUD_EnableSampling(AUD_InstPtr);
    rms_reset();

    uint32_t block_overruns = 0;
    while (1) {
        block_overruns += AUD_GetOverruns(AUD_InstPtr);

        if (AUD_LAvailable(AUD_InstPtr)) {
            int32_t l = AUD_GetL(AUD_InstPtr);
            int32_t r = 0;
            if (AUD_RAvailable(AUD_InstPtr)) {
                r = AUD_GetR(AUD_InstPtr);
            } else {
                xil_printf("not av rechts\n\r");
            }
            rms_add(l, r);

            if (rms_block_full()) {
                //xil_printf("Block full \n\r");
                uint16_t loud = rms_value_fp72();
                VIS_Core_RenderLoudness(VIS_InstPtr, loud);  /* 7.2-Fixed-Point, 400 = 100% */
                /* fp72: Ganzteil = loud>>2, Nachkomma = (loud&3)*25 (Schritte 0.25).
                 * maxAbs zeigt ob ueberhaupt Audio ankommt (0 => stille/keine Quelle). */
                //xil_printf("Lautstaerke: %u.%02u  (ovr=%u)\n\r",
                //           loud >> 2, (loud & 3u) * 25u,  block_overruns);
                block_overruns = 0;
                rms_reset();
            }
        } else {
            xil_printf("not av links\n\r");
        }
    }
    // END NEW Felix Knoll

    cleanup_platform();
    return 0;
}
