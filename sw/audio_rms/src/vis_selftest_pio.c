//begin edit Maximilian Hafeneder
/***************************** Include Files *******************************/
#include "vis_driver.h"
#include "vis_driver_i.h"
#include "vis_selftest_pio.h"

/************************** Constant Definitions ***************************/

XStatus VIS_TestRegister(VIS_Data *InstancePtr, uint32_t AddrOffset, uint32_t ExspectedReadRegValWriteFF, uint32_t ExspectedReadRegValWrite00)
{
  XStatus Status=XST_SUCCESS;
  uint32_t val;

  VIS_mWriteReg(InstancePtr->BaseAddress, AddrOffset, 0xffffffff);
  val=VIS_mReadReg(InstancePtr->BaseAddress, AddrOffset);
  if (val != ExspectedReadRegValWriteFF) 
  {  
    xil_printf("Value mismatch: A:0x%0x : Expected 0x%x -> Got 0x%x\n\r", AddrOffset, ExspectedReadRegValWriteFF, val);
    Status = XST_FAILURE;
  }

  VIS_mWriteReg(InstancePtr->BaseAddress, AddrOffset, 0x00000000);
  val=VIS_mReadReg(InstancePtr->BaseAddress, AddrOffset); 
  if (val != ExspectedReadRegValWrite00)
  {  
    xil_printf("Value mismatch: A:0x%0x : Expected 0x%x -> Got 0x%x\n\r", AddrOffset, ExspectedReadRegValWrite00, val);
    Status = XST_FAILURE;
  }

  return Status;
}


XStatus VIS_TestRegisters(VIS_Data *InstancePtr)
{
  XStatus Status=XST_SUCCESS, Statustmp=XST_SUCCESS;
  uint32_t val, expectedval;

  xil_printf("******************************\n\r");
	xil_printf("*VIS_TESTREGISTERS\n\r");
	xil_printf("******************************\n\r");

  Statustmp=VIS_TestRegister(InstancePtr, GCSR_ADDR_OFFSET, 0x00000000, 0x00000000);
  Status |= Statustmp;
  expectedval=GIER_GIE_MASK;
  Statustmp=VIS_TestRegister(InstancePtr, GIER_ADDR_OFFSET, expectedval, 0x00000000);
  Status |= Statustmp;
  expectedval=IPIER_FIE_MASK;
  Statustmp=VIS_TestRegister(InstancePtr, IPIER_ADDR_OFFSET, expectedval, 0x00000000);
  Status |= Statustmp;
  expectedval=IPISR_FDP_MASK;
  Statustmp=VIS_TestRegister(InstancePtr, IPISR_ADDR_OFFSET, 0x00000000, 0x00000000); // W1C
  Status |= Statustmp;
  Statustmp=VIS_TestRegister(InstancePtr, IDR_ADDR_OFFSET, 0x0000D15C, 0x0000D15C);
  Status |= Statustmp;
  Statustmp=VIS_TestRegister(InstancePtr, VERR_ADDR_OFFSET, 0x00000001, 0x00000001);
  Status |= Statustmp;
  expectedval=CTRL_VEN_MASK | CTRL_WD_MASK;
  Statustmp=VIS_TestRegister(InstancePtr, CTRL_ADDR_OFFSET, expectedval, 0x00000000);
  Status |= Statustmp;
  Statustmp=VIS_TestRegister(InstancePtr, STATUS_ADDR_OFFSET, 0x00000000, 0x00000000);
  Status |= Statustmp;
  expectedval=ADDRR_XA_MASK | ADDRR_YA_MASK;
  Statustmp=VIS_TestRegister(InstancePtr, ADDRR_ADDR_OFFSET, expectedval, 0x00000000);
  Status |= Statustmp;
  expectedval=VDATR_CHAR_MASK;
  Statustmp=VIS_TestRegister(InstancePtr, VDATR_ADDR_OFFSET, expectedval, 0x00000000);
  Status |= Statustmp;
  expectedval=COLR_COL_MASK;
  Statustmp=VIS_TestRegister(InstancePtr, COLR_ADDR_OFFSET, expectedval, 0x00000000);
  Status |= Statustmp;
  
  if (Status==XST_SUCCESS){
    xil_printf("VIS_TESTREGISTERS was successful\n\r");
  }else {
    xil_printf("VIS_TESREGISTERS failed\n\r");
  }
  return Status;
}


