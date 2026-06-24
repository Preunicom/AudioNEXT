
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

  // Poll for write done after each character to avoid overwhelming the IP and causing tearing artifacts and/or dropped frames.
  // This is necessary because the IP needs some time to process each character and update the display,
  // and if we write too fast, we might end up with a backlog of writes that the IP cannot handle in time.
  VIS_PollWD(InstancePtr);
}

// Poll for frame done
void VIS_PollFDP(VIS_Data *InstancePtr)
{
  UINTPTR baseaddr = InstancePtr->BaseAddress;

  // Clear any stale FDP event so we always wait for the NEXT frame boundary.
  // STATUS_FDP stays HIGH during H-blanking of visible rows and is therefore
  // unreliable; IPISR_FDP is a true one-shot event flag (W1C, set once per frame).
  VIS_mWriteReg(baseaddr, IPISR_ADDR_OFFSET, IPISR_FDP_MASK);
  while ((VIS_mReadReg(baseaddr, IPISR_ADDR_OFFSET) & IPISR_FDP_MASK) == 0) {}
  VIS_mWriteReg(baseaddr, IPISR_ADDR_OFFSET, IPISR_FDP_MASK);
}

// Poll for write done
void VIS_PollWD(VIS_Data *InstancePtr)
{
  UINTPTR baseaddr = InstancePtr->BaseAddress;

  while ((VIS_mReadReg(baseaddr, CTRL_ADDR_OFFSET) & CTRL_WD_MASK) != 0) {}
}
// End user code (Nicolas Lonthoff)
//end edit Maximilian Hafeneder / Nicolas Lonthoff