#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

/* Buzzer
 * Pin: P2.5   (TA0.2) PWM out
 * Pin: GND
 */

#define PERIOD 10000
#define DUTYCYCLE 0

/* Timer_A PWM Configuration Parameter */
Timer_A_PWMConfig pwmConfigBuzzer = {
    TIMER_A_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source = 3MHz
    TIMER_A_CLOCKSOURCE_DIVIDER_1,      // Reference clock become 3MHz/1 = 3,000,000
    PERIOD,                             // period = 10,000 tick in 1 second
    TIMER_A_CAPTURECOMPARE_REGISTER_2,  // CCR2
    TIMER_A_OUTPUTMODE_RESET_SET,       // set/reset output mode
    DUTYCYCLE                           // DutyCycle = 0
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
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN5,GPIO_PRIMARY_MODULE_FUNCTION);

    /* SMCLK clock Frequency 24,000,000 */
    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_24);


    /* Link the TIMER_A0 to PWMConfig (need to use the correct Timer_A see MSP432 Data Sheet) */
    Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfigBuzzer);


    /* Configuring Timer_A1 for Up Mode */
    Timer_A_configureUpMode(TIMER_A1_BASE, &upConfigBuzzer);


    /* Enabling interrupts and starting the timer */
//    Interrupt_enableSleepOnIsrExit();
    Interrupt_enableInterrupt(INT_TA1_0);
    Timer_A_startCounter(TIMER_A1_BASE, TIMER_A_UP_MODE);

    /* Enabling MASTER interrupts */
//    Interrupt_enableMaster();
}


// Timer Interrupt
void TA1_0_IRQHandler(void)
{

    /* If duty cycle is greater than period then stop sound */
    if (pwmConfigBuzzer.dutyCycle > PERIOD)
    {
        Timer_A_stopTimer (TIMER_A0_BASE); // stop PWMConfig
        Timer_A_stopTimer (TIMER_A1_BASE); // stop Timer
    }
    else
        pwmConfigBuzzer.dutyCycle += 1500; // Increase by 1500 each time


    Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfigBuzzer);

    /* Clear the capture-compare interrupt flag */
    Timer_A_clearCaptureCompareInterrupt(TIMER_A1_BASE,
    TIMER_A_CAPTURECOMPARE_REGISTER_0);
}
