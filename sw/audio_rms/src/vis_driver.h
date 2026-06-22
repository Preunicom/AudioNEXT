
//begin edit Maximilian Hafeneder / Nicolas Lonthoff
#ifndef VIS_DRIVER_H
#define VIS_DRIVER_H

/****************** Include Files ********************/
#include <stdint.h>
#include "xil_types.h"
#include "xstatus.h"
#include "xil_printf.h"
#include "xparameters.h"

#include "vis_driver_l.h" 


/************************** Constant Definitions ***************************/
///Debug Printout Mode
#define VIS_DEBUG_P 1


///Regsiter definitions and mask values
//Define base address
#define VIS_BASEADDRESS XPAR_VISUALIZATION_0_BASEADDR

//GCSR General/Global Control and Status Register slv_reg00 0x00 (all bits reserved)
#define GCSR_ADDR_OFFSET       0x000
//GIER Global Interrupt Enable slv_reg01 0x04
#define GIER_ADDR_OFFSET       0x004
#define GIER_GIE_MASK          0x00000001U  /* bit 0: Global Interrupt Enable */
//IPIER IP Interrupt Enable slv_reg02 0x08
#define IPIER_ADDR_OFFSET      0x008
#define IPIER_FIE_MASK         0x00000001U  /* bit 0: Frame Interrupt Enable */
//IPISR IP Interrupt Status slv_reg03 0x0C
#define IPISR_ADDR_OFFSET      0x00C
#define IPISR_FDP_MASK         0x00000001U  /* bit 0: Frame Done Processed (W1C) */
//IDR ID Register slv_reg04 0x10
#define IDR_ADDR_OFFSET        0x010
//VERR Version Register slv_reg05 0x14
#define VERR_ADDR_OFFSET       0x014

// Begin user code (Nicolas Lonthoff)
//CTRL Control Register slv_reg06 0x18
#define CTRL_ADDR_OFFSET       0x018
#define CTRL_VEN_MASK          0x00000001U  /* bit 0: Visualization Enable */
#define CTRL_WD_MASK           0x00000100U  /* bit 8: Write Data trigger (auto-cleared on vis_core ack) */
//STATUS Status Register slv_reg07 0x1C
#define STATUS_ADDR_OFFSET     0x01C
#define STATUS_FDP_MASK        0x00000001U  /* bit 0: Frame Done Processed (read-only) */
//ADDRR Address Register slv_reg08 0x20
#define ADDRR_ADDR_OFFSET      0x020
#define ADDRR_XA_MASK          0x0000007FU  /* bits 6:0:   X address (0-79) */
#define ADDRR_YA_MASK          0x00001F00U  /* bits 12:8:  Y address (0-29) */
//VDATR Character Data Register slv_reg09 0x24
#define VDATR_ADDR_OFFSET      0x024
#define VDATR_CHAR_MASK        0x0000007FU  /* bits 6:0:   7-bit ASCII character */
//COLR Color Register slv_reg10 0x28
#define COLR_ADDR_OFFSET       0x028
#define COLR_CR_MASK           0x0000000FU  /* bits 3:0:   Red   (4-bit) */
#define COLR_CG_MASK           0x00000F00U  /* bits 11:8:  Green (4-bit) */
#define COLR_CB_MASK           0x000F0000U  /* bits 19:16: Blue  (4-bit) */
#define COLR_COL_MASK          0x000F0F0FU  /* all color channel bits combined */
// End user code (Nicolas Lonthoff)

///Required for _g-file config mechanism (vis_driver_g.c and vis_driver_sinit.c)
#ifndef XPAR_VIS_NUM_INSTANCES
#define XPAR_VIS_NUM_INSTANCES	1
#endif

///!!!NOTE!!!:
//see vis_driver_g.c for further important IP configuration (baseaddress, interrupts, interrupt-parent etc.)


/**************************** Type Definitions *****************************/
typedef struct { 
  char *Name;
  UINTPTR BaseAddress;	/**< Register base address */
	u32 SysClockFreqHz;	/**< The AXI bus clock frequency */
  u32 IntrId;
	UINTPTR IntrParent;	/** Bit[0] Interrupt parent type Bit[64/32:1] Parent base address */
}VIS_Config; //core config data using the _g-file mechanism (do not insert further data here)


typedef void (*VIS_AppHandlerFpType) (void *CallBackRef);

typedef struct {
	VIS_Config Config; //core config using the _g-file mechanism
 	UINTPTR BaseAddress;	 /**< Base address of registers */ //already in VIS_Config, however replicate/buffer it here to avoid costly indirect access in the follwing
	VIS_AppHandlerFpType AppHandler; /**< Callback function */ 
	void *CallBackRef;	 /**< Callback reference for handler  (VIS_Data* instanceptr)*/
}VIS_Data; //config data; insert additional required (config) data in this struct


/************************** Function Prototypes ****************************/
// Begin user code (Nicolas Lonthoff)
/* Initialisation */
XStatus VIS_Init(VIS_Data *InstancePtr, UINTPTR BaseAddress);
void    VIS_InitCfg(VIS_Data *InstancePtr, UINTPTR BaseAddress, VIS_Config *ConfigPtr);
XStatus VIS_InitHw(VIS_Data *InstancePtr);

/* Config lookup (implemented in vis_driver_sinit.c) */
VIS_Config *VIS_LookupConfig(UINTPTR BaseAddress);

/* HAL: write one character cell to the framebuffer */
void VIS_WriteChar(VIS_Data *InstancePtr, u8 x, u8 y, u8 ch, u8 cr, u8 cg, u8 cb);

/* Polling sync: block until STATUS.FDP is set, then clear IPISR.FDP */
void VIS_PollFDP(VIS_Data *InstancePtr);
void VIS_PollWD(VIS_Data *InstancePtr)
// End user code (Nicolas Lonthoff)

/************************** Function Definitions ***************************/

#endif // VIS_DRIVER_H
//end edit Maximilian Hafeneder / Nicolas Lonthoff
