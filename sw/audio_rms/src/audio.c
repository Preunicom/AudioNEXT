// BEGIN NEW Felix Knoll
#include "audio.h"

static uint32_t overrun_count = 0;

/* 24-Bit auf 32 Bit erweitern. */
static int32_t sign_extend_24(uint32_t raw)
{
    raw &= 0x00FFFFFFu;
    if (raw & 0x00800000u) {
        raw |= 0xFF000000u;
    }
    return (int32_t)raw;
}

void audio_enable_sampling(void)
{
    AUDIO_REG(AUDIO_CTRL) = AUDIO_CTRL_SEN;
}

int audio_l_available(void)
{
    return (AUDIO_REG(AUDIO_STATUS) & AUDIO_STATUS_SLA) ? 1 : 0;
}

int audio_r_available(void)
{
    return (AUDIO_REG(AUDIO_STATUS) & AUDIO_STATUS_SRA) ? 1 : 0;
}

int32_t audio_get_l(void)
{
    return sign_extend_24(AUDIO_REG(AUDIO_ADATLR));
}

int32_t audio_get_r(void)
{
    return sign_extend_24(AUDIO_REG(AUDIO_ADATRR));
}

/* Returned ob  Overrun-Flags anliegen*/
uint32_t audio_get_overruns(void)
{
    uint32_t status = AUDIO_REG(AUDIO_STATUS);
    uint32_t cnt = 0;
    if (status & AUDIO_STATUS_DOL) cnt++;
    if (status & AUDIO_STATUS_DOR) cnt++;
    return cnt;
}
// END NEW Felix Knoll
