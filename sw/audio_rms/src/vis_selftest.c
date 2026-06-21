//begin edit Maximilian Hafeneder / Nicolas Lonthoff
/***************************** Include Files *******************************/
#include "vis_selftest.h"
#include "vis_driver_i.h"
#include "vis_driver_int.h"
#include "xil_printf.h"


/*****************************************************************************
 * PIO register selftests (originally vis_selftest_pio.c)
 *****************************************************************************/

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


/*****************************************************************************
 * Interrupt selftests (originally vis_selftest_int.c)
 *****************************************************************************/

//##INSERT YOUR CODE HERE

void VIS_AppHandler_SelfTest(void *CallBackRef)
{
	VIS_Data *InstancePtr = (VIS_Data *)CallBackRef;

  //Application specific handler called by VIS_InterruptHandler
  //here do nothing so far ...

}

// XStatus VIS_TestUp_int(VIS_Data *InstancePtr){

//   int Status=XST_SUCCESS;
//   uint32_t val=0;

//   xil_printf("******************************\n\r");
// 	xil_printf("*VIS_TESTUP_int\n\r");
// 	xil_printf("******************************\n\r");

//   ///Setup interrupt
//   Status = XSetupInterruptSystem(InstancePtr, (XInterruptHandler)VIS_InterruptHandler, \
// 				       InstancePtr->Config.IntrId, InstancePtr->Config.IntrParent, \
// 				       XINTERRUPT_DEFAULT_PRIORITY);

//   //Register the AT app handler if neccesary
//   //VIS_SetAppHandlerApp(InstancePtr, VIS_AppHandler_SelfTest, InstancePtr);


//   ///Do the normal setup of the AT component

//   //Check device id (debug only)
//   #ifdef VIS_DEBUG_P
//   val=VIS_mReadReg(InstancePtr->BaseAddress, IDR_ADDR_OFFSET);
//   xil_printf("IDR: 0x%x\n\r", val);
//   #endif

//   //Check version number (debug only)
//   #ifdef VIS_DEBUG_P
//   val=VIS_mReadReg(InstancePtr->BaseAddress, VERR_ADDR_OFFSET);
//   xil_printf("VERR: 0x%x\n\r", val);
//   #endif

//   //Set value load reagister / count limit for counter register
//   #ifdef VIS_DEBUG_P
//   val=VIS_mReadReg(InstancePtr->BaseAddress, LR0_ADDR_OFFSET);
//   xil_printf("LR0: 0x%x\n\r", val);
//   #endif
//   VIS_mWriteReg(InstancePtr->BaseAddress, LR0_ADDR_OFFSET, LR0_10S_100MHZ); //LR0_40NS_100MHZ LR0_655US_100MHZ LR0_10S_100MHZ
//   #ifdef VIS_DEBUG_P
//   val=VIS_mReadReg(InstancePtr->BaseAddress, LR0_ADDR_OFFSET);
//   xil_printf("LR0: 0x%x\n\r", val);
//   #endif

//   //Set AT Special Control and Status Register
//   //TB_ud0 <='1'; (5)
//   //TB_load0 <='0'; (4)
//   //TB_ent0_out <='1'; (2)
//   //VIS_mWriteReg(InstancePtr->BaseAddress, SCSR_ADDR_OFFSET, 0x00000024);
//   VIS_mWriteReg(InstancePtr->BaseAddress, SCSR_ADDR_OFFSET, SCSR_ENT0_OUT_MASK | SCSR_UD0_MASK);
//   #ifdef VIS_DEBUG_P
//   val=VIS_mReadReg(InstancePtr->BaseAddress, SCSR_ADDR_OFFSET);
//   xil_printf("SCSR: 0x%x\n\r", val);
//   #endif


//   ///Setup AT interrupt
//   //Enable AT IP interrupts
//   VIS_mWriteReg(InstancePtr->BaseAddress, IPIER_ADDR_OFFSET, IPIER_IPIE_MASK);
//   #ifdef VIS_DEBUG_P
//   val=VIS_mReadReg(InstancePtr->BaseAddress, IPIER_ADDR_OFFSET);
//   xil_printf("IPER: 0x%x\n\r", val);
//   #endif

//   //Enable AT global interrupt
//   VIS_mWriteReg(InstancePtr->BaseAddress, GIER_ADDR_OFFSET, GIER_GIE_MASK);
//   #ifdef VIS_DEBUG_P
//   val=VIS_mReadReg(InstancePtr->BaseAddress,  GIER_ADDR_OFFSET);
//   xil_printf("GIER: 0x%x\n\r", val);
//   #endif


