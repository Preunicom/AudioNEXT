//##INSERT YOUR CODE HERE 

//begin Maximilian Hafeneder
/****************** Include Files ********************/
#include "vis_driver.h"
#include "vis_driver_i.h"
#include "vis_driver_int.h"
#include "vis_selftest_int.h"

/************************** Constant Definitions ***************************/


/**************************** Type Definitions *****************************/


/************************** Variable Definitions ***************************/


/************************** Function Definitions ***************************/
void VIS_AppHandler_SelfTest(void *CallBackRef)
{
	VIS_Data *InstancePtr = (VIS_Data *)CallBackRef;

  //Application specific handler called by VIS_InterruptHandler
  //here do nothing so far ...
  
}

// XStatus AT_TestUp_int(AT_Data *InstancePtr){

//   int Status=XST_SUCCESS;
//   uint32_t val=0;
  
//   xil_printf("******************************\n\r");
// 	xil_printf("*AT_TESTUP_int\n\r");
// 	xil_printf("******************************\n\r");

//   ///Setup interrupt
//   Status = XSetupInterruptSystem(InstancePtr, (XInterruptHandler)AT_InterruptHandler, \
// 				       InstancePtr->Config.IntrId, InstancePtr->Config.IntrParent, \
// 				       XINTERRUPT_DEFAULT_PRIORITY);
  
//   //Register the AT app handler if neccesary
//   //AT_SetAppHandlerApp(InstancePtr, AT_AppHandler_SelfTest, InstancePtr);
  
  
//   ///Do the normal setup of the AT component
  
//   //Check device id (debug only)
//   #ifdef AT_DEBUG_P
//   val=AT_mReadReg(InstancePtr->BaseAddress, IDR_ADDR_OFFSET);
//   xil_printf("IDR: 0x%x\n\r", val);
//   #endif

//   //Check version number (debug only) 
//   #ifdef AT_DEBUG_P
//   val=AT_mReadReg(InstancePtr->BaseAddress, VERR_ADDR_OFFSET);
//   xil_printf("VERR: 0x%x\n\r", val);
//   #endif

//   //Set value load reagister / count limit for counter register
//   #ifdef AT_DEBUG_P
//   val=AT_mReadReg(InstancePtr->BaseAddress, LR0_ADDR_OFFSET);
//   xil_printf("LR0: 0x%x\n\r", val);
//   #endif
//   AT_mWriteReg(InstancePtr->BaseAddress, LR0_ADDR_OFFSET, LR0_10S_100MHZ); //LR0_40NS_100MHZ LR0_655US_100MHZ LR0_10S_100MHZ     
//   #ifdef AT_DEBUG_P
//   val=AT_mReadReg(InstancePtr->BaseAddress, LR0_ADDR_OFFSET);
//   xil_printf("LR0: 0x%x\n\r", val);
//   #endif

//   //Set AT Special Control and Status Register
//   //TB_ud0 <='1'; (5)
//   //TB_load0 <='0'; (4)
//   //TB_ent0_out <='1'; (2)  
//   //AT_mWriteReg(InstancePtr->BaseAddress, SCSR_ADDR_OFFSET, 0x00000024);
//   AT_mWriteReg(InstancePtr->BaseAddress, SCSR_ADDR_OFFSET, SCSR_ENT0_OUT_MASK | SCSR_UD0_MASK);
//   #ifdef AT_DEBUG_P
//   val=AT_mReadReg(InstancePtr->BaseAddress, SCSR_ADDR_OFFSET);
//   xil_printf("SCSR: 0x%x\n\r", val);
//   #endif


//   ///Setup AT interrupt
//   //Enable AT IP interrupts
//   AT_mWriteReg(InstancePtr->BaseAddress, IPIER_ADDR_OFFSET, IPIER_IPIE_MASK);
//   #ifdef AT_DEBUG_P
//   val=AT_mReadReg(InstancePtr->BaseAddress, IPIER_ADDR_OFFSET);
//   xil_printf("IPER: 0x%x\n\r", val);
//   #endif
  
