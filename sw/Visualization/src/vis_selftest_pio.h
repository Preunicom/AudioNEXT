//begin edit Maximilian Hafeneder
#ifndef VIS_SELFTEST_PIO_H
#define VIS_SELFTEST_PIO_H

/****************** Include Files ********************/


/**************************** Type Definitions *****************************/


/************************** Function Prototypes ****************************/

XStatus VIS_TestRegister(VIS_Data *InstancePtr, uint32_t AddrOffset, uint32_t ExspectedReadRegValWriteFF, uint32_t ExspectedReadRegValWrite00);

XStatus VIS_TestRegisters(VIS_Data *InstancePtr);

XStatus VIS_TestUp(VIS_Data *InstancePtr);

XStatus VIS_TestUpAutoRestart(VIS_Data *InstancePtr);


/************************** Function Definitions ***************************/

#endif // VIS_SELFTEST_PIO_H
//end edit Maximilian Hafeneder

