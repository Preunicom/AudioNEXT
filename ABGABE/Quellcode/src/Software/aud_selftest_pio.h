#ifndef AUD_SELFTEST_PIO_H
#define AUD_SELFTEST_PIO_H

/****************** Include Files ********************/
#include "aud_driver.h"


/**************************** Type Definitions *****************************/


/************************** Function Prototypes ****************************/

XStatus AUD_TestRegister(AUD_Data *InstancePtr, uint32_t AddrOffset,
                        uint32_t ExpectedReadRegValWriteFF,
                        uint32_t ExpectedReadRegValWrite00);

XStatus AUD_TestRegisters(AUD_Data *InstancePtr);

// BEGIN NEW Richard Tuch / Felix Knoll
XStatus AUD_TestSamplingEnable(AUD_Data *InstancePtr);
XStatus AUD_TestSamplingL(AUD_Data *InstancePtr);
XStatus AUD_TestSamplingR(AUD_Data *InstancePtr);
// END NEW Richard Tuch / Felix Knoll


/************************** Function Definitions ***************************/

#endif // AUD_SELFTEST_PIO_H
