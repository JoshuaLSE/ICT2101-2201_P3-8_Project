#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdio.h>
#include <sys_values.h>]
#include <sys_API.h>

#define TICKPERIOD      1000

uint32_t SR04IntTimes;

// -------------------------------------------------------------------------------------------------------------------
// manual waiting; Try converting this to interrupt based
static void Delay(uint32_t loop)
{
    volatile uint32_t i;

    for (i = 0; i < loop; i++)
        ;
}

// -------------------------------------------------------------------------------------------------------------------
// Configure Timer A0
void Initalise_HCSR04(void)
{
    //int a = CS_getSMCLK();                          //Get the current SMCLK frequency. Not neccessary for us

    /* Configuring P3.6 as Output */
    GPIO_setAsOutputPin(GPIO_PORT_P5, GPIO_PIN4);
    GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN4);

    /* Configuring P3.7 as Input */
    // WE GET THE INPUT FROM HERE!!! This tells us when the module has received the soundwave!
    GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P5, GPIO_PIN5);

    /* Timer_A UpMode Configuration Parameter */
    const Timer_A_UpModeConfig upConfig = {
    TIMER_A_CLOCKSOURCE_SMCLK,              // SMCLK Clock Source
            TIMER_A_CLOCKSOURCE_DIVIDER_3,          // SMCLK/3 = 1MHz
            TICKPERIOD,                  // 1000 tick period, limited to 16 bits
            TIMER_A_TAIE_INTERRUPT_DISABLE,         // Disable Timer interrupt
            TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE,     // Enable CCR0 interrupt
            TIMER_A_DO_CLEAR                        // Clear value
            };
    /*
     * Open declaration for Timer_A_UpMode config. Also use the driverlib documentation.
     * There's 2 things to know here, as it differs from PWM.
     * TAIE -   TimerA Interrupt (triggers when it resets @ max value -> 0) WE NOT USING THIS
     * CCR0 -   CaptureCompareRegister0 triggers when timer has counted to TICKPERIOD
     * Timer_A_DO_CLEAR
     * This clears the timer value when it hits the TICKPERIOD, why? Because we increment
     * the global variable SR04IntTimes. This helps us prevent overflow
     */

    /* Configuring Timer_A0 for Up Mode */
    Timer_A_configureUpMode(TIMER_A0_BASE, &upConfig);
    // TIMER_AX_BASE where X can be 0 to 3

    /* Enabling interrupts and starting the timer */
    Interrupt_enableInterrupt(INT_TA0_0);
    //Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE);

    //Timer_A_stopTimer(TIMER_A0_BASE);
    Timer_A_clearTimer(TIMER_A0_BASE);

}

// -------------------------------------------------------------------------------------------------------------------

void TA0_0_IRQHandler(void)
{
    /* Increment global variable (count number of interrupt occurred) */
    SR04IntTimes++;

    /* Clears the capture-compare interrupt flag */
    Timer_A_clearCaptureCompareInterrupt(TIMER_A0_BASE,
    TIMER_A_CAPTURECOMPARE_REGISTER_0);
}

// -------------------------------------------------------------------------------------------------------------------

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
    Timer_A_clearTimer(TIMER_A0_BASE);
    Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE);

    /* Detects negative-edge */
    while (GPIO_getInputPinValue(GPIO_PORT_P5, GPIO_PIN5) == 1)
        ;

    /* Stop Timer */
    Timer_A_stopTimer(TIMER_A0_BASE);

    /* Obtain Pulse Width in microseconds */
    pulseduration = SR04IntTimes * TICKPERIOD + Timer_A_getCounterValue(TIMER_A0_BASE);

    /* Calculating distance in cm */
    calculateddistance = (float) pulseduration / 58.0f;
    return calculateddistance;
}

// -------------------------------------------------------------------------------------------------------------------

uint32_t main(void)
{
    Initalise_HCSR04();

    /* Configure P1.0 LED and set it to LOW */
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);

    while (1)
    {
        Delay(3000);

        /* Obtain distance from HCSR04 sensor and check if its less then minimum distance */
        float a = getHCSR04Distance(); // just cast it to a float to print, dont need double work
        printf("Distance %f \n", a);
        fflush(stdout);
    }
}
