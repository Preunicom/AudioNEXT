// BEGIN NEW Felix Knoll
#ifndef AUDIO_H
#define AUDIO_H

#include <stdint.h>

/* TODO: Basisadresse setzen. */
#define AUDIO_BASEADDR 0x00000000u

#define AUDIO_REG(off) (*(volatile uint32_t *)(AUDIO_BASEADDR + (off)))

/* Registeroffsets */
#define AUDIO_GIER   0x04u
#define AUDIO_IPIER  0x08u
#define AUDIO_IPISR  0x0Cu
#define AUDIO_CTRL   0x18u
#define AUDIO_STATUS 0x1Cu
#define AUDIO_ADATLR 0x20u
#define AUDIO_ADATRR 0x24u
// END NEW Felix Knoll


// EDIT CODE BEGIN Richard Tuch
// weitere Register
#define AUDIO_IDR    0x10u
#define AUDIO_VERR   0x14u

#define AUDIO_ID_VALUE      0x0000FEEDu
#define AUDIO_VERSION_VALUE 0x00000001u

#define AUDIO_GIER_GIE      (1u << 0)

#define AUDIO_IPIER_SLIE    (1u << 0)
#define AUDIO_IPIER_SRIE    (1u << 8)
// EDIT CODE END Richard Tuch


// BEGIN NEW Felix Knoll
#define AUDIO_CTRL_SEN   (1u << 0)   /* Sampling Enable           */
#define AUDIO_STATUS_SLA (1u << 16)  /* Sample Left Available     */
#define AUDIO_STATUS_SRA (1u << 24)  /* Sample Right Available    */
#define AUDIO_STATUS_DOL (1u << 0)   /* Data Overrun Left         */
#define AUDIO_STATUS_DOR (1u << 8)   /* Data Overrun Right        */

void     audio_enable_sampling(void);
int      audio_l_available(void);
int      audio_r_available(void);
int32_t  audio_get_l(void);
int32_t  audio_get_r(void);
uint32_t audio_get_overruns(void);
// END NEW Felix Knoll


// EDIT CODE BEGIN Richard Tuch
// Register Interface
uint32_t audio_get_id(void);
uint32_t audio_get_version(void);

void audio_enable_global_interrupts(void);
void audio_enable_left_interrupt(void);
void audio_enable_right_interrupt(void);
// EDIT CODE END Richard Tuch


// BEGIN NEW Felix Knoll

#endif /* AUDIO_H */
// END NEW Felix Knoll
