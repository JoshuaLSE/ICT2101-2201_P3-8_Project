#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdint.h>
#include <stdbool.h>

/* 7 LED color + blink (RED, GREEN, BLUE, YELLOW, PURPLE, CYAN, WHITE)
 *
 *      To make (YELLOW, PURPLE, CYAN, WHITE) LED blink,
 *      need to use clearbits() function first to clear the bits.
 */

void onRedLED(void)
{
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0);
    GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN0);
}

void onGreenLED(void)
{
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN1);
    GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN1);
}

void onBlueLED(void)
{
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN2);
    GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN2);
}

void onYellowLED(void)
{
    /* P2.0(red) and P2.1(green) combine as (Yellow LED) as output */
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN1);
    GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN0);
    GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN1);
}

void onPurpleLED(void)
{
    /* P2.0(red) and P2.2(blue) combine as (Purple LED) as output */
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN2);
    GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN0);
    GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN2);
}

void onCyanLED(void)
{
    /* P2.1(green) and P2.2(blue) combine as (Cyan LED) as output */
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN1);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN2);
    GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN1);
    GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN2);
}

void onWhiteLED(void)
{
    /* P2.0(red), P2.1(green), P2.2(blue) combine as (White LED) as output */
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN1);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN2);
    GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN0);
    GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN1);
    GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN2);
}


/* Different LED color keep changing by itself */
void onChangingLEDColor(void)
{
    int i;
    unsigned char j = 0;


    while (1)
    {
        for (i=0; i<100000; i++); // delay
        P2 -> OUT = j++;
        /* if j is greater than 7 binary 111 then reset as 000 restart color again */
        if (j > 7)
        {
            j = 0;
            P2 -> OUT = j;
        }
    }
}


void onRedLED_Blink(void)
{
    volatile uint32_t i;
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0);
    GPIO_toggleOutputOnPin(GPIO_PORT_P2,GPIO_PIN0);
    for(i=100000; i>0; i--); // Delay
}

void onGreenLED_Blink(void)
{
    volatile uint32_t i;
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN1);
    GPIO_toggleOutputOnPin(GPIO_PORT_P2,GPIO_PIN1);
    for(i=100000; i>0; i--); // Delay
}

void onBlueLED_Blink(void)
{
    volatile uint32_t i;
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN2);
    GPIO_toggleOutputOnPin(GPIO_PORT_P2,GPIO_PIN2);
    for(i=100000; i>0; i--); // Delay
}

/* Clearing the bits */
void clearBits(void)
{
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN1);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN2);
    P2OUT = 0;
}

void onYellowLED_Blink(void)
{
    volatile uint32_t i;
    /* P2.0(red) and P2.1(green) combine as (Yellow LED) as output */
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN1);
    P2OUT ^= 3;
    for(i=100000; i>0; i--); // Delay
}

void onPurpleLED_Blink(void)
{
    volatile uint32_t i;
    /* P2.0(red) and P2.2(blue) combine as (Purple LED) as output */
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN2);
    P2OUT ^= 5;
    for(i=100000; i>0; i--); // Delay
}

void onCyanLED_Blink(void)
{
    volatile uint32_t i;
    /* P2.1(green) and P2.2(blue) combine as (Cyan LED) as output */
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN1);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN2);
    P2OUT ^= 6;
    for(i=100000; i>0; i--); // Delay
}

void onWhiteLED_Blink(void)
{
    volatile uint32_t i;
    /* P2.0(red), P2.1(green), P2.2(blue)combine as (White LED) as output */
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN1);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN2);
    P2OUT ^= 7;
    for(i=100000; i>0; i--); // Delay
}
