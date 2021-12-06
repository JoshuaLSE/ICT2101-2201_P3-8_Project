#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys_values.h>
#include <sys_API.h>

/*         IR Line Tracking
 * Left IR Line      Right IR line
 * VCC: 3V3          3V3
 * GND: GND          GND
 * D0: P1.6          P1.7
 * UPDATED: Josh: P4.4, P4.5
 *
 */

void irLineTracking_UsingTimer(void)
{
    /* Get Left IRLine input value */
    GPIO_setAsInputPin(GPIO_PORT_P4, GPIO_PIN4);

    /* Get Right IRLine input value */
    GPIO_setAsInputPin(GPIO_PORT_P4, GPIO_PIN5);
}

// Joshua's Adapted Code
void boundsCheck(void)
{
    uint8_t leftIRInputValue = GPIO_getInputPinValue(GPIO_PORT_P4, GPIO_PIN4);
    if (leftIRInputValue == 1)
    {
        emergencyTrig = 1;
        Mutex = 1;
        printf("E Left\n");
        fflush(stdout);
    }
    uint8_t rightIRInputValue = GPIO_getInputPinValue(GPIO_PORT_P4, GPIO_PIN5);
    if (rightIRInputValue == 1)
    {
        emergencyTrig = 1;
        Mutex = 1;
        printf("E Right\n");
        fflush(stdout);
    }
}
