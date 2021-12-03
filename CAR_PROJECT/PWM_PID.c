#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys_values.h>
#include <sys_API.h>
#include <ESP8266.h>
#include <UART_Driver.h>
#include <MSPIO.h>

//  _____      ____  __  __   ___   ___ ___   _   ___ _    ___ ___
// | _ \ \    / /  \/  | \ \ / /_\ | _ \_ _| /_\ | _ ) |  | __/ __|
// |  _/\ \/\/ /| |\/| |  \ V / _ \|   /| | / _ \| _ \ |__| _|\__ \
// |_|   \_/\_/ |_|  |_|   \_/_/ \_\_|_\___/_/ \_\___/____|___|___/
//

#define PERIOD      10000
#define DUTYCYCLE   0000

/*
 * Movement API is MOV[direction, speed, duration]
 * Direction:   1 - forward, 2 - right, 3 - backwards, 4 - left
 * Duration:    5 to 10 seconds
 */

int COUNTR = 0;
int COUNTL = 0;

int emergencyTrig;

int Direction;
int Speed;
int Duration;
int TimerCount;

int SPEED[2] = { 5000, 5500 };
/*
 * Timer_A PWM Configuration Parameters
 */
Timer_A_PWMConfig pwmConfigL = { TIMER_A_CLOCKSOURCE_SMCLK, //REF: SMCLK FREQ is 24MHz, too high 32.768khz
        TIMER_A_CLOCKSOURCE_DIVIDER_8,     //Used: FREQ is 3MHz
        PERIOD,
        TIMER_A_CAPTURECOMPARE_REGISTER_2, //CCR2
        TIMER_A_OUTPUTMODE_RESET_SET,
        DUTYCYCLE };

Timer_A_PWMConfig pwmConfigR = { TIMER_A_CLOCKSOURCE_SMCLK,
TIMER_A_CLOCKSOURCE_DIVIDER_8,
                                 PERIOD,
                                 TIMER_A_CAPTURECOMPARE_REGISTER_1, //CCR1
                                 TIMER_A_OUTPUTMODE_RESET_SET,
                                 DUTYCYCLE };

Timer_A_UpModeConfig timerConfig = {
TIMER_A_CLOCKSOURCE_SMCLK,         // SMCLK Clock Source, 24Mhz due to CS_setDCO
        TIMER_A_CLOCKSOURCE_DIVIDER_24, // Ref 1MHz, triggered every 1/20th of a second
        50000,                          // 50000 tick period, limited to 16 bits
        TIMER_A_TAIE_INTERRUPT_DISABLE,     // Won't overflow
        TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE, // Enable CCR0 interrupt
        TIMER_A_DO_CLEAR                    // Clear value
        };

//  ___ ___ ___
// | _ \_ _|   \
// |  _/| || |) |
// |_| |___|___/
//

#define KP 1.3
#define KD 0.25
#define KI 0.025
float target[2] = { 27, 19 };

int Counter_L;
float Counter_L_Prev;
float Counter_L_Sum;

int Counter_R;
float Counter_R_Prev;
float Counter_R_Sum;

/*
 * Initialise variables and hardware for wheel counters
 */
void initialisePIDcounter(void)
{
    Counter_L = 0;
    Counter_L_Prev = 0;
    Counter_L_Sum = 0;

    Counter_R = 0;
    Counter_R_Prev = 0;
    Counter_R_Sum = 0;

    Interrupt_enableInterrupt(INT_PORT3);
    GPIO_clearInterruptFlag(GPIO_PORT_P3, GPIO_PIN6);
    GPIO_clearInterruptFlag(GPIO_PORT_P3, GPIO_PIN7);
    GPIO_enableInterrupt(GPIO_PORT_P3, GPIO_PIN6);
    GPIO_enableInterrupt(GPIO_PORT_P3, GPIO_PIN7);
}

/*
 * Port 3 ISR - This ISR will increment for both 3.6 & 3.7
 */
void PORT3_IRQHandler(void)
{
    uint32_t status = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P3);

    if (status & GPIO_PIN6)
    {
        Counter_R++;
        COUNTR++;
    }
    if (status & GPIO_PIN7)
    {
        Counter_L++;
        COUNTL++;
    }

    GPIO_clearInterruptFlag(GPIO_PORT_P3, status);
}

/*
 * PID to correct wheel rotation
 */
void pid(void)
{
    // Store counter locally
    float Stored_L = Counter_L;
    float Stored_R = Counter_R;
//    printf("L: %.1f, R: %.2f\n", Stored_L,Stored_R);
//    fflush(stdout);
    // Reset counter for easier calculation
    Counter_L = 0;
    Counter_R = 0;

    //Correction
    float currentError_L = target[Speed] - Stored_L;
    float adjustment_L = currentError_L * KP + Counter_L_Prev * KD
            + Counter_L_Sum * KI;

    pwmConfigL.dutyCycle += (int) adjustment_L;

    Counter_L_Prev = currentError_L;
    Counter_L_Sum += currentError_L;

    Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfigL);

    float currentError_R = target[Speed] - Stored_R;
    float adjustment_R = currentError_R * KP + Counter_R_Prev * KD
            + Counter_R_Sum * KI;

    pwmConfigR.dutyCycle += (int) adjustment_R;

    Counter_R_Prev = currentError_R;
    Counter_R_Sum += currentError_R;

    Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfigR);
