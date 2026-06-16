
#ifndef AT_DRIVER_L_H
#define AT_DRIVER_L_H

/*
* this module contains low-level driver functions like macros
*/

/****************** Include Files ********************/
#include "xil_io.h"

/************************** Constant Definitions ***************************/


/**************************** Type Definitions *****************************/


/************************** Function Prototypes ****************************/


/************************** Function Definitions ***************************/
/**
 *
 * Write a value to a AT register. A 32 bit write is performed.
 * If the component is implemented in a smaller width, only the least
 * significant data is written.
 *
 * @param   BaseAddress is the base address of the AT device.
 * @param   RegOffset is the register offset from the base to write to.
 * @param   Data is the data written to the register.
 *
 * @return  None.
 *
 * @note
 * C-style signature:
 * 	void AT_mWriteReg(u32 BaseAddress, unsigned RegOffset, u32 Data)
 *
 */
#define AT_mWriteReg(BaseAddress, RegOffset, Data) \
  	Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))

/**
 *
 * Read a value from a AT register. A 32 bit read is performed.
 * If the component is implemented in a smaller width, only the least
 * significant data is read from the register. The most significant data
 * will be read as 0.
 *
 * @param   BaseAddress is the base address of the AT device.
 * @param   RegOffset is the register offset from the base to write to.
 *
 * @return  Data is the data from the register.
 *
 * @note
 * C-style signature:
 * 	u32 AT_mReadReg(u32 BaseAddress, unsigned RegOffset)
 *
 */
#define AT_mReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))


#endif // AT_DRIVER_L_H
