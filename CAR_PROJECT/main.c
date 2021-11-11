/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdio.h>
#include <sys_values.h>
#include <sys_API.h>


/* Things done: 14/10/2021
 * Both wheels can spin
 * Calculate spin/5seconds for left side only, RIGHT SIDE NOT DONE
 * PID controller not yet set (maybe use another timer to manage it? or fit it into the current timers)
 * Need to implement the Ultrasonic as a individual module
 * Need to experiment the UART sensor
 */

int main(void)
{
    /* Halting the watchdog */
    MAP_WDT_A_holdTimer();

//    initialisePWM(1);        //Initialise PWM
//    initialiseUART();       //Initialise UART
    initialiseUSM();        //Initialise USM

    /* Enabling interrupts and starting the watchdog timer*/
    Interrupt_enableSleepOnIsrExit();
    Interrupt_enableMaster();
//    MOV(1, 1, 100); // forward, low speed, 10seconds

    /* Sleeping when not in use */
    while (1)
    {
        PCM_gotoLPM0();
    }
}
