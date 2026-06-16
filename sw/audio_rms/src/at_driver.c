
/***************************** Include Files *******************************/
#include "at_driver.h"
#include "at_driver_i.h"
#include "at_driver_l.h"


/************************** Constant Definitions ***************************/


/**************************** Type Definitions *****************************/


/************************** Variable Definitions ***************************/


/************************** Function Prototypes ****************************/
static void AT_StubAppHandler(void *CallBackRef);
static int32_t AT_SignExtend24(uint32_t raw);


/************************** Function Definitions ***************************/
static void AT_StubAppHandler(void *CallBackRef)
{
	Xil_AssertVoid(CallBackRef != NULL);
	//Dummy handler
	//just do nothing
}


XStatus AT_Init(AT_Data *InstancePtr, uint32_t BaseAddress)
{
    XStatus Status = XST_SUCCESS;
    AT_Config *ConfigPtr;

    Xil_AssertNonvoid(InstancePtr != NULL);

    //Get core configuration (file _g mechanism)
    ConfigPtr = AT_LookupConfig(BaseAddress);
    if (!ConfigPtr) {
        Status = XST_DEVICE_NOT_FOUND;
        return Status;
    }

    AT_InitCfg(InstancePtr, BaseAddress, ConfigPtr);
    Status = AT_InitHw(InstancePtr);
    return Status;
}


void AT_InitCfg(AT_Data *InstancePtr, uint32_t BaseAddress, AT_Config *ConfigPtr)
{
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(BaseAddress != 0x0);
    Xil_AssertVoid(ConfigPtr != NULL);

    InstancePtr->Config      = *ConfigPtr;
    InstancePtr->BaseAddress = BaseAddress;
    InstancePtr->AppHandler  = AT_StubAppHandler;
    InstancePtr->CallBackRef = InstancePtr;
}


XStatus AT_InitHw(AT_Data *InstancePtr)
{
    XStatus Status = XST_SUCCESS;

    ///Immediate HW Initialisation
    //Nothing required for Audio IP at init time

    return Status;
}


static int32_t AT_SignExtend24(uint32_t raw)
{
    raw &= 0x00FFFFFFu;
    if (raw & 0x00800000u)
        raw |= 0xFF000000u;
    return (int32_t)raw;
}

void AT_EnableSampling(AT_Data *InstancePtr)
{
    Xil_AssertVoid(InstancePtr != NULL);
    AT_mWriteReg(InstancePtr->BaseAddress, CTRL_ADDR_OFFSET, CTRL_SEN_MASK);
}

int AT_LAvailable(AT_Data *InstancePtr)
{
    Xil_AssertNonvoid(InstancePtr != NULL);
    return (AT_mReadReg(InstancePtr->BaseAddress, STATUS_ADDR_OFFSET) & STATUS_SLA_MASK) ? 1 : 0;
}

int AT_RAvailable(AT_Data *InstancePtr)
{
    Xil_AssertNonvoid(InstancePtr != NULL);
    return (AT_mReadReg(InstancePtr->BaseAddress, STATUS_ADDR_OFFSET) & STATUS_SRA_MASK) ? 1 : 0;
}

int32_t AT_GetL(AT_Data *InstancePtr)
{
    Xil_AssertNonvoid(InstancePtr != NULL);
    return AT_SignExtend24(AT_mReadReg(InstancePtr->BaseAddress, ADATLR_ADDR_OFFSET));
}

int32_t AT_GetR(AT_Data *InstancePtr)
{
    Xil_AssertNonvoid(InstancePtr != NULL);
    return AT_SignExtend24(AT_mReadReg(InstancePtr->BaseAddress, ADATRR_ADDR_OFFSET));
}

uint32_t AT_GetOverruns(AT_Data *InstancePtr)
{
    uint32_t status, cnt = 0;
    Xil_AssertNonvoid(InstancePtr != NULL);
    status = AT_mReadReg(InstancePtr->BaseAddress, STATUS_ADDR_OFFSET);
    if (status & STATUS_DOL_MASK) cnt++;
    if (status & STATUS_DOR_MASK) cnt++;
    return cnt;
}

uint32_t AT_GetId(AT_Data *InstancePtr)
{
    Xil_AssertNonvoid(InstancePtr != NULL);
    return AT_mReadReg(InstancePtr->BaseAddress, IDR_ADDR_OFFSET);
}

uint32_t AT_GetVersion(AT_Data *InstancePtr)
{
    Xil_AssertNonvoid(InstancePtr != NULL);
    return AT_mReadReg(InstancePtr->BaseAddress, VERR_ADDR_OFFSET);
}
