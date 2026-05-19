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
#include "audio.h"
#include "audio_rms.h"
// END NEW Felix Knoll

// BEGIN NEW Felix Knoll
/* TODO: echte Ausgabe nach implementierung ersetzen. */
void print_on_display(uint16_t value)
{
    xil_printf("value: 0x%03x\n\r", value);
}
// END NEW Felix Knoll

int main()
{
    init_platform();

    // BEGIN NEW Felix Knoll
    audio_enable_sampling();
    rms_reset();

    /* Polling
     * 32000 Samples pro Kanal ensprechen 3 Hz Ausgaberate */
    uint32_t block_overruns = 0;
    while (1) {
        block_overruns += audio_get_overruns();

        if (audio_l_available()) {
            int32_t l = audio_get_l();
            int32_t r = 0;
            if (audio_r_available()) {
                r = audio_get_r();
            }
            rms_add(l, r);

            if (rms_block_full()) {
                print_on_display(rms_value_fp72());
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
