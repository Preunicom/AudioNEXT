
//begin edit Maximilian Hafeneder
#ifndef VIS_SELFTEST_INT_H
#define VIS_SELFTEST_INT_H

/****************** Include Files ********************/


/************************** Constant Definitions ***************************/


/**************************** Type Definitions *****************************/


/************************** Function Prototypes ****************************/
void VIS_AppHandler_SelfTest(void *CallBackRef);


XStatus VIS_TestUp_int(VIS_Data *VISInstancePtr);

XStatus VIS_TestUpAutoRestart_int(VIS_Data *InstancePtr);

XStatus VIS_TestUp_int_AutorestartSW(VIS_Data *InstancePtr);


/************************** Function Definitions ***************************/


#endif // VIS_SELFTEST_INT_H

//end edit Maximilian Hafeneder