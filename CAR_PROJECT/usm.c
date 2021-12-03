#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdio.h>
#include <sys_values.h>
#include <sys_API.h>

#define TICKPERIOD      1000

uint32_t SR04IntTimes;

// Configure Timer A2
void Initialise_USM(void)
{
    /* Timer_A UpMode Configuration Parameter */
    const Timer_A_UpModeConfig upConfig = {
    TIMER_A_CLOCKSOURCE_SMCLK,              // SMCLK Clock Source
            TIMER_A_CLOCKSOURCE_DIVIDER_8,          // SMCLK/3 = 1MHz
            TICKPERIOD,                  // 1000 tick period, limited to 16 bits
            TIMER_A_TAIE_INTERRUPT_DISABLE,         // Disable Timer interrupt
            TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE,     // Enable CCR0 interrupt
            TIMER_A_DO_CLEAR                        // Clear value
            };

    GPIO_setAsOutputPin(GPIO_PORT_P5, GPIO_PIN4);
    GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN4);
    GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P5, GPIO_PIN5);

    Timer_A_configureUpMode(TIMER_A2_BASE, &upConfig);
    Interrupt_enableInterrupt(INT_TA2_0);
    Timer_A_clearTimer(TIMER_A2_BASE);

}

float getHCSR04Distance(void)
{
    uint32_t pulseduration = 0;
    float calculateddistance = 0;

    /* Generate 10us pulse at P3.6 This initiates the HCSR04*/
    GPIO_setOutputHighOnPin(GPIO_PORT_P5, GPIO_PIN4);
    Delay(30);
    GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN4);

    /* Wait for positive-edge This gets a return from HCSR04*/
    while (GPIO_getInputPinValue(GPIO_PORT_P5, GPIO_PIN5) == 0)
        ;

    /* Start Timer */
    SR04IntTimes = 0;
    Timer_A_clearTimer(TIMER_A2_BASE);
    Timer_A_startCounter(TIMER_A2_BASE, TIMER_A_UP_MODE);

    /* Detects negative-edge */
    while (GPIO_getInputPinValue(GPIO_PORT_P5, GPIO_PIN5) == 1)
        ;

    /* Stop Timer */
    Timer_A_stopTimer(TIMER_A2_BASE);

    /* Obtain Pulse Width in microseconds */
    pulseduration = SR04IntTimes * TICKPERIOD
            + Timer_A_getCounterValue(TIMER_A2_BASE);

    /* Calculating distance in cm */
    calculateddistance = (float) pulseduration / 174.0f;
    return (int)calculateddistance;
}

void TA2_0_IRQHandler(void)
{
    /* Increment global variable (count number of interrupt occurred) */
    SR04IntTimes++;

    /* Clear interrupt flag */
    Timer_A_clearCaptureCompareInterrupt(TIMER_A2_BASE,
                                         TIMER_A_CAPTURECOMPARE_REGISTER_0);
}

