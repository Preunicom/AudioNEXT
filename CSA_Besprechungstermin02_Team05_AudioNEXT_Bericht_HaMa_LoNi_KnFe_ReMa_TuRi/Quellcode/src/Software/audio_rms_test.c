// BEGIN NEW Felix Knoll
#include <stdint.h>
#include "xil_printf.h"
#include "audio_rms.h"

#define FULL_SCALE  ((int32_t)(1 << 23) - 1)
#define HALF_SCALE  ((int32_t)(1 << 22))

static int pass = 0;
static int fail = 0;

static void check(const char *name, int ok)
{
    if (ok) {
        xil_printf("PASS: %s\n\r", name);
        pass++;
    } else {
        xil_printf("FAIL: %s\n\r", name);
        fail++;
    }
}

/* Hilfsfunktion: füllt Block mit identischen Samples und gibt fp72 zurück. */
static uint16_t run_block(int32_t l, int32_t r)
{
    uint32_t i;
    rms_reset();
    for (i = 0; i < RMS_SAMPLES_PER_CH; i++) {
        rms_add(l, r);
    }
    return rms_value_fp72();
}

int audio_rms_test(void)
{
    uint16_t val;

    xil_printf("--- audio_rms test ---\n\r");

    rms_reset();
    check("fp72 before any add -> 0", rms_value_fp72() == 0);

    val = run_block(0, 0);
    check("silence -> fp72==0", val == 0);

    val = run_block(FULL_SCALE, FULL_SCALE);
    check("full scale -> fp72 ~400", val >= 398 && val <= 400);

    val = run_block(HALF_SCALE, HALF_SCALE);
    check("half scale -> fp72==200", val == 200);

    rms_reset();
    rms_add(0, 0);
    check("not full after 1 add", rms_block_full() == 0);
    run_block(0, 0);
    check("full after 32000 adds", rms_block_full() == 1);

    rms_reset();
    check("reset clears block_full", rms_block_full() == 0);

    val = run_block(-FULL_SCALE, -FULL_SCALE);
    check("negative full scale -> fp72 ~400", val >= 398 && val <= 400);

    val = run_block(FULL_SCALE, 0);
    check("L=full R=0 -> fp72 ~282", val >= 281 && val <= 283);

    xil_printf("--- %d passed, %d failed ---\n\r", pass, fail);
    return fail;
}
// END NEW Felix Knoll