//   ///Start the AT component / ipcore
//   //Check Global Control and Status Register before starting (debug only)
//   #ifdef VIS_DEBUG_P
//   val=VIS_mReadReg(InstancePtr->BaseAddress, GCSR_ADDR_OFFSET);
//   xil_printf("GCSR: 0x%x\n\r", val);
//   #endif
//   //Actual set of Global Control and Status Register (initiate start)
//   //TB_auto_restart<='0'; (7)
//   //TB_ap_start <='1'; (0)
//   VIS_mWriteReg(InstancePtr->BaseAddress, GCSR_ADDR_OFFSET, GCSR_START_MASK);

//   //Check Global Control and Status Register (debug only)
//   #ifdef VIS_DEBUG_P
//   val=VIS_mReadReg(InstancePtr->BaseAddress, GCSR_ADDR_OFFSET);
//   xil_printf("GCSR: 0x%x\n\r", val);
//   //Note this control read will cause hangup in polling loop if the LR0 value is too short!!!
//   #endif


//   ///Sync Interrupt
//   val=VIS_WaitForInt(InstancePtr);

//   //Check IPISR value after interrupt (reset correctly?) (debug only)
//   #ifdef VIS_DEBUG_P
//   val=VIS_mReadReg(InstancePtr->BaseAddress, IPISR_ADDR_OFFSET);
//   xil_printf("Int sync done - IPISR: 0x%x\n\r", val);
//   #endif



//   ///Do wrap up / checks after sync interrupt

//   //disable AT Interrupt again? - not necessary in this case here
//   /*
//   //Disable IP specific part
//     //Disable AT global interrupt
//     val=VIS_mReadReg(InstancePtr->BaseAddress, GIER_ADDR_OFFSET);
//     VIS_mWriteReg(InstancePtr->BaseAddress, GIER_ADDR_OFFSET, (val & ~GIER_GIE_MASK));
//     #ifdef VIS_DEBUG_P
//     val=VIS_mReadReg(InstancePtr->BaseAddress,  GIER_ADDR_OFFSET);
//     xil_printf("GIER: 0x%x\n\r", val);
//     #endif


//     //Disable AT IP interrupts
//     val=VIS_mReadReg(InstancePtr->BaseAddress, GIER_ADDR_OFFSET);
//     VIS_mWriteReg(InstancePtr->BaseAddress, IPIER_ADDR_OFFSET, (val & ~IPIER_IPIE_MASK));
//     #ifdef VIS_DEBUG_P
//     val=VIS_mReadReg(InstancePtr->BaseAddress, IPIER_ADDR_OFFSET);
//     xil_printf("IPER: 0x%x\n\r", val);
//     #endif

//   //Disable platform specific part (xinterrupt_wrap.h/c in
//   //  platform/microblaze_0/standalone_microblaze_0/bsp/libsrc/standalone/src/common/intr/xinterupt_wrap.c/h)
//   //Deregister interrupt handler in interrupt controller
//    XDisableIntrId(InstancePtr->Config.IntrId, InstancePtr->Config.IntrParent);
//    */


//   //reset done; this read has to be here since we use interrupt
//   //and do not longer have a polling loop for done where the done signal is reset
//   //so this is our read wich resets the done signal
//   val=VIS_mReadReg(InstancePtr->BaseAddress, GCSR_ADDR_OFFSET);
//   #ifdef VIS_DEBUG_P
//   xil_printf("GCSR: 0x%x\n\r", val);
//   #endif
//   //check again if done and start reset is accomplished (debug only)
//   #ifdef VIS_DEBUG_P
//   val=VIS_mReadReg(InstancePtr->BaseAddress, GCSR_ADDR_OFFSET);
//   xil_printf("GCSR: 0x%x\n\r", val);
//   #endif

//   //check value counter register (debug only)
//   #ifdef VIS_DEBUG_P
//   val=VIS_mReadReg(InstancePtr->BaseAddress, CR0_ADDR_OFFSET);
//   xil_printf("CR0: 0x%x\n\r", val);
//   #endif


//   if (Status != XST_SUCCESS) {
// 		xil_printf("VIS_TestUP_int failed\r\n");
// 		return XST_FAILURE;
// 	}

// 	xil_printf("VIS_TestUP_int passed\n\r");
// 	return XST_SUCCESS;
// }


// XStatus VIS_TestUpAutoRestart_int(VIS_Data *InstancePtr){

// //##INSERT YOUR CODE HERE

// int Status=XST_SUCCESS;
//   uint32_t val=0;

//   xil_printf("******************************\n\r");
// 	xil_printf("*VIS_TESTUPAUTORESTART_int\n\r");
// 	xil_printf("******************************\n\r");