//   //Enable AT global interrupt
//   AT_mWriteReg(InstancePtr->BaseAddress, GIER_ADDR_OFFSET, GIER_GIE_MASK);
//   #ifdef AT_DEBUG_P
//   val=AT_mReadReg(InstancePtr->BaseAddress,  GIER_ADDR_OFFSET);
//   xil_printf("GIER: 0x%x\n\r", val);
//   #endif

  
//   ///Start the AT component / ipcore
//   //Check Global Control and Status Register before starting (debug only)
//   #ifdef AT_DEBUG_P
//   val=AT_mReadReg(InstancePtr->BaseAddress, GCSR_ADDR_OFFSET);   
//   xil_printf("GCSR: 0x%x\n\r", val); 
//   #endif
//   //Actual set of Global Control and Status Register (initiate start)
//   //TB_auto_restart<='0'; (7)    
//   //TB_ap_start <='1'; (0)
//   AT_mWriteReg(InstancePtr->BaseAddress, GCSR_ADDR_OFFSET, GCSR_START_MASK);

//   //Check Global Control and Status Register (debug only)
//   #ifdef AT_DEBUG_P
//   val=AT_mReadReg(InstancePtr->BaseAddress, GCSR_ADDR_OFFSET);
//   xil_printf("GCSR: 0x%x\n\r", val);
//   //Note this control read will cause hangup in polling loop if the LR0 value is too short!!!
//   #endif
     

//   ///Sync Interrupt 
//   val=AT_WaitForInt(InstancePtr);

//   //Check IPISR value after interrupt (reset correctly?) (debug only)  
//   #ifdef AT_DEBUG_P
//   val=AT_mReadReg(InstancePtr->BaseAddress, IPISR_ADDR_OFFSET);
//   xil_printf("Int sync done - IPISR: 0x%x\n\r", val); 
//   #endif

  
   
//   ///Do wrap up / checks after sync interrupt
  
//   //disable AT Interrupt again? - not necessary in this case here
//   /*
//   //Disable IP specific part
//     //Disable AT global interrupt
//     val=AT_mReadReg(InstancePtr->BaseAddress, GIER_ADDR_OFFSET);
//     AT_mWriteReg(InstancePtr->BaseAddress, GIER_ADDR_OFFSET, (val & ~GIER_GIE_MASK));
//     #ifdef AT_DEBUG_P
//     val=AT_mReadReg(InstancePtr->BaseAddress,  GIER_ADDR_OFFSET);
//     xil_printf("GIER: 0x%x\n\r", val);
//     #endif


//     //Disable AT IP interrupts
//     val=AT_mReadReg(InstancePtr->BaseAddress, GIER_ADDR_OFFSET);
//     AT_mWriteReg(InstancePtr->BaseAddress, IPIER_ADDR_OFFSET, (val & ~IPIER_IPIE_MASK));
//     #ifdef AT_DEBUG_P
//     val=AT_mReadReg(InstancePtr->BaseAddress, IPIER_ADDR_OFFSET);
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
//   val=AT_mReadReg(InstancePtr->BaseAddress, GCSR_ADDR_OFFSET);
//   #ifdef AT_DEBUG_P   
//   xil_printf("GCSR: 0x%x\n\r", val);
//   #endif
//   //check again if done and start reset is accomplished (debug only)
//   #ifdef AT_DEBUG_P
//   val=AT_mReadReg(InstancePtr->BaseAddress, GCSR_ADDR_OFFSET); 
//   xil_printf("GCSR: 0x%x\n\r", val);
//   #endif
  
//   //check value counter register (debug only)  
//   #ifdef AT_DEBUG_P
//   val=AT_mReadReg(InstancePtr->BaseAddress, CR0_ADDR_OFFSET);
//   xil_printf("CR0: 0x%x\n\r", val);
//   #endif

   
//   if (Status != XST_SUCCESS) {
// 		xil_printf("AT_TestUP_int failed\r\n");
// 		return XST_FAILURE;
// 	}

// 	xil_printf("AT_TestUP_int passed\n\r");
// 	return XST_SUCCESS;
// }  


// XStatus AT_TestUpAutoRestart_int(AT_Data *InstancePtr){

// //##INSERT YOUR CODE HERE 

// int Status=XST_SUCCESS;
//   uint32_t val=0;
  
