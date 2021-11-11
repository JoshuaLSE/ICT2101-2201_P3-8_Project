#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdio.h>
#include <sys_values.h>
#include <sys_API.h>

#define MIN_DISTANCE    15.0f
#define TICKPERIOD      1000

bool usmCompleted;

/*
 * Use timer32 for this. Timer A a bit overused atm
 * Periodic mode
 * 16 bit resolution
 */

uint32_t SR04IntTimes;
int delay;

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
void initialiseUSM(void)
{
    usmCompleted = false;
    /* Configuring P3.6 as Output */
    GPIO_setAsOutputPin(GPIO_PORT_P5, GPIO_PIN4);
    GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN4);

    /* Configuring P3.7 as Input */
    // WE GET THE INPUT FROM HERE!!! This tells us when the module has received the soundwave!
    GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P5, GPIO_PIN5);

    Timer32_initModule(TIMER32_0_BASE, TIMER32_PRESCALER_16, TIMER32_32BIT,
    TIMER32_PERIODIC_MODE);
    Timer32_setCount(TIMER32_0_BASE, 187500);   // triggers every 1s
    // Timer is set at 187.5KHz
    // Resets every 1 second + Calls Interrupt
    Timer32_registerInterrupt(TIMER32_0_INTERRUPT); //Calls ISR 1 when Timer_0 is done
    Timer32_clearInterruptFlag(TIMER32_0_BASE);
    Timer32_enableInterrupt(TIMER32_0_BASE);

    Timer32_initModule(TIMER32_1_BASE, TIMER32_PRESCALER_16, TIMER32_32BIT,
    TIMER32_PERIODIC_MODE);
    Timer32_setCount(TIMER32_1_BASE, 1875);     // triggers every 0.01s
    Timer32_registerInterrupt(TIMER32_1_INTERRUPT);
    Timer32_clearInterruptFlag(TIMER32_1_BASE);
    Timer32_enableInterrupt(TIMER32_1_BASE);
}

// -------------------------------------------------------------------------------------------------------------------

void T32_INT1_IRQHandler(void)
{
    if (delay < DELAY)
    {
        delay++;
    }
    else
        readyFlag = true;
    Timer32_clearInterruptFlag(TIMER32_0_BASE);
}

void T32_INT2_IRQHandler(void)
{
    SR04IntTimes++;
    Timer32_clearInterruptFlag(TIMER32_1_BASE);
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
//    Timer_A_clearTimer(TIMER_A0_BASE);
    Timer32_setCount(TIMER32_0_BASE, 0);
    Timer32_startTimer(TIMER32_0_BASE, false);
//    Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE);

    /* Detects negative-edge */
    while (GPIO_getInputPinValue(GPIO_PORT_P5, GPIO_PIN5) == 1)
        ;

    /* Stop Timer */
    Timer_A_stopTimer(TIMER_A0_BASE);

    /* Obtain Pulse Width in microseconds */
    pulseduration = SR04IntTimes * TICKPERIOD
            + Timer_A_getCounterValue(TIMER_A0_BASE);

    Timer_A_clearTimer(TIMER_A0_BASE);

    /* Calculating distance in cm */
    calculateddistance = (float) pulseduration / 58.0f;
    return calculateddistance;
}

// -------------------------------------------------------------------------------------------------------------------

//uint32_t main(void)
//{
//    Initalise_HCSR04();
//
//    /* Configure P1.0 LED and set it to LOW */
//    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
//    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
//
//    while (1)
//    {
//        Delay(3000);
//
//        /* Obtain distance from HCSR04 sensor and check if its less then minimum distance */
//        float a = getHCSR04Distance(); // just cast it to a float to print, dont need double work
//        if ((a < MIN_DISTANCE))
//        {
//            GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
//            printf("Distance %f \n", a);                    // need to flush the printf statement.
//            fflush(stdout);                                 // as learnt in debugging, this has TREMENDOUS OVERHEAD
//        }                                                   // aka VERY intrusive
//        else
//        {
//            GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
//            printf("Distance %f \n", a);
//            fflush(stdout);
//        }
//    }
//}