//   ///Setup interrupts
//   Status = XSetupInterruptSystem(InstancePtr, (XInterruptHandler)VIS_InterruptHandler, \
// 				       InstancePtr->Config.IntrId, InstancePtr->Config.IntrParent, \
// 				       XINTERRUPT_DEFAULT_PRIORITY);

//   //Register the AT app handler if neccesary
//   //VIS_SetAppHandlerApp(InstancePtr, VIS_AppHandler_SelfTest, InstancePtr);


//   ///Do the normal setup of the AT component

//   //Check device id (debug only)
//   #ifdef VIS_DEBUG_P
//   val=VIS_mReadReg(InstancePtr->BaseAddress, IDR_ADDR_OFFSET);
//   xil_printf("IDR: 0x%x\n\r", val);
//   #endif

//   //Check version number (debug only)
//   #ifdef VIS_DEBUG_P
//   val=VIS_mReadReg(InstancePtr->BaseAddress, VERR_ADDR_OFFSET);
//   xil_printf("VERR: 0x%x\n\r", val);
//   #endif

//   //Set value load reagister / count limit for counter register
//   #ifdef VIS_DEBUG_P
//   val=VIS_mReadReg(InstancePtr->BaseAddress, LR0_ADDR_OFFSET);
//   xil_printf("LR0: 0x%x\n\r", val);
//   #endif
//   VIS_mWriteReg(InstancePtr->BaseAddress, LR0_ADDR_OFFSET, LR0_10S_100MHZ); //LR0_40NS_100MHZ LR0_655US_100MHZ LR0_10S_100MHZ
//   #ifdef VIS_DEBUG_P
//   val=VIS_mReadReg(InstancePtr->BaseAddress, LR0_ADDR_OFFSET);
//   xil_printf("LR0: 0x%x\n\r", val);
//   #endif

//   //Set AT Special Control and Status Regsiter
//   //TB_ud0 <='1'; (5)
//   //TB_load0 <='0'; (4)
//   //TB_ent0_out <='1'; (2)
//   //VIS_mWriteReg(InstancePtr->BaseAddress, SCSR_ADDR_OFFSET, 0x00000024);
//   VIS_mWriteReg(InstancePtr->BaseAddress, SCSR_ADDR_OFFSET, SCSR_ENT0_OUT_MASK | SCSR_UD0_MASK);
//   #ifdef VIS_DEBUG_P
//   val=VIS_mReadReg(InstancePtr->BaseAddress, SCSR_ADDR_OFFSET);
//   xil_printf("SCSR: 0x%x\n\r", val);
//   #endif


//   ///Setup AT interrupt
//   //Enable AT IP interrupts
//   VIS_mWriteReg(InstancePtr->BaseAddress, IPIER_ADDR_OFFSET, IPIER_IPIE_MASK);
//   #ifdef VIS_DEBUG_P
//   val=VIS_mReadReg(InstancePtr->BaseAddress, IPIER_ADDR_OFFSET);
//   xil_printf("IPER: 0x%x\n\r", val);
//   #endif

//   //Enable AT global interrupt
//   VIS_mWriteReg(InstancePtr->BaseAddress, GIER_ADDR_OFFSET, GIER_GIE_MASK);
//   #ifdef VIS_DEBUG_P
//   val=VIS_mReadReg(InstancePtr->BaseAddress,  GIER_ADDR_OFFSET);
//   xil_printf("GIER: 0x%x\n\r", val);
//   #endif


//   ///Start the AT component / ipcore
//   //Check Global Control and Status Register before starting (debug only)
//   #ifdef VIS_DEBUG_P
//   val=VIS_mReadReg(InstancePtr->BaseAddress, GCSR_ADDR_OFFSET);
//   xil_printf("GCSR: 0x%x\n\r", val);
//   #endif
//   //Actual set of Global Control and Status Register (initiate start)
//   //TB_auto_restart<='0'; (7)
//   //TB_ap_start <='1'; (0)
//   VIS_mWriteReg(InstancePtr->BaseAddress, GCSR_ADDR_OFFSET, GCSR_START_MASK);

//   //Check Global Control and Status Register (debug only)
//   #ifdef VIS_DEBUG_P
//   val=VIS_mReadReg(InstancePtr->BaseAddress, GCSR_ADDR_OFFSET);
//   xil_printf("GCSR: 0x%x\n\r", val);
//   //Note this control read will cause hangup in polling loop if the LR0 value is too short!!!
//   #endif


//   while (1)
//   {
//     ///Sync Interrupt
//     val=VIS_WaitForInt(InstancePtr);

//     //Check IPISR value after interrupt (reset correctly?) (debug only)
//     #ifdef VIS_DEBUG_P
//     val=VIS_mReadReg(InstancePtr->BaseAddress, IPISR_ADDR_OFFSET);
//     xil_printf("Int sync done - IPISR: 0x%x\n\r", val);
//     #endif

