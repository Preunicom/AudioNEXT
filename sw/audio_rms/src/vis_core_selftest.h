
//begin edit Nicolas Lonthoff
#ifndef VIS_CORE_SELFTEST_H
#define VIS_CORE_SELFTEST_H

#include "xstatus.h"
#include "vis_driver.h"
#include "vis_core.h"

/************************** Function Prototypes ****************************/
/* Unit test: text formatting (no hardware required) */
XStatus VIS_Core_TestFormat(void);

/* Unit test: bar width calculation (no hardware required) */
XStatus VIS_Core_TestBarWidth(void);

/* Integration test: full render pipeline (requires display hardware) */
XStatus VIS_Core_TestRender(VIS_Data *InstancePtr);

#endif // VIS_CORE_SELFTEST_H
//end edit Nicolas Lonthoff