//   xil_printf("******************************\n\r");
// 	xil_printf("*AT_TESTUPAUTORESTART_int\n\r");
// 	xil_printf("******************************\n\r");

//   ///Setup interrupts
//   Status = XSetupInterruptSystem(InstancePtr, (XInterruptHandler)AT_InterruptHandler, \
// 				       InstancePtr->Config.IntrId, InstancePtr->Config.IntrParent, \
// 				       XINTERRUPT_DEFAULT_PRIORITY);

//   //Register the AT app handler if neccesary
//   //AT_SetAppHandlerApp(InstancePtr, AT_AppHandler_SelfTest, InstancePtr);
  
  
//   ///Do the normal setup of the AT component
  
//   //Check device id (debug only)
//   #ifdef AT_DEBUG_P
//   val=AT_mReadReg(InstancePtr->BaseAddress, IDR_ADDR_OFFSET);
//   xil_printf("IDR: 0x%x\n\r", val);
//   #endif

//   //Check version number (debug only) 
//   #ifdef AT_DEBUG_P
//   val=AT_mReadReg(InstancePtr->BaseAddress, VERR_ADDR_OFFSET);
//   xil_printf("VERR: 0x%x\n\r", val);
//   #endif

//   //Set value load reagister / count limit for counter register
//   #ifdef AT_DEBUG_P
//   val=AT_mReadReg(InstancePtr->BaseAddress, LR0_ADDR_OFFSET);
//   xil_printf("LR0: 0x%x\n\r", val);
//   #endif
//   AT_mWriteReg(InstancePtr->BaseAddress, LR0_ADDR_OFFSET, LR0_10S_100MHZ); //LR0_40NS_100MHZ LR0_655US_100MHZ LR0_10S_100MHZ     
//   #ifdef AT_DEBUG_P
//   val=AT_mReadReg(InstancePtr->BaseAddress, LR0_ADDR_OFFSET);
//   xil_printf("LR0: 0x%x\n\r", val);
//   #endif

//   //Set AT Special Control and Status Regsiter
//   //TB_ud0 <='1'; (5)
//   //TB_load0 <='0'; (4)
//   //TB_ent0_out <='1'; (2)
//   //AT_mWriteReg(InstancePtr->BaseAddress, SCSR_ADDR_OFFSET, 0x00000024);
//   AT_mWriteReg(InstancePtr->BaseAddress, SCSR_ADDR_OFFSET, SCSR_ENT0_OUT_MASK | SCSR_UD0_MASK);
//   #ifdef AT_DEBUG_P
//   val=AT_mReadReg(InstancePtr->BaseAddress, SCSR_ADDR_OFFSET);
//   xil_printf("SCSR: 0x%x\n\r", val);
//   #endif


//   ///Setup AT interrupt
//   //Enable AT IP interrupts
//   AT_mWriteReg(InstancePtr->BaseAddress, IPIER_ADDR_OFFSET, IPIER_IPIE_MASK);
//   #ifdef AT_DEBUG_P
//   val=AT_mReadReg(InstancePtr->BaseAddress, IPIER_ADDR_OFFSET);
//   xil_printf("IPER: 0x%x\n\r", val);
//   #endif
  
//   //Enable AT global interrupt
//   AT_mWriteReg(InstancePtr->BaseAddress, GIER_ADDR_OFFSET, GIER_GIE_MASK);
//   #ifdef AT_DEBUG_P
//   val=AT_mReadReg(InstancePtr->BaseAddress,  GIER_ADDR_OFFSET);
//   xil_printf("GIER: 0x%x\n\r", val);
//   #endif

  
//   ///Start the AT component / ipcore
//   //Check Global Control and Status Register before starting (debug only)
//   #ifdef AT_DEBUG_P
//   val=AT_mReadReg(InstancePtr->BaseAddress, GCSR_ADDR_OFFSET);   
//   xil_printf("GCSR: 0x%x\n\r", val); 
//   #endif
//   //Actual set of Global Control and Status Register (initiate start)
//   //TB_auto_restart<='0'; (7)    
//   //TB_ap_start <='1'; (0)
//   AT_mWriteReg(InstancePtr->BaseAddress, GCSR_ADDR_OFFSET, GCSR_START_MASK);

