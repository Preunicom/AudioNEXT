// EDIT CODE BEGIN Richard Tuch
#include "audio_test.h"
#include "audio.h"

static uint32_t failures;

static void check(int condition)
{
    if (!condition)
    {
        failures++;
    }
}

uint32_t audio_test_run(void)
{
    failures = 0;

    /* ID Register Test */
    check(audio_get_id() == AUDIO_ID_VALUE);

    /* Version Register Test */
    check(audio_get_version() == AUDIO_VERSION_VALUE);

    /* Global Interrupt Enable */
    AUDIO_REG(AUDIO_GIER) = 0;

    audio_enable_global_interrupts();

    check(
        (AUDIO_REG(AUDIO_GIER) & AUDIO_GIER_GIE) != 0
    );

    /* Left Interrupt Enable */
    AUDIO_REG(AUDIO_IPIER) = 0;

    audio_enable_left_interrupt();

    check(
        (AUDIO_REG(AUDIO_IPIER) & AUDIO_IPIER_SLIE) != 0
    );

    /* Right Interrupt Enable */
    audio_enable_right_interrupt();

    check(
        (AUDIO_REG(AUDIO_IPIER) & AUDIO_IPIER_SRIE) != 0
    );

    /* Sampling Enable */
    AUDIO_REG(AUDIO_CTRL) = 0;

    audio_enable_sampling();

    check(
        (AUDIO_REG(AUDIO_CTRL) & AUDIO_CTRL_SEN) != 0
    );

    return failures;
}
// EDIT CODE END Richard Tuch