//XStatus VIS_TestUp(VIS_Data *InstancePtr)
//{
//  XStatus Status;
//  uint32_t val, val2;

//  xil_printf("******************************\n\r");
//	xil_printf("*VIS_TESTUP\n\r");
//	xil_printf("******************************\n\r");

//  #ifdef VIS_DEBUG_P
//  val=VIS_mReadReg(InstancePtr->BaseAddress, IDR_ADDR_OFFSET);
//  xil_printf("IDR: 0x%x\n\r", val);
//  #endif

//  #ifdef VIS_DEBUG_P
//  val=VIS_mReadReg(InstancePtr->BaseAddress, VERR_ADDR_OFFSET);
//  xil_printf("VERR: 0x%x\n\r", val);
//  #endif

//  #ifdef VIS_DEBUG_P
//  val=VIS_mReadReg(InstancePtr->BaseAddress, LR0_ADDR_OFFSET);
//  xil_printf("LR0: 0x%x\n\r", val);
//  #endif
    
//  VIS_mWriteReg(InstancePtr->BaseAddress, LR0_ADDR_OFFSET, LR0_10S_100MHZ); //LR0_40NS_100MHZ LR0_655US_100MHZ LR0_10S_100MHZ 
 
//  #ifdef VIS_DEBUG_P
//  val=VIS_mReadReg(InstancePtr->BaseAddress, LR0_ADDR_OFFSET);
//  xil_printf("LR0: 0x%x\n\r", val);
//  #endif

  //TB_ud0 <='1'; (5)
  //TB_load0 <='0'; (4)
  //TB_ent0_out <='1'; (2)
  //VIS_mWriteReg(InstancePtr->BaseAddress, SCSR_ADDR_OFFSET, 0x00000024);
//  VIS_mWriteReg(InstancePtr->BaseAddress, SCSR_ADDR_OFFSET, SCSR_ENT0_OUT_MASK | SCSR_UD0_MASK);
//  #ifdef VIS_DEBUG_P
//  val=VIS_mReadReg(InstancePtr->BaseAddress, SCSR_ADDR_OFFSET);
//  xil_printf("SCSR: 0x%x\n\r", val);
//  #endif

//  #ifdef VIS_DEBUG_P
//  val=VIS_mReadReg(InstancePtr->BaseAddress, GCSR_ADDR_OFFSET);   
//  xil_printf("GCSR: 0x%x\n\r", val); 
//  #endif
  //TB_auto_restart<='0'; (7)    
  //TB_ap_start <='1'; (0)
//  VIS_mWriteReg(InstancePtr->BaseAddress, GCSR_ADDR_OFFSET, GCSR_START_MASK);

//  #ifdef VIS_DEBUG_P
//  val=VIS_mReadReg(InstancePtr->BaseAddress, GCSR_ADDR_OFFSET);
//  xil_printf("GCSR: 0x%x\n\r", val);
  //Note this control read will cause hanghup in polling loop if the LR0 value is too short!!!
//  #endif
     
 
//  val=VIS_PollDone(InstancePtr);
    
	   
//  #ifdef VIS_DEBUG_P
//  xil_printf("GCSR break: 0x%x\n\r", val);
//  #endif

  

//  #ifdef VIS_DEBUG_P
  //check if done and start reset is done
//  val=VIS_mReadReg(InstancePtr->BaseAddress, GCSR_ADDR_OFFSET); 
//  xil_printf("GCSR: 0x%x\n\r", val);
//  #endif
    
//  #ifdef VIS_DEBUG_P
//  val=VIS_mReadReg(InstancePtr->BaseAddress, CR0_ADDR_OFFSET);
//  xil_printf("CR0: 0x%x\n\r", val);
//  #endif