//   //Check Global Control and Status Register (debug only)
//   #ifdef AT_DEBUG_P
//   val=AT_mReadReg(InstancePtr->BaseAddress, GCSR_ADDR_OFFSET);
//   xil_printf("GCSR: 0x%x\n\r", val);
//   //Note this control read will cause hangup in polling loop if the LR0 value is too short!!!
//   #endif
     

//   while (1)
//   {
//     ///Sync Interrupt 
//     val=AT_WaitForInt(InstancePtr);
      
//     //Check IPISR value after interrupt (reset correctly?) (debug only)  
//     #ifdef AT_DEBUG_P
//     val=AT_mReadReg(InstancePtr->BaseAddress, IPISR_ADDR_OFFSET);
//     xil_printf("Int sync done - IPISR: 0x%x\n\r", val); 
//     #endif

//     ///Restart the AT component / ipcore
//     /*
//     //Check Global Control and Status Register 
//     val=AT_mReadReg(InstancePtr->BaseAddress, GCSR_ADDR_OFFSET);   
//     #ifdef AT_DEBUG_P    
//     xil_printf("GCSR: 0x%x\n\r", val); 
//     #endif
//     */
//     //TB_auto_restart<='0'; (7)    
//     //TB_ap_start <='1'; (0)
//     //AT_mWriteReg(InstancePtr->BaseAddress, GCSR_ADDR_OFFSET, GCSR_START_MASK);

//     //Check Global Control and Status Register (debug only)
//     /*
//     #ifdef AT_DEBUG_P
//     val=AT_mReadReg(InstancePtr->BaseAddress, GCSR_ADDR_OFFSET);
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
//     val=AT_mReadReg(InstancePtr->BaseAddress, GIER_ADDR_OFFSET);
//     AT_mWriteReg(InstancePtr->BaseAddress, GIER_ADDR_OFFSET, (val & ~GIER_GIE_MASK));
//     #ifdef AT_DEBUG_P
//     val=AT_mReadReg(InstancePtr->BaseAddress,  GIER_ADDR_OFFSET);
//     xil_printf("GIER: 0x%x\n\r", val);
//     #endif


//     //Disable AT IP interrupts
//     val=AT_mReadReg(InstancePtr->BaseAddress, GIER_ADDR_OFFSET);
//     AT_mWriteReg(InstancePtr->BaseAddress, IPIER_ADDR_OFFSET, (val & ~IPIER_IPIE_MASK));
//     #ifdef AT_DEBUG_P
//     val=AT_mReadReg(InstancePtr->BaseAddress, IPIER_ADDR_OFFSET);
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
//   val=AT_mReadReg(InstancePtr->BaseAddress, GCSR_ADDR_OFFSET);
//   #ifdef AT_DEBUG_P   
//   xil_printf("GCSR: 0x%x\n\r", val);
//   #endif
//   //check again if done and start reset is accomplished (debug only)
//   #ifdef AT_DEBUG_P
//   val=AT_mReadReg(InstancePtr->BaseAddress, GCSR_ADDR_OFFSET); 
//   xil_printf("GCSR: 0x%x\n\r", val);
//   #endif
  
//   //check value counter register (debug only)  
//   #ifdef AT_DEBUG_P
//   val=AT_mReadReg(InstancePtr->BaseAddress, CR0_ADDR_OFFSET);
//   xil_printf("CR0: 0x%x\n\r", val);
//   #endif

   
//   if (Status != XST_SUCCESS) {
// 		xil_printf("AT_TestUpAutoRestart_int dailed\r\n");
// 		return XST_FAILURE;
// 	}

// 	xil_printf("AT_TestUpAutoRestart_int passed\n\r");
// 	return XST_SUCCESS;

//   //##INSERT YOUR CODE HERE END
// }  
  
  
// XStatus AT_TestUp_int_AutorestartSW(AT_Data *InstancePtr)
// {
//   //##INSERT YOUR CODE HERE 

//   int Status=XST_SUCCESS;
//   uint32_t val=0;
  
//   xil_printf("******************************\n\r");
// 	xil_printf("*AT_TestUp_int_AutorestartSW\n\r");
// 	xil_printf("******************************\n\r");