//     ///Restart the AT component / ipcore
//     /*
//     //Check Global Control and Status Register
//     val=VIS_mReadReg(InstancePtr->BaseAddress, GCSR_ADDR_OFFSET);
//     #ifdef VIS_DEBUG_P
//     xil_printf("GCSR: 0x%x\n\r", val);
//     #endif
//     */
//     //TB_auto_restart<='0'; (7)
//     //TB_ap_start <='1'; (0)
//     //VIS_mWriteReg(InstancePtr->BaseAddress, GCSR_ADDR_OFFSET, GCSR_START_MASK);

//     //Check Global Control and Status Register (debug only)
//     /*
//     #ifdef VIS_DEBUG_P
//     val=VIS_mReadReg(InstancePtr->BaseAddress, GCSR_ADDR_OFFSET);
//     xil_printf("GCSR: 0x%x\n\r", val);
//     //Note this control read will cause hangup in polling loop if the LR0 value is too short!!!
//     #endif
//     */
//   }



//   ///Do wrap up / checks after sync interrupt

//   //disable AT Interrupt again? - not necessary in this case here
//   /*
//   //Disable IP specific part
//     //Disable AT global interrupt
//     val=VIS_mReadReg(InstancePtr->BaseAddress, GIER_ADDR_OFFSET);
//     VIS_mWriteReg(InstancePtr->BaseAddress, GIER_ADDR_OFFSET, (val & ~GIER_GIE_MASK));
//     #ifdef VIS_DEBUG_P
//     val=VIS_mReadReg(InstancePtr->BaseAddress,  GIER_ADDR_OFFSET);
//     xil_printf("GIER: 0x%x\n\r", val);
//     #endif


//     //Disable AT IP interrupts
//     val=VIS_mReadReg(InstancePtr->BaseAddress, GIER_ADDR_OFFSET);
//     VIS_mWriteReg(InstancePtr->BaseAddress, IPIER_ADDR_OFFSET, (val & ~IPIER_IPIE_MASK));
//     #ifdef VIS_DEBUG_P
//     val=VIS_mReadReg(InstancePtr->BaseAddress, IPIER_ADDR_OFFSET);
//     xil_printf("IPER: 0x%x\n\r", val);
//     #endif

//   //Disable platform specific part (xinterrupt_wrap.h/c in
//   //  platform/microblaze_0/standalone_microblaze_0/bsp/libsrc/standalone/src/common/intr/xinterupt_wrap.c/h)
//     //Deregister interrupt handler in interrupt controller
//     XDisableIntrId(InstancePtr->Config.IntrId, InstancePtr->Config.IntrParent);
//   */

//   //reset done; this read has to be here since we use interrupt
//   //and do not longer have a polling loop for done where the done signal is reset
//   //so this is our read wich resets the done signal
//   val=VIS_mReadReg(InstancePtr->BaseAddress, GCSR_ADDR_OFFSET);
//   #ifdef VIS_DEBUG_P
//   xil_printf("GCSR: 0x%x\n\r", val);
//   #endif
//   //check again if done and start reset is accomplished (debug only)
//   #ifdef VIS_DEBUG_P
//   val=VIS_mReadReg(InstancePtr->BaseAddress, GCSR_ADDR_OFFSET);
//   xil_printf("GCSR: 0x%x\n\r", val);
//   #endif

//   //check value counter register (debug only)
//   #ifdef VIS_DEBUG_P
//   val=VIS_mReadReg(InstancePtr->BaseAddress, CR0_ADDR_OFFSET);
//   xil_printf("CR0: 0x%x\n\r", val);
//   #endif


//   if (Status != XST_SUCCESS) {
// 		xil_printf("VIS_TestUpAutoRestart_int dailed\r\n");
// 		return XST_FAILURE;
// 	}

// 	xil_printf("VIS_TestUpAutoRestart_int passed\n\r");
// 	return XST_SUCCESS;

//   //##INSERT YOUR CODE HERE END
// }


// XStatus VIS_TestUp_int_AutorestartSW(VIS_Data *InstancePtr)
// {
//   //##INSERT YOUR CODE HERE

//   int Status=XST_SUCCESS;
//   uint32_t val=0;

//   xil_printf("******************************\n\r");
// 	xil_printf("*VIS_TestUp_int_AutorestartSW\n\r");
// 	xil_printf("******************************\n\r");

//   ///Setup interrupts
//   Status = XSetupInterruptSystem(InstancePtr, (XInterruptHandler)VIS_InterruptHandler, \
// 				       InstancePtr->Config.IntrId, InstancePtr->Config.IntrParent, \
// 				       XINTERRUPT_DEFAULT_PRIORITY);

