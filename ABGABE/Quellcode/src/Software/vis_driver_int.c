
//begin edit Maximilian Hafeneder
/****************** Include Files ********************/
#include "vis_driver.h"
#include "vis_driver_i.h"
#include "vis_driver_int.h"


/************************** Constant Definitions ***************************/


/**************************** Type Definitions *****************************/


/************************** Variable Definitions ***************************/


/************************** Function Definitions ***************************/
static void VIS_SetAppHandler(VIS_Data *InstancePtr, VIS_AppHandlerFpType FuncPtr,
			void *CallBackRef)
{
	Xil_AssertVoid(InstancePtr != NULL);
	Xil_AssertVoid(FuncPtr != NULL);
  
	InstancePtr->AppHandler = FuncPtr; 
	InstancePtr->CallBackRef = CallBackRef; 
}


uint32_t VIS_WaitForInt(VIS_Data *InstancePtr){
  
  uint32_t val=0;
  
  while (1) {  
		if (vis_eventIsSet() ) {
			break;
		}  
  }  
  
  vis_clearEvent(0);
    
  return val;  
}


void VIS_InterruptHandler(void *pInstancePtr) 
{
  uint32_t val;
  VIS_Data *InstancePtr;
  //Disable interrupts?
  //microblaze_disable_interrupts(); //alternative if only own int: XEnableIntrId(VIS_INTRID, VIS_INTRPARENT);
  
  //Verify that each of the inputs are valid.
	Xil_AssertVoid(pInstancePtr != NULL);
  //Convert the non-typed pointer to an VIS instance pointer
  InstancePtr = (VIS_Data *) pInstancePtr;


  //check global int enabled //useful? (debug only)
  /*
  #ifdef VIS_DEBUG_P
  val=VIS_mReadReg(InstancePtr->BaseAddress, GIER_ADDR_OFFSET);
  xil_printf("ISR: GIER: 0x%x\n\r", val);
  #endif
  */
  
  //check ip int enabled //useful? (debug only)
  /*
  #ifdef VIS_DEBUG_P
  val=VIS_mReadReg(InstancePtr->BaseAddress, IPIER_ADDR_OFFSET);
  xil_printf("ISR: IPER: 0x%x\n\r", val);
  #endif
  */
  
  //check int occured //useful? (debug only)
  /*
  #ifdef VIS_DEBUG_P
  val=VIS_mReadReg(InstancePtr->BaseAddress, IPISR_ADDR_OFFSET);
  xil_printf("ISR: IPER: 0x%x\n\r", val);
  #endif
  */
  

  //call the app callback / handler (not neccessary so far, comment in if required)
  //InstancePtr->AppHandler(InstancePtr->CallBackRef); 


  //set the sync event	
  vis_setEvent(1);
  

  ///Do any special handling here if reuiqred -> is not required  

  /*
  //disable the counter //necessary? -> not necessary
  val=VIS_mReadReg(InstancePtr->BaseAddress, GCSR_ADDR_OFFSET);  
  VIS_mWriteReg(InstancePtr->BaseAddress, val & ~GCSR_START_MASK);
  #ifdef VIS_DEBUG_P
  val=VIS_mReadReg(InstancePtr->BaseAddress, GCSR_ADDR_OFFSET);   
  xil_printf("ISR: GCSR: 0x%x\n\r", val); 
  #endif
  //  
  //(optional) trigger reloading the load value //necessary? -> not necessary
  val=VIS_mReadReg(InstancePtr->BaseAddress, SCSR_ADDR_OFFSET);
  VIS_mWriteReg(InstancePtr->BaseAddress, SCSR_ADDR_OFFSET, val | SCSR_LOAD0_MASK );
  #ifdef VIS_DEBUG_P
  val=VIS_mReadReg(InstancePtr->BaseAddress, SCSR_ADDR_OFFSET);
  xil_printf("ISR: SCSR: 0x%x\n\r", val);
  #endif
  //
  //reset the load trigger again //necessary? -> not necessary
  val=VIS_mReadReg(InstancePtr->BaseAddress, SCSR_ADDR_OFFSET);
  VIS_mWriteReg(InstancePtr->BaseAddress, SCSR_ADDR_OFFSET, val & ~SCSR_LOAD0_MASK );
  */


  //acknowledge the interrupt
  #ifdef VIS_DEBUG_P
  val=VIS_mReadReg(InstancePtr->BaseAddress, IPISR_ADDR_OFFSET);
  xil_printf("ISR: IPISR before ack: 0x%x\n\r", val);
  #endif
  VIS_mWriteReg(InstancePtr->BaseAddress, IPISR_ADDR_OFFSET, IPISR_FDP_MASK);
  #ifdef VIS_DEBUG_P
  val=VIS_mReadReg(InstancePtr->BaseAddress, IPISR_ADDR_OFFSET);
  xil_printf("ISR: IPISR after ack: 0x%x\n\r", val);
  #endif
  
  //Enable interrupts again?  
  //microblaze_enable_interrupts(); //alternative if only own int: XDisableIntrId(VIS_INTRID, VIS_INTRPARENT);
  
}

//end edit Maximilian Hafeneder