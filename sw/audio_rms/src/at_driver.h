#ifndef AT_DRIVER_H
#define AT_DRIVER_H

#include <stdint.h>
#include "xil_types.h"
#include "xstatus.h"
#include "xil_printf.h"
#include "xparameters.h"
#include "at_driver_l.h"

#define AT_DEBUG_P 1

// TODO: BSP-Platform von XSA aktualisieren, damit xparameters.h XPAR_AUDIO_0_BASEADDR definiert
#define AT_BASEADDRESS XPAR_AUDIO_0_BASEADDR

// Registeroffsets
#define IDR_ADDR_OFFSET    0x010  // ID-Register (read-only)
#define VERR_ADDR_OFFSET   0x014  // Versions-Register (read-only)
#define CTRL_ADDR_OFFSET   0x018
#define STATUS_ADDR_OFFSET 0x01C  // read-only
#define ADATLR_ADDR_OFFSET 0x020  // Audiodaten Links
#define ADATRR_ADDR_OFFSET 0x024  // Audiodaten Rechts

// CTRL-Bits
#define CTRL_SEN_MASK   0x00000001  // Sampling Enable

// STATUS-Bits
#define STATUS_DOL_MASK 0x00000001  // Data Overrun Links
#define STATUS_DOR_MASK 0x00000100  // Data Overrun Rechts
#define STATUS_SLA_MASK 0x00010000  // Sample Links verfügbar
#define STATUS_SRA_MASK 0x01000000  // Sample Rechts verfügbar

// Erwartete Konstanten aus dem IP
#define AUDIO_ID_VALUE      0x0000FEEDu
#define AUDIO_VERSION_VALUE 0x00000001u

#ifndef XPAR_AT_NUM_INSTANCES
#define XPAR_AT_NUM_INSTANCES 1
#endif

typedef struct {
    char    *Name;
    UINTPTR  BaseAddress;
    u32      SysClockFreqHz;
} AT_Config;

typedef void (*AT_AppHandlerFpType)(void *CallBackRef);

typedef struct {
    AT_Config           Config;
    UINTPTR             BaseAddress;
    AT_AppHandlerFpType AppHandler;
    void               *CallBackRef;
} AT_Data;

XStatus  AT_Init(AT_Data *InstancePtr, uint32_t BaseAddress);
void     AT_InitCfg(AT_Data *InstancePtr, uint32_t BaseAddress, AT_Config *ConfigPtr);
XStatus  AT_InitHw(AT_Data *InstancePtr);
AT_Config *AT_LookupConfig(UINTPTR BaseAddress);

void     AT_EnableSampling(AT_Data *InstancePtr);
int      AT_LAvailable(AT_Data *InstancePtr);
int      AT_RAvailable(AT_Data *InstancePtr);
int32_t  AT_GetL(AT_Data *InstancePtr);
int32_t  AT_GetR(AT_Data *InstancePtr);
uint32_t AT_GetOverruns(AT_Data *InstancePtr);
uint32_t AT_GetId(AT_Data *InstancePtr);
uint32_t AT_GetVersion(AT_Data *InstancePtr);

#endif // AT_DRIVER_H
