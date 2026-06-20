#ifndef VIS_SELFTEST_H
#define VIS_SELFTEST_H

#include "xstatus.h"
#include "vis_driver.h"
#include "vis_core.h"

/************************** Function Prototypes ****************************/

/* PIO register selftests */
XStatus VIS_TestRegister(VIS_Data *InstancePtr, uint32_t AddrOffset,
                         uint32_t ExspectedReadRegValWriteFF,
                         uint32_t ExspectedReadRegValWrite00);
XStatus VIS_TestRegisters(VIS_Data *InstancePtr);

/* Interrupt selftest handler */
void VIS_AppHandler_SelfTest(void *CallBackRef);

/* Core unit tests (no hardware required) */
XStatus VIS_Core_TestFormat(void);
XStatus VIS_Core_TestBarWidth(void);

/* Core integration test (requires display hardware) */
XStatus VIS_Core_TestRender(VIS_Data *InstancePtr);

#endif // VIS_SELFTEST_H