//   ///Setup interrupts
//   Status = XSetupInterruptSystem(InstancePtr, (XInterruptHandler)AT_InterruptHandler, \
// 				       InstancePtr->Config.IntrId, InstancePtr->Config.IntrParent, \
// 				       XINTERRUPT_DEFAULT_PRIORITY);
  
//   //Register the AT app handler if neccesary
//   //AT_SetAppHandlerApp(InstancePtr, AT_AppHandler_SelfTest, InstancePtr);
  
  
//   ///Do the normal setup of the AT component
  
//   //Check device id (debug only)
//   #ifdef AT_DEBUG_P
//   val=AT_mReadReg(InstancePtr->BaseAddress, IDR_ADDR_OFFSET);
//   xil_printf("IDR: 0x%x\n\r", val);
//   #endif

//   //Check version number (debug only) 
//   #ifdef AT_DEBUG_P
//   val=AT_mReadReg(InstancePtr->BaseAddress, VERR_ADDR_OFFSET);
//   xil_printf("VERR: 0x%x\n\r", val);
//   #endif

//   //Set value load reagister / count limit for counter register
//   #ifdef AT_DEBUG_P
//   val=AT_mReadReg(InstancePtr->BaseAddress, LR0_ADDR_OFFSET);
//   xil_printf("LR0: 0x%x\n\r", val);
//   #endif
//   AT_mWriteReg(InstancePtr->BaseAddress, LR0_ADDR_OFFSET, LR0_10S_100MHZ); //LR0_40NS_100MHZ LR0_655US_100MHZ LR0_10S_100MHZ     
//   #ifdef AT_DEBUG_P
//   val=AT_mReadReg(InstancePtr->BaseAddress, LR0_ADDR_OFFSET);
//   xil_printf("LR0: 0x%x\n\r", val);
//   #endif

//   //Set AT Special Control and Status Regsiter
//   //TB_ud0 <='1'; (5)
//   //TB_load0 <='0'; (4)
//   //TB_ent0_out <='1'; (2)
//   //AT_mWriteReg(InstancePtr->BaseAddress, SCSR_ADDR_OFFSET, 0x00000024);
//   AT_mWriteReg(InstancePtr->BaseAddress, SCSR_ADDR_OFFSET, SCSR_ENT0_OUT_MASK | SCSR_UD0_MASK);
//   #ifdef AT_DEBUG_P
//   val=AT_mReadReg(InstancePtr->BaseAddress, SCSR_ADDR_OFFSET);
//   xil_printf("SCSR: 0x%x\n\r", val);
//   #endif


//   ///Setup AT interrupt
//   //Enable AT IP interrupts
//   AT_mWriteReg(InstancePtr->BaseAddress, IPIER_ADDR_OFFSET, IPIER_IPIE_MASK);
//   #ifdef AT_DEBUG_P
//   val=AT_mReadReg(InstancePtr->BaseAddress, IPIER_ADDR_OFFSET);
//   xil_printf("IPER: 0x%x\n\r", val);
//   #endif
  
//   //Enable AT global interrupt
//   AT_mWriteReg(InstancePtr->BaseAddress, GIER_ADDR_OFFSET, GIER_GIE_MASK);
//   #ifdef AT_DEBUG_P
//   val=AT_mReadReg(InstancePtr->BaseAddress,  GIER_ADDR_OFFSET);
//   xil_printf("GIER: 0x%x\n\r", val);
//   #endif

  
//   ///Start the AT component / ipcore
//   //Check Global Control and Status Register before starting (debug only)
//   #ifdef AT_DEBUG_P
//   val=AT_mReadReg(InstancePtr->BaseAddress, GCSR_ADDR_OFFSET);   
//   xil_printf("GCSR: 0x%x\n\r", val); 
//   #endif
//   //Actual set of Global Control and Status Register (initiate start)
//   //TB_auto_restart<='0'; (7)    
//   //TB_ap_start <='1'; (0)
//   AT_mWriteReg(InstancePtr->BaseAddress, GCSR_ADDR_OFFSET, GCSR_START_MASK);

