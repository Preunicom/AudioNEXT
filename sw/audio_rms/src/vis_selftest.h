//begin edit Maximilian Hafeneder / Nicolas Lonthoff
#ifndef VIS_SELFTEST_H
#define VIS_SELFTEST_H

#include "xstatus.h"
#include "vis_driver.h"
#include "vis_core.h"

/************************** Function Prototypes ****************************/

/* PIO register selftests (originally vis_selftest_pio.h) */
XStatus VIS_TestRegister(VIS_Data *InstancePtr, uint32_t AddrOffset,
                         uint32_t ExspectedReadRegValWriteFF,
                         uint32_t ExspectedReadRegValWrite00);
XStatus VIS_TestRegisters(VIS_Data *InstancePtr);
XStatus VIS_TestUp(VIS_Data *InstancePtr);
XStatus VIS_TestUpAutoRestart(VIS_Data *InstancePtr);

/* Interrupt selftests (originally vis_selftest_int.h) */
void VIS_AppHandler_SelfTest(void *CallBackRef);
XStatus VIS_TestUp_int(VIS_Data *VISInstancePtr);
XStatus VIS_TestUpAutoRestart_int(VIS_Data *InstancePtr);
XStatus VIS_TestUp_int_AutorestartSW(VIS_Data *InstancePtr);

/* Core selftests (originally vis_core_selftest.h) */

/* Unit test: text formatting (no hardware required) */
XStatus VIS_Core_TestFormat(void);

/* Unit test: bar width calculation (no hardware required) */
XStatus VIS_Core_TestBarWidth(void);

/* Integration test: full render pipeline (requires display hardware) */
XStatus VIS_Core_TestRender(VIS_Data *InstancePtr);

#endif // VIS_SELFTEST_H
//end edit Maximilian Hafeneder / Nicolas Lonthoff
