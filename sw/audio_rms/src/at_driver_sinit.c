
/****************** Include Files ********************/
//#include "xparameters.h"

#include "at_driver.h"
#include "at_driver_i.h"


/************************** Constant Definitions ***************************/


/**************************** Type Definitions *****************************/
extern AT_Config AT_ConfigTable[XPAR_AT_NUM_INSTANCES];

/************************** Variable Definitions ***************************/


/************************** Function Definitions ***************************/

AT_Config *AT_LookupConfig(UINTPTR BaseAddress)
{
	extern AT_Config AT_ConfigTable[];
	AT_Config *CfgPtr = NULL;
	u32 Index;

	for (Index = 0; AT_ConfigTable[Index].Name != NULL; Index++) {
		if ((AT_ConfigTable[Index].BaseAddress == BaseAddress) || !BaseAddress) {
			CfgPtr = &AT_ConfigTable[Index];
			break;
		}
	}

	return CfgPtr;
}
