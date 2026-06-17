
/****************** Include Files ********************/
//#include "xparameters.h"

#include "aud_driver.h"
#include "aud_driver_i.h"


/************************** Constant Definitions ***************************/


/**************************** Type Definitions *****************************/
extern AUD_Config AUD_ConfigTable[XPAR_AUD_NUM_INSTANCES];

/************************** Variable Definitions ***************************/


/************************** Function Definitions ***************************/

AUD_Config *AUD_LookupConfig(UINTPTR BaseAddress)
{
	extern AUD_Config AUD_ConfigTable[];
	AUD_Config *CfgPtr = NULL;
	u32 Index;

	for (Index = 0; AUD_ConfigTable[Index].Name != NULL; Index++) {
		if ((AUD_ConfigTable[Index].BaseAddress == BaseAddress) || !BaseAddress) {
			CfgPtr = &AUD_ConfigTable[Index];
			break;
		}
	}

	return CfgPtr;
}
