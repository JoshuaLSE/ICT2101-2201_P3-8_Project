//#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
//#include <stdint.h>
//#include <stdbool.h>
//#include <stdio.h>
//
///*       IR Line Tracking
// * Left IR Line      Right IR line
// * VCC: 3V3          3V3
// * GND: GND          GND
// * D0: P1.6          P1.7
// *
// */
//
//void irLineTracking_UsingInterrupt(void)
//{
//    /* IRLine left & right input value */
//    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN6|GPIO_PIN7);
//
//    /* P1.0 LED as output */
//    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
//    GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
//
//    /* P2.0 LED as output */
//    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0);
//    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0);
//
//    /* Clear interrupt flags */
//    GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN6|GPIO_PIN7);
//
//    /* Enable interrupts */
//    GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN6|GPIO_PIN7);
//    Interrupt_enableInterrupt(INT_PORT1);
////    Interrupt_enableMaster();
////
////    /* Sleeping when not in use */
////    while(1)
////    {
////       PCM_gotoLPM0();
////    }
//}
//
//// Interrupt
//void PORT1_IRQHandler(void)
//{
//    /* Read Left IRLine input value and stop the car */
//    int status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P1);
//    int pinSix = GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN6);
//    printf("Status: %d, PinValue: %d\n", status, pinSix);
//    fflush(stdout);
////    /* If Right IRLine detected value equal high means obstacle detected */
////    if (pinSix == 1)
////    {
////        printf("black:%d \n", pinSix);
////        fflush(stdout);
////        GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0); //on LED1.0
////    }
////    if (pinSix == 0)
////    {
////        printf("white:%d \n", pinSix);
////        fflush(stdout);
////        GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0); //on LED1.0
////    }
////    else
////    {
////        printf("there:%d\n", pinSix);
////        fflush(stdout);
////        GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);  //off LED1.0
////    }
//
////    /* If Right IRLine detected value equal high means obstacle detected */
////    if (status & GPIO_PIN7 == GPIO_PIN7)
////    {
////        GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN0); //on LED2.0
////    }
////    else
////    {
////        GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0); //off LED2.0
////    }
//
//    /* Clear interrupt flag */
//    GPIO_clearInterruptFlag(GPIO_PORT_P1, status);
//}