//   //Check Global Control and Status Register (debug only)
//   #ifdef AT_DEBUG_P
//   val=AT_mReadReg(InstancePtr->BaseAddress, GCSR_ADDR_OFFSET);
//   xil_printf("GCSR: 0x%x\n\r", val);
//   //Note this control read will cause hangup in polling loop if the LR0 value is too short!!!
//   #endif
     
//   while (1)
//   {
//     ///Sync Interrupt 
//     val=AT_WaitForInt(InstancePtr);
      
//     //IPISR value after interrupt (reset correctly?) (debug only)  
//     #ifdef AT_DEBUG_P
//     val=AT_mReadReg(InstancePtr->BaseAddress, IPISR_ADDR_OFFSET);
//     xil_printf("Int sync done - IPISR: 0x%x\n\r", val); 
//     #endif

//     ///Restart the AT component / ipcore
//     /*
//     //Check Global Control and Status Register 
//     val=AT_mReadReg(InstancePtr->BaseAddress, GCSR_ADDR_OFFSET);   
//     #ifdef AT_DEBUG_P    
//     xil_printf("GCSR: 0x%x\n\r", val); 
//     #endif
//     */
//     //TB_auto_restart<='0'; (7)    
//     //TB_ap_start <='1'; (0)
//     AT_mWriteReg(InstancePtr->BaseAddress, GCSR_ADDR_OFFSET, GCSR_START_MASK);

//     //Check Global Control and Status Register (debug only)
//     /*
//     #ifdef AT_DEBUG_P
//     val=AT_mReadReg(InstancePtr->BaseAddress, GCSR_ADDR_OFFSET);
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
//     val=AT_mReadReg(InstancePtr->BaseAddress, GIER_ADDR_OFFSET);
//     AT_mWriteReg(InstancePtr->BaseAddress, GIER_ADDR_OFFSET, (val & ~GIER_GIE_MASK));
//     #ifdef AT_DEBUG_P
//     val=AT_mReadReg(InstancePtr->BaseAddress,  GIER_ADDR_OFFSET);
//     xil_printf("GIER: 0x%x\n\r", val);
//     #endif


//     //Disable AT IP interrupts
//     val=AT_mReadReg(InstancePtr->BaseAddress, GIER_ADDR_OFFSET);
//     AT_mWriteReg(InstancePtr->BaseAddress, IPIER_ADDR_OFFSET, (val & ~IPIER_IPIE_MASK));
//     #ifdef AT_DEBUG_P
//     val=AT_mReadReg(InstancePtr->BaseAddress, IPIER_ADDR_OFFSET);
//     xil_printf("IPER: 0x%x\n\r", val);
//     #endif

//   //Disable platform specific part (xinterrupt_wrap.h/c in  
//   //  platform/microblaze_0/standalone_microblaze_0/bsp/libsrc/standalone/src/common/intr/xinterupt_wrap.c/h)
//     //Deregister interrupt handler in interrupt controller
//     XDisableIntrId(InstancePtr->Config.IntrId, InstancePtr->Config.IntrParent); 
//   */
    
//   //check if done and start reset is done (debug only)
//   #ifdef AT_DEBUG_P
//   val=AT_mReadReg(InstancePtr->BaseAddress, GCSR_ADDR_OFFSET); 
//   xil_printf("GCSR: 0x%x\n\r", val);
//   #endif
//   #ifdef AT_DEBUG_P
//   val=AT_mReadReg(InstancePtr->BaseAddress, GCSR_ADDR_OFFSET); 
//   xil_printf("GCSR: 0x%x\n\r", val);
//   #endif
    
//   //check value counter register (debug only)  
//   #ifdef AT_DEBUG_P
//   val=AT_mReadReg(InstancePtr->BaseAddress, CR0_ADDR_OFFSET);
//   xil_printf("CR0: 0x%x\n\r", val);
//   #endif
   
//   if (Status != XST_SUCCESS) {
// 		xil_printf("AT_TestUP_int failed\r\n");
// 		return XST_FAILURE;
// 	}

// 	xil_printf("AT_TestUP_int passed\n\r");
// 	return XST_SUCCESS;

//   //##INSERT YOUR CODE HERE END
// }    

//end edit Maximilian Hafeneder
//##INSERT YOUR CODE HERE END