//   //Register the AT app handler if neccesary
//   //VIS_SetAppHandlerApp(InstancePtr, VIS_AppHandler_SelfTest, InstancePtr);


//   ///Do the normal setup of the AT component

//   //Check device id (debug only)
//   #ifdef VIS_DEBUG_P
//   val=VIS_mReadReg(InstancePtr->BaseAddress, IDR_ADDR_OFFSET);
//   xil_printf("IDR: 0x%x\n\r", val);
//   #endif

//   //Check version number (debug only)
//   #ifdef VIS_DEBUG_P
//   val=VIS_mReadReg(InstancePtr->BaseAddress, VERR_ADDR_OFFSET);
//   xil_printf("VERR: 0x%x\n\r", val);
//   #endif

//   //Set value load reagister / count limit for counter register
//   #ifdef VIS_DEBUG_P
//   val=VIS_mReadReg(InstancePtr->BaseAddress, LR0_ADDR_OFFSET);
//   xil_printf("LR0: 0x%x\n\r", val);
//   #endif
//   VIS_mWriteReg(InstancePtr->BaseAddress, LR0_ADDR_OFFSET, LR0_10S_100MHZ); //LR0_40NS_100MHZ LR0_655US_100MHZ LR0_10S_100MHZ
//   #ifdef VIS_DEBUG_P
//   val=VIS_mReadReg(InstancePtr->BaseAddress, LR0_ADDR_OFFSET);
//   xil_printf("LR0: 0x%x\n\r", val);
//   #endif

//   //Set AT Special Control and Status Regsiter
//   //TB_ud0 <='1'; (5)
//   //TB_load0 <='0'; (4)
//   //TB_ent0_out <='1'; (2)
//   //VIS_mWriteReg(InstancePtr->BaseAddress, SCSR_ADDR_OFFSET, 0x00000024);
//   VIS_mWriteReg(InstancePtr->BaseAddress, SCSR_ADDR_OFFSET, SCSR_ENT0_OUT_MASK | SCSR_UD0_MASK);
//   #ifdef VIS_DEBUG_P
//   val=VIS_mReadReg(InstancePtr->BaseAddress, SCSR_ADDR_OFFSET);
//   xil_printf("SCSR: 0x%x\n\r", val);
//   #endif


//   ///Setup AT interrupt
//   //Enable AT IP interrupts
//   VIS_mWriteReg(InstancePtr->BaseAddress, IPIER_ADDR_OFFSET, IPIER_IPIE_MASK);
//   #ifdef VIS_DEBUG_P
//   val=VIS_mReadReg(InstancePtr->BaseAddress, IPIER_ADDR_OFFSET);
//   xil_printf("IPER: 0x%x\n\r", val);
//   #endif

//   //Enable AT global interrupt
//   VIS_mWriteReg(InstancePtr->BaseAddress, GIER_ADDR_OFFSET, GIER_GIE_MASK);
//   #ifdef VIS_DEBUG_P
//   val=VIS_mReadReg(InstancePtr->BaseAddress,  GIER_ADDR_OFFSET);
//   xil_printf("GIER: 0x%x\n\r", val);
//   #endif


//   ///Start the AT component / ipcore
//   //Check Global Control and Status Register before starting (debug only)
//   #ifdef VIS_DEBUG_P
//   val=VIS_mReadReg(InstancePtr->BaseAddress, GCSR_ADDR_OFFSET);
//   xil_printf("GCSR: 0x%x\n\r", val);
//   #endif
//   //Actual set of Global Control and Status Register (initiate start)
//   //TB_auto_restart<='0'; (7)
//   //TB_ap_start <='1'; (0)
//   VIS_mWriteReg(InstancePtr->BaseAddress, GCSR_ADDR_OFFSET, GCSR_START_MASK);

//   //Check Global Control and Status Register (debug only)
//   #ifdef VIS_DEBUG_P
//   val=VIS_mReadReg(InstancePtr->BaseAddress, GCSR_ADDR_OFFSET);
//   xil_printf("GCSR: 0x%x\n\r", val);
//   //Note this control read will cause hangup in polling loop if the LR0 value is too short!!!
//   #endif

//   while (1)
//   {
//     ///Sync Interrupt
//     val=VIS_WaitForInt(InstancePtr);

//     //IPISR value after interrupt (reset correctly?) (debug only)
//     #ifdef VIS_DEBUG_P
//     val=VIS_mReadReg(InstancePtr->BaseAddress, IPISR_ADDR_OFFSET);
//     xil_printf("Int sync done - IPISR: 0x%x\n\r", val);
//     #endif

