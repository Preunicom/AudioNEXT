/*
 * helloworld.c: simple test application
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

/****************** Include Files ********************/
//#include <stdio.h>
#include <stdint.h>
#include "platform.h"
#include "xil_printf.h"

//begin edit Maximilian Hafeneder
#include "vis_driver.h"
#include "vis_selftest.h"


/************************** Constant Definitions ***************************/


/**************************** Type Definitions *****************************/
//VIS_Data VIS_DataInst; //driver data struct (global)

/************************** Variable Definitions ***************************/


/************************** Function Definitions ***************************/


int main()
{
    VIS_Data VIS_Inst;
    VIS_Data *VIS_InstPtr = &VIS_Inst;
    XStatus Status;

    init_platform();
    #ifdef VIS_DEBUG_P
    print("Platform initialized\n\r");
    #endif

    Status=VIS_Init(VIS_InstPtr, VIS_BASEADDRESS);
    if (Status != XST_SUCCESS){
      xil_printf("error during VIS_Init(). Check/Debug manually.\n\r");
    }


    VIS_TestRegisters(VIS_InstPtr);
    //VIS_TestUp(VIS_InstPtr);
    //VIS_TestUpAutoRestart(VIS_InstPtr);
    //VIS_TestUp_int(VIS_InstPtr);
    //VIS_TestUpAutoRestart_int(VIS_InstPtr);
    //VIS_TestUp_int_AutorestartSW(VIS_InstPtr);
    VIS_Core_TestFormat();
    VIS_Core_TestBarWidth();
    VIS_Core_TestFDP(VIS_InstPtr);
    VIS_Core_TestScrolling(VIS_InstPtr);
    VIS_Core_TestRender(VIS_InstPtr);

//end edit Maximilian Hafeneder

    xil_printf("end of program reached. cleaning up.\n\r");

    cleanup_platform();
    return 0;
}
