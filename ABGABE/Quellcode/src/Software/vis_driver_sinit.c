
/****************** Include Files ********************/
//#include "xparameters.h"
//begin edit Maximilian Hafeneder
#include "vis_driver.h"
#include "vis_driver_i.h"


/************************** Constant Definitions ***************************/


/**************************** Type Definitions *****************************/
extern VIS_Config VIS_ConfigTable[XPAR_VIS_NUM_INSTANCES];

/************************** Variable Definitions ***************************/


/************************** Function Definitions ***************************/

VIS_Config *VIS_LookupConfig(UINTPTR BaseAddress)
{
	extern VIS_Config VIS_ConfigTable[];
	VIS_Config *CfgPtr = NULL;
	u32 Index;

	for (Index = 0; VIS_ConfigTable[Index].Name != NULL; Index++) {
		/*
		 * If BaseAddress is 0, return Configuration for 0th instance of
		 * device.
		 * As instance base address varies based on designs,
		 * driver examples can pass base address as 0, to use avilable
		 * instance.
		 */
		if ((VIS_ConfigTable[Index].BaseAddress == BaseAddress) ||
		    !BaseAddress)  {
			CfgPtr = &VIS_ConfigTable[Index];
			break;
		}
	}
//end edit Maximilian Hafeneder
	return CfgPtr;
}