//     ///Restart the AT component / ipcore
//     /*
//     //Check Global Control and Status Register
//     val=VIS_mReadReg(InstancePtr->BaseAddress, GCSR_ADDR_OFFSET);
//     #ifdef VIS_DEBUG_P
//     xil_printf("GCSR: 0x%x\n\r", val);
//     #endif
//     */
//     //TB_auto_restart<='0'; (7)
//     //TB_ap_start <='1'; (0)
//     VIS_mWriteReg(InstancePtr->BaseAddress, GCSR_ADDR_OFFSET, GCSR_START_MASK);

//     //Check Global Control and Status Register (debug only)
//     /*
//     #ifdef VIS_DEBUG_P
//     val=VIS_mReadReg(InstancePtr->BaseAddress, GCSR_ADDR_OFFSET);
//     xil_printf("GCSR: 0x%x\n\r", val);
//     //Note this control read will cause hangup in polling loop if the LR0 value is too short!!!
//     #endif
//     */
//   }

//   ///Do wrap up / checks after sync interrupt

//   ///disable AT Interrupt again? - not necessary in this case here
//   /*
//   //Disable IP specific part
//     //Disable AT global interrupt
//     val=VIS_mReadReg(InstancePtr->BaseAddress, GIER_ADDR_OFFSET);
//     VIS_mWriteReg(InstancePtr->BaseAddress, GIER_ADDR_OFFSET, (val & ~GIER_GIE_MASK));
//     #ifdef VIS_DEBUG_P
//     val=VIS_mReadReg(InstancePtr->BaseAddress,  GIER_ADDR_OFFSET);
//     xil_printf("GIER: 0x%x\n\r", val);
//     #endif


//     //Disable AT IP interrupts
//     val=VIS_mReadReg(InstancePtr->BaseAddress, GIER_ADDR_OFFSET);
//     VIS_mWriteReg(InstancePtr->BaseAddress, IPIER_ADDR_OFFSET, (val & ~IPIER_IPIE_MASK));
//     #ifdef VIS_DEBUG_P
//     val=VIS_mReadReg(InstancePtr->BaseAddress, IPIER_ADDR_OFFSET);
//     xil_printf("IPER: 0x%x\n\r", val);
//     #endif

//   //Disable platform specific part (xinterrupt_wrap.h/c in
//   //  platform/microblaze_0/standalone_microblaze_0/bsp/libsrc/standalone/src/common/intr/xinterupt_wrap.c/h)
//     //Deregister interrupt handler in interrupt controller
//     XDisableIntrId(InstancePtr->Config.IntrId, InstancePtr->Config.IntrParent);
//   */

//   //check if done and start reset is done (debug only)
//   #ifdef VIS_DEBUG_P
//   val=VIS_mReadReg(InstancePtr->BaseAddress, GCSR_ADDR_OFFSET);
//   xil_printf("GCSR: 0x%x\n\r", val);
//   #endif
//   #ifdef VIS_DEBUG_P
//   val=VIS_mReadReg(InstancePtr->BaseAddress, GCSR_ADDR_OFFSET);
//   xil_printf("GCSR: 0x%x\n\r", val);
//   #endif

//   //check value counter register (debug only)
//   #ifdef VIS_DEBUG_P
//   val=VIS_mReadReg(InstancePtr->BaseAddress, CR0_ADDR_OFFSET);
//   xil_printf("CR0: 0x%x\n\r", val);
//   #endif

//   if (Status != XST_SUCCESS) {
// 		xil_printf("VIS_TestUP_int failed\r\n");
// 		return XST_FAILURE;
// 	}

// 	xil_printf("VIS_TestUP_int passed\n\r");
// 	return XST_SUCCESS;

//   //##INSERT YOUR CODE HERE END
// }

//##INSERT YOUR CODE HERE END


/*****************************************************************************
 * Core selftests (originally vis_core_selftest.c)
 *****************************************************************************/

/*
 * Compare two char arrays of length len.
 * Prints the first mismatching position and returns XST_FAILURE.
 */
static XStatus check_str(const char *expected, const char *actual,
                          u8 len, const char *test_name)
{
    u8 i;
    for (i = 0; i < len; i++) {
        if (expected[i] != actual[i]) {
            xil_printf("  FAIL %s: [%u] expected 0x%02x ('%c') got 0x%02x ('%c')\n\r",
                       test_name, (unsigned)i,
                       (unsigned)(u8)expected[i], expected[i],
                       (unsigned)(u8)actual[i],   actual[i]);
            return XST_FAILURE;
        }
    }
    return XST_SUCCESS;
}

/*
 * Check a single bar-width result and print on mismatch.
 */
