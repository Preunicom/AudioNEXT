
//begin edit Maximilian Hafeneder
#ifndef VIS_DRIVER_INT_H
#define VIS_DRIVER_INT_H

/****************** Include Files ********************/
#include "xil_exception.h"
#include "xinterrupt_wrap.h"
#include "xintc.h"

#include "vis_event.h"

/************************** Constant Definitions ***************************/


/**************************** Type Definitions *****************************/


/************************** Function Prototypes ****************************/
void static VIS_SetAppHandler(VIS_Data *InstancePtr, VIS_AppHandlerFpType FuncPtr,
			void *CallBackRef);

uint32_t VIS_WaitForInt(VIS_Data *InstancePtr);

void VIS_InterruptHandler(void *InstancePtr);  //static? not necessary so far

/************************** Function Definitions ***************************/


#endif // VIS_DRIVER_INT_H
//end edit Maximilian Hafeneder
