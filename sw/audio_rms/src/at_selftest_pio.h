#ifndef AT_SELFTEST_PIO_H
#define AT_SELFTEST_PIO_H

/****************** Include Files ********************/
#include "at_driver.h"


/**************************** Type Definitions *****************************/


/************************** Function Prototypes ****************************/

XStatus AT_TestRegister(AT_Data *InstancePtr, uint32_t AddrOffset,
                        uint32_t ExpectedReadRegValWriteFF,
                        uint32_t ExpectedReadRegValWrite00);

XStatus AT_TestRegisters(AT_Data *InstancePtr);

XStatus AT_TestSampling(AT_Data *InstancePtr);


/************************** Function Definitions ***************************/

#endif // AT_SELFTEST_PIO_H