static XStatus check_bar_width(uint16_t rms, u8 expected, const char *label)
{
    u8 got = VIS_Core_CalcBarWidth(rms);
    if (got != expected) {
        xil_printf("  FAIL %s: rms=%u -> expected width %u, got %u\n\r",
                   label, (unsigned)rms, (unsigned)expected, (unsigned)got);
        return XST_FAILURE;
    }
    return XST_SUCCESS;
}


/*
 * Unit test: VIS_Core_FormatRmsText
 *
 * Verifies the 17-character output "  Level: XXX.YY %" for boundary
 * values of the 7.2 fixed-point input.
 * No hardware required.
 */
XStatus VIS_Core_TestFormat(void)
{
    XStatus Status = XST_SUCCESS, tmp;
    char buf[18];

    xil_printf("******************************\n\r");
    xil_printf("* VIS_CORE_TESTFORMAT\n\r");
    xil_printf("******************************\n\r");

    /* All four fractional variants at integer 0 */
    VIS_Core_FormatRmsText(0, buf);
    tmp = check_str("  Level: 000.00 %", buf, 17, "rms=0 (0.00%)");
    Status |= tmp;

    VIS_Core_FormatRmsText(1, buf);
    tmp = check_str("  Level: 000.25 %", buf, 17, "rms=1 (0.25%)");
    Status |= tmp;

    VIS_Core_FormatRmsText(2, buf);
    tmp = check_str("  Level: 000.50 %", buf, 17, "rms=2 (0.50%)");
    Status |= tmp;

    VIS_Core_FormatRmsText(3, buf);
    tmp = check_str("  Level: 000.75 %", buf, 17, "rms=3 (0.75%)");
    Status |= tmp;

    /* Integer increment: 4 = 1.00% */
    VIS_Core_FormatRmsText(4, buf);
    tmp = check_str("  Level: 001.00 %", buf, 17, "rms=4 (1.00%)");
    Status |= tmp;

    /* Exact 100% */
    VIS_Core_FormatRmsText(400, buf);
    tmp = check_str("  Level: 100.00 %", buf, 17, "rms=400 (100.00%)");
    Status |= tmp;

    /* Maximum representable value: 127.75% */
    VIS_Core_FormatRmsText(511, buf);
    tmp = check_str("  Level: 127.75 %", buf, 17, "rms=511 (127.75%)");
    Status |= tmp;

    if (Status == XST_SUCCESS)
        xil_printf("VIS_CORE_TESTFORMAT passed\n\r");
    else
        xil_printf("VIS_CORE_TESTFORMAT FAILED\n\r");

    return Status;
}


/*
 * Unit test: VIS_Core_CalcBarWidth
 *
 * Verifies bar column count for key points in the 0-511 input range.
 * Expected: 100% (rms=400) fills all 80 columns; values above 100% are capped.
 * No hardware required.
 */
XStatus VIS_Core_TestBarWidth(void)
{
    XStatus Status = XST_SUCCESS, tmp;

    xil_printf("******************************\n\r");
    xil_printf("* VIS_CORE_TESTBARWIDTH\n\r");
    xil_printf("******************************\n\r");

    /* Silence */
    tmp = check_bar_width(0,   0,  "rms=0   (0%)");
    Status |= tmp;

    /* 25%: 100 * 80 / 400 = 20 */
    tmp = check_bar_width(100, 20, "rms=100 (25%)");
    Status |= tmp;

    /* 50%: 200 * 80 / 400 = 40 */
    tmp = check_bar_width(200, 40, "rms=200 (50%)");
    Status |= tmp;

    /* Just below 100%: 399 * 80 / 400 = 79 (integer division) */
    tmp = check_bar_width(399, 79, "rms=399 (~100%)");
    Status |= tmp;

    /* Exact 100%: cap triggers, returns 80 */
    tmp = check_bar_width(400, 80, "rms=400 (100%)");
    Status |= tmp;

    /* Over-range: capped at 80 */
    tmp = check_bar_width(511, 80, "rms=511 (>100%)");
    Status |= tmp;

    if (Status == XST_SUCCESS)
        xil_printf("VIS_CORE_TESTBARWIDTH passed\n\r");
    else
        xil_printf("VIS_CORE_TESTBARWIDTH FAILED\n\r");

    return Status;
}


/*
 * Integration test: VIS_Core_RenderLoudness (scrolling bar chart)
 *
 * Each call appends one column to the 80-column ring buffer and redraws the
 * full display. Requires display hardware; visual output is the final check.
 *
 * Expected on screen after this test:
 *   col 0-73: empty (rms=0, no bar)
 *   col 74:   ~7 rows green   (rms=100, 25%)
 *   col 75:   15 rows green   (rms=200, 50%)
 *   col 76:   18 rows yellow  (rms=240, 60%)
 *   col 77:   24 rows red     (rms=320, 80%)
 *   col 78:   30 rows red     (rms=400, 100%)
 *   col 79:   30 rows red     (rms=511, >100%, capped)
 */
