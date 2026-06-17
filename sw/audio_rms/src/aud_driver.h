#ifndef AUD_DRIVER_H
#define AUD_DRIVER_H

#include <stdint.h>
#include "xil_types.h"
#include "xstatus.h"
#include "xil_printf.h"
#include "xparameters.h"
#include "aud_driver_l.h"

#define AUD_DEBUG_P 1

// TODO: BSP-Platform von XSA aktualisieren, damit xparameters.h XPAR_AUDIO_0_BASEADDR definiert
#define AUD_BASEADDRESS XPAR_AUDIO_0_BASEADDR

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

#ifndef XPAR_AUD_NUM_INSTANCES
#define XPAR_AUD_NUM_INSTANCES 1
#endif

typedef struct {
    char    *Name;
    UINTPTR  BaseAddress;
    u32      SysClockFreqHz;
} AUD_Config;

typedef void (*AUD_AppHandlerFpType)(void *CallBackRef);

typedef struct {
    AUD_Config           Config;
    UINTPTR             BaseAddress;
    AUD_AppHandlerFpType AppHandler;
    void               *CallBackRef;
} AUD_Data;

XStatus  AUD_Init(AUD_Data *InstancePtr, uint32_t BaseAddress);
void     AUD_InitCfg(AUD_Data *InstancePtr, uint32_t BaseAddress, AUD_Config *ConfigPtr);
XStatus  AUD_InitHw(AUD_Data *InstancePtr);
AUD_Config *AUD_LookupConfig(UINTPTR BaseAddress);

void     AUD_EnableSampling(AUD_Data *InstancePtr);
int      AUD_LAvailable(AUD_Data *InstancePtr);
int      AUD_RAvailable(AUD_Data *InstancePtr);
int32_t  AUD_GetL(AUD_Data *InstancePtr);
int32_t  AUD_GetR(AUD_Data *InstancePtr);
uint32_t AUD_GetOverruns(AUD_Data *InstancePtr);
uint32_t AUD_GetId(AUD_Data *InstancePtr);
uint32_t AUD_GetVersion(AUD_Data *InstancePtr);

#endif // AUD_DRIVER_H
