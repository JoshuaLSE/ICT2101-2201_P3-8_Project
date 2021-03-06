#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys_values.h>
#include <sys_API.h>

/* Buzzer
 * Pin: P2.7   (TA0.4) PWM out
 * Pin: GND
 */

int buzzer_set = 0;
#define PERIOD 10000
#define DUTYCYCLE 0

/* Timer_A PWM Configuration Parameter */
Timer_A_PWMConfig pwmConfigBuzzer = {
        TIMER_A_CLOCKSOURCE_SMCLK,              // SMCLK Clock Source = 24MHz (Joshua: after set DCO_24)
        TIMER_A_CLOCKSOURCE_DIVIDER_8,          // Reference clock become 24MHz
        PERIOD,                                 // period = 10,000 tick in 1 second
        TIMER_A_CAPTURECOMPARE_REGISTER_4,      // CCR4 (P2.7)
        TIMER_A_OUTPUTMODE_RESET_SET,           // set/reset output mode
        DUTYCYCLE                               // DutyCycle = 0
        };

#define TIMER_PERIOD 40000

/* Timer_A UpMode Configuration Parameter */
const Timer_A_UpModeConfig upConfigBuzzer = {
        TIMER_A_CLOCKSOURCE_SMCLK,               // SMCLK Clock Source = 3MHz
        TIMER_A_CLOCKSOURCE_DIVIDER_64,          // SMCLK/64
        TIMER_PERIOD,                            // period = 40000 tick in 1 second
        TIMER_A_TAIE_INTERRUPT_DISABLE,          // Disable Timer interrupt
        TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE,      // Enable CCR0 interrupt
        TIMER_A_DO_CLEAR                         // Clear value
        };

void buzzer(void)
{
    buzzer_set = 1;                                         //Joshua: To let TA interrupt handle it
    pwmConfigBuzzer.dutyCycle = 0;                          // Joshua: Reset to 0 each time.
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN7,
    GPIO_PRIMARY_MODULE_FUNCTION);                          // Joshua: Set to P2.7 instead, using CCR4
    Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfigBuzzer);
    Timer_A_configureUpMode(TIMER_A3_BASE, &upConfigBuzzer); //Josh: change to A3 due to A0 -> PWM, A1 -> PID, A2 -> USM
    Interrupt_enableInterrupt(INT_TA3_0);                    //Josh: reflected to A3
    Timer_A_startCounter(TIMER_A3_BASE, TIMER_A_UP_MODE);
}

// Timer Interrupt
void TA3_0_IRQHandler(void)
{
    /* If duty cycle is greater than period then stop buzzer */
    if (pwmConfigBuzzer.dutyCycle > PERIOD)
    {
        if (buzzer_set == 1)
        {
            Timer_A_stopTimer(TIMER_A0_BASE); // stop PWMConfig

            buzzer_set = 0;
            Mutex = 1;
        }
    }
    else
    {
        pwmConfigBuzzer.dutyCycle += 1500; // Increase by 1500 each time
        Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfigBuzzer);
    }

    boundsCheck();

    /* Clear the capture-compare interrupt flag */
    Timer_A_clearCaptureCompareInterrupt(TIMER_A3_BASE,
    TIMER_A_CAPTURECOMPARE_REGISTER_0);
}