XStatus VIS_Core_TestRender(VIS_Data *InstancePtr)
{
    xil_printf("******************************\n\r");
    xil_printf("* VIS_CORE_TESTRENDER\n\r");
    xil_printf("******************************\n\r");

    VIS_Core_Clear(InstancePtr);

    VIS_Core_RenderLoudness(InstancePtr, 0);
    xil_printf("  col+1: rms=0   (0.00%%  - empty column)\n\r");

    VIS_Core_RenderLoudness(InstancePtr, 100);
    xil_printf("  col+2: rms=100 (25.00%% - short green column)\n\r");

    VIS_Core_RenderLoudness(InstancePtr, 200);
    xil_printf("  col+3: rms=200 (50.00%% - mid green column)\n\r");

    VIS_Core_RenderLoudness(InstancePtr, 240);
    xil_printf("  col+4: rms=240 (60.00%% - enters yellow zone)\n\r");

    VIS_Core_RenderLoudness(InstancePtr, 320);
    xil_printf("  col+5: rms=320 (80.00%% - enters red zone)\n\r");

    VIS_Core_RenderLoudness(InstancePtr, 400);
    xil_printf("  col+6: rms=400 (100.00%% - full height column)\n\r");

    VIS_Core_RenderLoudness(InstancePtr, 511);
    xil_printf("  col+7: rms=511 (127.75%% - over-range, capped at full height)\n\r");

    xil_printf("VIS_CORE_TESTRENDER passed (verify display output visually)\n\r");
    return XST_SUCCESS;
}


/*
 * Hardware protocol test: STATUS.FDP asserts within one frame period.
 *
 * Clears any pending FDP, writes one character to trigger a display update,
 * then polls STATUS.FDP with a timeout (~3 frame periods at 60 Hz / 100 MHz).
 * Returns XST_FAILURE if FDP never arrives, so a broken sync line is caught
 * as a test failure rather than an infinite hang.
 * Requires display hardware.
 */
XStatus VIS_Core_TestFDP(VIS_Data *InstancePtr)
{
    uint32_t timeout = 5000000u;

    xil_printf("******************************\n\r");
    xil_printf("* VIS_CORE_TESTFDP\n\r");
    xil_printf("******************************\n\r");

    /* Clear any FDP left over from a previous operation */
    VIS_mWriteReg(InstancePtr->BaseAddress, IPISR_ADDR_OFFSET, IPISR_FDP_MASK);

    /* Write one character — this sets CTRL_WD and triggers the hardware */
    VIS_WriteChar(InstancePtr, 0, 0, (u8)'*',
                  VIS_CORE_CR_WHITE, VIS_CORE_CG_WHITE, VIS_CORE_CB_WHITE);

    /* Poll with timeout instead of spinning forever */
    while ((VIS_mReadReg(InstancePtr->BaseAddress, STATUS_ADDR_OFFSET) & STATUS_FDP_MASK) == 0) {
        if (--timeout == 0) {
            xil_printf("  FAIL: STATUS.FDP never asserted (display hardware not responding)\n\r");
            return XST_FAILURE;
        }
    }
    VIS_mWriteReg(InstancePtr->BaseAddress, IPISR_ADDR_OFFSET, IPISR_FDP_MASK);

    xil_printf("VIS_CORE_TESTFDP passed\n\r");
    return XST_SUCCESS;
}


/*
 * Ring buffer wrap test: fill all 80 columns once with a rising ramp.
 *
 * Calls VIS_Core_RenderLoudness exactly VIS_CORE_COLS (80) times, driving
 * rms linearly from 0 to 100% (400). After the last call the ring buffer
 * has wrapped once and the display should show a smooth staircase rising
 * from left (empty) to right (full height).
 * Requires display hardware; visual output is the final check.
 */
XStatus VIS_Core_TestScrolling(VIS_Data *InstancePtr)
{
    u8 i;

    xil_printf("******************************\n\r");
    xil_printf("* VIS_CORE_TESTSCROLLING\n\r");
    xil_printf("******************************\n\r");

    VIS_Core_Clear(InstancePtr);

    for (i = 0; i < VIS_CORE_COLS; i++) {
        uint16_t rms = (uint16_t)((u32)i * VIS_CORE_RMS_100PCT / (VIS_CORE_COLS - 1U));
        VIS_Core_RenderLoudness(InstancePtr, rms);
    }

    xil_printf("VIS_CORE_TESTSCROLLING passed (verify rising staircase on display)\n\r");
    return XST_SUCCESS;
}
//end edit Maximilian Hafeneder / Nicolas Lonthoff
