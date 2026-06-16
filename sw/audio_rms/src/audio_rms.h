// BEGIN NEW Felix Knoll
#ifndef AUDIO_RMS_H
#define AUDIO_RMS_H

#include <stdint.h>

/* 96 kHz pro Kanal, mit 3 Hz Darstellung 96000Hz/3Hz = 32000*/
#define RMS_SAMPLES_PER_CH 32000u

void     rms_reset(void);
void     rms_add(int32_t l, int32_t r);
int      rms_block_full(void);
uint16_t rms_value_fp72(void);

#endif /* AUDIO_RMS_H */
// END NEW Felix Knoll
