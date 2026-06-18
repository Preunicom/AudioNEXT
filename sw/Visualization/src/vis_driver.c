
//begin edit Maximilian Hafeneder / Nicolas Lonthoff
/***************************** Include Files *******************************/
#include "vis_driver.h"
#include "vis_driver_i.h"
#include "vis_driver_l.h"


/************************** Constant Definitions ***************************/


/**************************** Type Definitions *****************************/


/************************** Variable Definitions ***************************/


/************************** Function Prototypes ****************************/
static void VIS_StubAppHandler(void *CallBackRef);


/************************** Function Definitions ***************************/
static void VIS_StubAppHandler(void *CallBackRef)
{
	Xil_AssertVoid(CallBackRef != NULL);
	//Dummy handler 
  //just do nothing 
}


XStatus VIS_Init(VIS_Data *InstancePtr, UINTPTR BaseAddress)
{
  XStatus Status = XST_SUCCESS;
  VIS_Config *ConfigPtr;

  Xil_AssertNonvoid(InstancePtr != NULL);

  //Get core configuration (file _g mechanism) 
	ConfigPtr = VIS_LookupConfig(BaseAddress);
    if (!ConfigPtr) {
		Status = XST_DEVICE_NOT_FOUND;
    return Status;
	}

  VIS_InitCfg(InstancePtr, BaseAddress, ConfigPtr);
  
  Status = VIS_InitHw(InstancePtr);

  return Status; 
}   


void VIS_InitCfg(VIS_Data *InstancePtr, UINTPTR BaseAddress, VIS_Config *ConfigPtr)
{
  Xil_AssertVoid(InstancePtr != NULL);
  Xil_AssertVoid(BaseAddress != 0x0);
  Xil_AssertVoid(ConfigPtr != NULL);
  
  InstancePtr->Config = *ConfigPtr;
    
  InstancePtr->BaseAddress = BaseAddress;

  InstancePtr->AppHandler = VIS_StubAppHandler; //set dummy handler, should be overwritten later
  InstancePtr->CallBackRef = InstancePtr;
}


XStatus VIS_InitHw(VIS_Data *InstancePtr)
{
  XStatus Status = XST_SUCCESS;
   
  ///Immediate HW Initilaization
  //Put in here things which have to be done immediately after start
  
  // Begin user code (Nicolas Lonthoff)
  UINTPTR baseaddr = InstancePtr->BaseAddress;

  VIS_mWriteReg(baseaddr, CTRL_ADDR_OFFSET, CTRL_VEN_MASK);

  if ((VIS_mReadReg(baseaddr, CTRL_ADDR_OFFSET) & CTRL_VEN_MASK) != CTRL_VEN_MASK) {
    Status = XST_FAILURE;
  }
  // End user code (Nicolas Lonthoff)

  return Status;
}


// Begin user code (Nicolas Lonthoff)
void VIS_WriteChar(VIS_Data *InstancePtr, u8 x, u8 y, u8 ch, u8 cr, u8 cg, u8 cb)
{
  UINTPTR baseaddr = InstancePtr->BaseAddress;

  Xil_AssertVoid(x <= 79);
  Xil_AssertVoid(y <= 29);

  VIS_mWriteReg(baseaddr, ADDRR_ADDR_OFFSET, ((u32)y << 8) | (u32)x);
  VIS_mWriteReg(baseaddr, VDATR_ADDR_OFFSET, (u32)ch);
  VIS_mWriteReg(baseaddr, COLR_ADDR_OFFSET,  ((u32)cb << 16) | ((u32)cg << 8) | (u32)cr);
  VIS_mWriteReg(baseaddr, CTRL_ADDR_OFFSET,  CTRL_VEN_MASK | CTRL_WD_MASK);
}


void VIS_PollFDP(VIS_Data *InstancePtr)
{
  UINTPTR baseaddr = InstancePtr->BaseAddress;

  while ((VIS_mReadReg(baseaddr, STATUS_ADDR_OFFSET) & STATUS_FDP_MASK) == 0) {}

  VIS_mWriteReg(baseaddr, IPISR_ADDR_OFFSET, IPISR_FDP_MASK);
}
// End user code (Nicolas Lonthoff)
//end edit Maximilian Hafeneder / Nicolas Lonthoff