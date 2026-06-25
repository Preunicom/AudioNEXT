// BEGIN NEW Felix Knoll
#include "audio_rms.h"

/* maximaler Ausschlag. */
#define RMS_FULLSCALE 8388608u  /* 2^23 */

static uint64_t sq_acc = 0;  /* Summe der Quadrate beider Kanäle */
static uint32_t pair_cnt = 0; /* Anzahl bereits verarbeiteter Links und Rechts Paare */

/* Ganzzahlige Wurzel */
static uint64_t isqrt(uint64_t value)
{
    uint64_t x;
    uint64_t y;

    if (value == 0) {
        return 0;
    }
    x = value;
    y = (x + 1) / 2;
    while (y < x) {
        x = y;
        y = (x + value / x) / 2;
    }
    return x;
}

void rms_reset(void)
{
    sq_acc = 0;
    pair_cnt = 0;
}

/* Energiesumme beider Kanaele */
void rms_add(int32_t l, int32_t r)
{
    sq_acc += (uint64_t)((int64_t)l * (int64_t)l);
    sq_acc += (uint64_t)((int64_t)r * (int64_t)r);
    pair_cnt++;
}

int rms_block_full(void)
{
    return (pair_cnt >= RMS_SAMPLES_PER_CH) ? 1 : 0;
}

/* Lautstaerke in Prozent  für 7.2-Fixed-Point Output */
uint16_t rms_value_fp72(void)
{
    uint64_t mean_sq;
    uint64_t rms;
    uint64_t fp72;

    if (pair_cnt == 0) {
        return 0;
    }
    mean_sq = sq_acc / ((uint64_t)pair_cnt * 2u);
    rms = isqrt(mean_sq);

    fp72 = (rms * 3200u) / RMS_FULLSCALE;
    if (fp72 > 0x1FFu) {
        fp72 = 0x1FFu;
    }
    return (uint16_t)fp72;
}
// END NEW Felix Knoll
