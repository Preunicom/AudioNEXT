/***************************** Include Files *******************************/
#include "aud_driver.h"
#include "aud_driver_i.h"
#include "aud_driver_l.h"


/************************** Constant Definitions ***************************/


/**************************** Type Definitions *****************************/


/************************** Variable Definitions ***************************/


/************************** Function Prototypes ****************************/
static void AUD_StubAppHandler(void *CallBackRef);
// BEGIN NEW Felix Knoll
static int32_t AUD_SignExtend24(uint32_t raw);
// END NEW Felix Knoll


/************************** Function Definitions ***************************/
static void AUD_StubAppHandler(void *CallBackRef)
{
	Xil_AssertVoid(CallBackRef != NULL);
	//Dummy handler
	//just do nothing
}


XStatus AUD_Init(AUD_Data *InstancePtr, uint32_t BaseAddress)
{
    XStatus Status = XST_SUCCESS;
    AUD_Config *ConfigPtr;

    Xil_AssertNonvoid(InstancePtr != NULL);

    //Get core configuration (file _g mechanism)
    ConfigPtr = AUD_LookupConfig(BaseAddress);
    if (!ConfigPtr) {
        Status = XST_DEVICE_NOT_FOUND;
        return Status;
    }

    AUD_InitCfg(InstancePtr, BaseAddress, ConfigPtr);
    Status = AUD_InitHw(InstancePtr);
    return Status;
}


void AUD_InitCfg(AUD_Data *InstancePtr, uint32_t BaseAddress, AUD_Config *ConfigPtr)
{
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(BaseAddress != 0x0);
    Xil_AssertVoid(ConfigPtr != NULL);

    InstancePtr->Config      = *ConfigPtr;
    InstancePtr->BaseAddress = BaseAddress;
    InstancePtr->AppHandler  = AUD_StubAppHandler;
    InstancePtr->CallBackRef = InstancePtr;
}


XStatus AUD_InitHw(AUD_Data *InstancePtr)
{
    XStatus Status = XST_SUCCESS;

    ///Immediate HW Initialisation
    //Nothing required for Audio IP at init time

    return Status;
}


// BEGIN NEW Felix Knoll
static int32_t AUD_SignExtend24(uint32_t raw)
{
    raw &= 0x00FFFFFFu;
    if (raw & 0x00800000u)
        raw |= 0xFF000000u;
    return (int32_t)raw;
}

void AUD_EnableSampling(AUD_Data *InstancePtr)
{
    Xil_AssertVoid(InstancePtr != NULL);
    AUD_mWriteReg(InstancePtr->BaseAddress, CTRL_ADDR_OFFSET, CTRL_SEN_MASK);
}

int AUD_LAvailable(AUD_Data *InstancePtr)
{
    Xil_AssertNonvoid(InstancePtr != NULL);
    return (AUD_mReadReg(InstancePtr->BaseAddress, STATUS_ADDR_OFFSET) & STATUS_SLA_MASK) ? 1 : 0;
}

int AUD_RAvailable(AUD_Data *InstancePtr)
{
    Xil_AssertNonvoid(InstancePtr != NULL);
    return (AUD_mReadReg(InstancePtr->BaseAddress, STATUS_ADDR_OFFSET) & STATUS_SRA_MASK) ? 1 : 0;
}

int32_t AUD_GetL(AUD_Data *InstancePtr)
{
    Xil_AssertNonvoid(InstancePtr != NULL);
    return AUD_SignExtend24(AUD_mReadReg(InstancePtr->BaseAddress, ADATLR_ADDR_OFFSET));
}

int32_t AUD_GetR(AUD_Data *InstancePtr)
{
    Xil_AssertNonvoid(InstancePtr != NULL);
    return AUD_SignExtend24(AUD_mReadReg(InstancePtr->BaseAddress, ADATRR_ADDR_OFFSET));
}

uint32_t AUD_GetOverruns(AUD_Data *InstancePtr)
{
    uint32_t status, cnt = 0;
    Xil_AssertNonvoid(InstancePtr != NULL);
    status = AUD_mReadReg(InstancePtr->BaseAddress, STATUS_ADDR_OFFSET);
    if (status & STATUS_DOL_MASK) cnt++;
    if (status & STATUS_DOR_MASK) cnt++;
    return cnt;
}

// END NEW Felix Knoll


// EDIT CODE BEGIN Richard Tuch
uint32_t AUD_GetId(AUD_Data *InstancePtr)
{
    Xil_AssertNonvoid(InstancePtr != NULL);
    return AUD_mReadReg(InstancePtr->BaseAddress, IDR_ADDR_OFFSET);
}

uint32_t AUD_GetVersion(AUD_Data *InstancePtr)
{
    Xil_AssertNonvoid(InstancePtr != NULL);
    return AUD_mReadReg(InstancePtr->BaseAddress, VERR_ADDR_OFFSET);
}
// EDIT CODE END Richard Tuch