//    printf("L: %d, R: %d\n", pwmConfigL.dutyCycle, pwmConfigR.dutyCycle);
//    fflush(stdout);
//    printf("TargetSpeed: %.3f\n"
//           "Right Counter: %.2f  Right current Error: %.3f  Right adj:%.3f  Right DC: %d\n"
//           "Left Counter: %.2f  Left current Error: %.3f  Left adj:%.3f  Left DC: %d\n\n",
//           target[Speed], Stored_R, currentError_R, adjustment_R,
//           pwmConfigR.dutyCycle, Stored_L, currentError_L, adjustment_L,
//           pwmConfigL.dutyCycle);
//    fflush(stdout);
}

//  _____      ____  __
// | _ \ \    / /  \/  |
// |  _/\ \/\/ /| |\/| |
// |_|   \_/\_/ |_|  |_|
//

/*
 * Initialise the PWM variables and hardware
 */
void initialisePWM()
{
    Direction = 1;
    Speed = 0;
    Duration = 0;
    TimerCount = 0;

    initialisePIDcounter();
    /*
     * P4.0, P4.2, P4.4 and P4.5 as Output
     * P2.4 as peripheral output for PWM
     * P1.1 for button interrupt
     */

    //Enable A - RIGHT Wheel
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN0);
    GPIO_setAsOutputPin(GPIO_PORT_P5, GPIO_PIN6);
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN4,
    GPIO_PRIMARY_MODULE_FUNCTION); //ena

    //Enable B - LEFT Wheel
    GPIO_setAsOutputPin(GPIO_PORT_P5, GPIO_PIN7);
    GPIO_setAsOutputPin(GPIO_PORT_P6, GPIO_PIN6);
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN5,
    GPIO_PRIMARY_MODULE_FUNCTION); //enb

    Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfigL);
    Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfigR);

    Timer_A_configureUpMode(TIMER_A1_BASE, &timerConfig);
    Interrupt_enableInterrupt(INT_TA1_0);
    Timer_A_clearTimer(TIMER_A1_BASE);
}

/* API for movement
 * 0) Set pwmCompleted flag
 * 1) Set speed
 * 2) Set timer
 * 3) Set power config
 * 4) Start timer
 */
int MOV(int direction, int speed, int duration)
{
    // Reset the Count
    COUNTR = 0;
    COUNTL = 0;
    // Set up emergency stop variable
    emergencyTrig = 0;
    Direction = direction;
    // Set up timer duration - Triggered every 1/20th of a sec => Increment it by 20 to match per second
    Duration = duration * 20;
    // Reset TimerCount
    TimerCount = 0;
    pwmConfigR.dutyCycle = SPEED[1];
    pwmConfigL.dutyCycle = SPEED[0];

    /*
     * Similar output = stop. (Think XOR)
     * Reverse output = reverse direction
     */
    switch (direction)
    {
    case 1:     // Forwards
        GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN0);   // YELLOW   IN1
        GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN6);    // ORANGE   IN2
        GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN7);    // RED      IN3
        GPIO_setOutputHighOnPin(GPIO_PORT_P6, GPIO_PIN6);   // BROWN    IN4
        break;

    case 2:     // Reverse
        GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN0);    // YELLOW   IN1
        GPIO_setOutputHighOnPin(GPIO_PORT_P5, GPIO_PIN6);   // ORANGE   IN2
        GPIO_setOutputHighOnPin(GPIO_PORT_P5, GPIO_PIN7);   // RED      IN3
        GPIO_setOutputLowOnPin(GPIO_PORT_P6, GPIO_PIN6);    // BROWN    IN4
        break;

    case 3:     // turn left
        GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN0);
        GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN6);
        GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN7);
        GPIO_setOutputLowOnPin(GPIO_PORT_P6, GPIO_PIN6);
        break;

    case 4:     // turn right
        GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN0);
        GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN6);
        GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN7);
        GPIO_setOutputHighOnPin(GPIO_PORT_P6, GPIO_PIN6);
        break;
    }
    Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfigL);
    Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfigR);
    Timer_A_startCounter(TIMER_A1_BASE, TIMER_A_UP_MODE);
    printf("MOV called\n");
    fflush(stdout);
    return 0;
}

/*
 * Reset & Stop
 */
void emergencyStop(void)
{
    pwmConfigR.dutyCycle = 0000;
    pwmConfigL.dutyCycle = 0000;
    Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfigL);
    Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfigR);
    Counter_L_Prev = 0;
    Counter_L_Sum = 0;
    Counter_R_Prev = 0;
    Counter_R_Sum = 0;
    GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN0);
    GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN6);
    GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN7);
    GPIO_setOutputLowOnPin(GPIO_PORT_P6, GPIO_PIN6);
    Timer_A_stopTimer(TIMER_A0_BASE);
    Timer_A_stopTimer(TIMER_A1_BASE);
}

//TimerA1 handler
void TA1_0_IRQHandler(void)
{
    if (emergencyTrig == 0)
    {
        if (TimerCount < Duration)
        {
            TimerCount++;
            pid();
        }
        else
        {
            emergencyStop();
            Mutex = 1; // Release the commandHandler trap
            printf("Action Completed Mutex Released\n");
            fflush(stdout);
            Timer_A_clearCaptureCompareInterrupt(TIMER_A1_BASE,
            TIMER_A_CAPTURECOMPARE_REGISTER_0);
        }
    }
    else
    {
        emergencyStop();
        Mutex = 1; // Release the commandHandler trap
        printf("Emergency Mutex Released\n");
        fflush(stdout);
        Timer_A_clearCaptureCompareInterrupt(TIMER_A1_BASE,
        TIMER_A_CAPTURECOMPARE_REGISTER_0);
    }
    Timer_A_clearCaptureCompareInterrupt(TIMER_A1_BASE,
    TIMER_A_CAPTURECOMPARE_REGISTER_0);
}