//  xil_printf("VIS_TestUP passed\n\r");

//  return XST_SUCCESS;
//}


//XStatus VIS_TestUpAutoRestart(VIS_Data *InstancePtr)
//{
//  XStatus Status;
//  uint32_t val, val2;

//  xil_printf("******************************\n\r");
//	xil_printf("*VIS_TESTUPAUTORESTART\n\r");
//	xil_printf("******************************\n\r");


//  #ifdef VIS_DEBUG_P
//  val=VIS_mReadReg(InstancePtr->BaseAddress, IDR_ADDR_OFFSET);
//  xil_printf("IDR: 0x%x\n\r", val);
//  #endif

//  #ifdef VIS_DEBUG_P
//  val=VIS_mReadReg(InstancePtr->BaseAddress, VERR_ADDR_OFFSET);
//  xil_printf("VERR: 0x%x\n\r", val);
//  #endif

//  #ifdef VIS_DEBUG_P
//  val=VIS_mReadReg(InstancePtr->BaseAddress, LR0_ADDR_OFFSET);
//  xil_printf("LR0: 0x%x\n\r", val);
//  #endif
    
//  VIS_mWriteReg(InstancePtr->BaseAddress, LR0_ADDR_OFFSET, LR0_1S_100MHZ);  
 
//  #ifdef VIS_DEBUG_P
//  val=VIS_mReadReg(InstancePtr->BaseAddress, LR0_ADDR_OFFSET);
//  xil_printf("LR0: 0x%x\n\r", val);
//  #endif

  //TB_ud0 <='1'; (5)
  //TB_load0 <='0'; (4)
  //TB_ent0_out <='1'; (2) 
  //VIS_mWriteReg(InstancePtr->BaseAddress, SCSR_ADDR_OFFSET, 0x00000024); 
//  VIS_mWriteReg(InstancePtr->BaseAddress, SCSR_ADDR_OFFSET, SCSR_ENT0_OUT_MASK | SCSR_UD0_MASK); 
//  #ifdef VIS_DEBUG_P
//  val=VIS_mReadReg(InstancePtr->BaseAddress, SCSR_ADDR_OFFSET);
//  xil_printf("SCSR: 0x%x\n\r", val);
//  #endif

//  #ifdef VIS_DEBUG_P
//  val=VIS_mReadReg(InstancePtr->BaseAddress, GCSR_ADDR_OFFSET);   
//  xil_printf("GCSR: 0x%x\n\r", val); 
//  #endif
  //TB_auto_restart<='1'; (7)    
  //TB_ap_start <='1'; (0)
  //VIS_mWriteReg(InstancePtr->BaseAddress, GCSR_ADDR_OFFSET, 0x00000081);
//  VIS_mWriteReg(InstancePtr->BaseAddress, GCSR_ADDR_OFFSET, GCSR_START_MASK | GCSR_AUTO_RESTART_MASK);

//  #ifdef VIS_DEBUG_P
//  val=VIS_mReadReg(InstancePtr->BaseAddress, GCSR_ADDR_OFFSET);
//  xil_printf("GCSR: 0x%x\n\r", val);
  //Note this control read will cause hanghup in polling loop if the LR0 value is too short!!!
//  #endif
     
 
//  val=VIS_PollDone(InstancePtr);
    
	   
//  #ifdef VIS_DEBUG_P
//  xil_printf("GCSR break: 0x%x\n\r", val);
//  #endif



//  #ifdef VIS_DEBUG_P
  //check if done and start reset is done
//  val=VIS_mReadReg(InstancePtr->BaseAddress, GCSR_ADDR_OFFSET); 
//  xil_printf("GCSR: 0x%x\n\r", val);
//  #endif
    
//  #ifdef VIS_DEBUG_P
//  val=VIS_mReadReg(InstancePtr->BaseAddress, CR0_ADDR_OFFSET);
//  xil_printf("CR0: 0x%x\n\r", val);
//  #endif

//  xil_printf("VIS_TestUP passed\n\r");
//}

//end edit Maximilian Hafeneder

