#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdint.h>
#include <stdbool.h>

/*         IR Line Tracking
 * Left IR Line      Right IR line
 * VCC: 3V3          3V3
 * GND: GND          GND
 * D0: P1.6          P1.7
 *
 */

/* Application Defines  */
#define TIMER_PERIOD    0x2DC6

/* Timer_A UpMode Configuration Parameter */
const Timer_A_UpModeConfig upConfig =
{
        TIMER_A_CLOCKSOURCE_SMCLK,               // SMCLK Clock Source = 3MHz
        TIMER_A_CLOCKSOURCE_DIVIDER_64,          // SMCLK/64
        TIMER_PERIOD,                            // 5000 tick period
        TIMER_A_TAIE_INTERRUPT_DISABLE,          // Disable Timer interrupt
        TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE ,     // Enable CCR0 interrupt
        TIMER_A_DO_CLEAR                         // Clear value
};


void irLineTracking_UsingTimer(void)
{
    /* Get Left IRLine input value */
    GPIO_setAsInputPin(GPIO_PORT_P1, GPIO_PIN6);
    GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN6);

    /* Get Right IRLine input value */
    GPIO_setAsInputPin(GPIO_PORT_P1, GPIO_PIN7);
    GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN7);

    /* P1.0 LED as output */
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);

    /* P2.0 LED as output */
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0);

    /* Configuring Timer_A1 for Up Mode */
    Timer_A_configureUpMode(TIMER_A1_BASE, &upConfig);

    /* Enabling interrupts and starting the timer */
//    Interrupt_enableSleepOnIsrExit();
    Interrupt_enableInterrupt(INT_TA1_0);
    Timer_A_startCounter(TIMER_A1_BASE, TIMER_A_UP_MODE);

    /* Enabling MASTER interrupts */
//    Interrupt_enableMaster();

//    /* Sleeping when not in use */
//    while(1)
//    {
//        PCM_gotoLPM0();
//    }
}

// Timer Interrupt
void TA1_0_IRQHandler(void)
{
    /* Read Left IRLine input value */
    uint8_t leftIRInputValue = GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN6);


    /* If Left IRLine detected value is high means obstacle detected */
    if (leftIRInputValue == 1)
    {
        GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0); //on LED1.0
    }
    else
    {
        GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);  //off LED1.0
    }


    /* Read Right IRLine input value */
    uint8_t rightIRInputValue = GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN7);


    /* If Right IRLine detected value is high means obstacle detected */
    if (rightIRInputValue == 1)
    {
        GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN0); //on LED2.0
    }
    else
    {
        GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0); //off LED2.0
    }

    /* Clear the capture-compare interrupt flag */
    Timer_A_clearCaptureCompareInterrupt(TIMER_A1_BASE,
            TIMER_A_CAPTURECOMPARE_REGISTER_0);
}
