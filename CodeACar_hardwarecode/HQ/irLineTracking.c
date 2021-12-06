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
 * D0: P4.4          P4.5
 */

void irLineTracking(void)
{
    /* Set P4.4 as input pin for Left IRLine */
    GPIO_setAsInputPin(GPIO_PORT_P4, GPIO_PIN4);

    /* Set P4.5 as input pin for Right IRLine */
    GPIO_setAsInputPin(GPIO_PORT_P4, GPIO_PIN5);
}

// Joshua's Adapted Code
void boundsCheck(void)
{
    /* Read Left IRLine input value */
    uint8_t leftIRInputValue = GPIO_getInputPinValue(GPIO_PORT_P4, GPIO_PIN4);
    /* If detect black, it will trigger stop */
    if (leftIRInputValue != 1)
    {
        emergencyTrig = 1;
        Mutex = 1;
        printf("E Left\n");
        fflush(stdout);
    }

    /* Read Right IRLine input value */
    uint8_t rightIRInputValue = GPIO_getInputPinValue(GPIO_PORT_P4, GPIO_PIN5);
    /* If detect black, it will trigger stop */
    if (rightIRInputValue != 1)
    {
        emergencyTrig = 1;
        Mutex = 1;
        printf("E Right\n");
        fflush(stdout);
    }
}
