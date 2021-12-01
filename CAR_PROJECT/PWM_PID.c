#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys_values.h>
#include <sys_API.h>

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
 * Speed:       0 - slow, 1 - fast
 * Duration:    5 to 10 seconds
 */

int COUNTR;
int COUNTL;

int Direction;
int Speed;
int Duration;
int facingDirection;
int TimerCount;
bool pwmCompleted; //declared as global variable in sys_values.h
int LOWSPEED[2] = {3200, 3300};
int HIGHSPEED[2] = {4650, 4800};

/*
 * Timer_A PWM Configuration Parameter
 * Starting at 0% duty cycle
 * Lower speed will be at 25%
 * Higher speed will be at 50%
 */
Timer_A_PWMConfig pwmConfigL = { TIMER_A_CLOCKSOURCE_SMCLK, //REF: SMCLK FREQ is 3MHz
        TIMER_A_CLOCKSOURCE_DIVIDER_10,   //Used: FREQ is 125kHz
        PERIOD,
        TIMER_A_CAPTURECOMPARE_REGISTER_2, //CCR2
        TIMER_A_OUTPUTMODE_RESET_SET,
        DUTYCYCLE };

Timer_A_PWMConfig pwmConfigR = { TIMER_A_CLOCKSOURCE_SMCLK,
TIMER_A_CLOCKSOURCE_DIVIDER_10,
                                 PERIOD,
                                 TIMER_A_CAPTURECOMPARE_REGISTER_1, //CCR1
                                 TIMER_A_OUTPUTMODE_RESET_SET,
                                 DUTYCYCLE };

Timer_A_UpModeConfig timerConfig = {
TIMER_A_CLOCKSOURCE_SMCLK,             // SMCLK Clock Source
        TIMER_A_CLOCKSOURCE_DIVIDER_64, // SMCLK = 3MHz, therefore /64 = 46.875KHz
        46874,             // 50000 tick period, limited to 16 bits
        TIMER_A_TAIE_INTERRUPT_DISABLE,             // Disable Timer interrupt
        TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE,             // Enable CCR0 interrupt
        TIMER_A_DO_CLEAR             // Clear value
        };

//  ___ ___ ___
// | _ \_ _|   \
// |  _/| || |) |
// |_| |___|___/
//

#define KP 1.3
#define KD 0.25
#define KI 0.025
float target[2] = { 12, 19 };

int Counter_L;
float Counter_L_Prev;
float Counter_L_Sum;

int Counter_R;
float Counter_R_Prev;
float Counter_R_Sum;

void initialisePIDcounter(void)
{
    /*Initiate the Counter*/
    Counter_L = 0;
    Counter_L_Prev = 0;
    Counter_L_Sum = 0;

    Counter_R = 0;
    Counter_R_Prev = 0;
    Counter_R_Sum = 0;

    /*Enable Counter Interrupt*/
    Interrupt_enableInterrupt(INT_PORT3);
    GPIO_clearInterruptFlag(GPIO_PORT_P3, GPIO_PIN6);
    GPIO_clearInterruptFlag(GPIO_PORT_P3, GPIO_PIN7);
    GPIO_enableInterrupt(GPIO_PORT_P3, GPIO_PIN6);
    GPIO_enableInterrupt(GPIO_PORT_P3, GPIO_PIN7);
}

/* Port 3 ISR - This ISR will increment for both 3.6 & 3.7*/
void PORT3_IRQHandler(void)
{
    uint32_t status = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P3);

    if (status & GPIO_PIN6)
        Counter_R++;
    if (status & GPIO_PIN7)
        Counter_L++;

    // Clear interrupt!!!
    GPIO_clearInterruptFlag(GPIO_PORT_P3, status);
}

//PID Method
void pid(void)
{
    // Store counter locally
    float Stored_L = Counter_L;
    float Stored_R = Counter_R;
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

void initialisePWM(int initialDirection)
{
    Direction = 1;
    Speed = 0;
    Duration = 0;
    facingDirection = 0;
    TimerCount = 0;
    pwmCompleted = false;
    facingDirection = initialDirection;

    initialisePIDcounter();
    /*
     * P4.0, P4.2, P4.4 and P4.5 as Output
     * P2.4 as peripheral output for PWM
     * P1.1 for button interrupt
     */

    //Enable A - RIGHT Wheel
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN0);
    GPIO_setAsOutputPin(GPIO_PORT_P5, GPIO_PIN6);
    GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN0);    // YELLOW   IN1
    GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN6);   // ORANGE   IN2
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN4,
    GPIO_PRIMARY_MODULE_FUNCTION); //ena

    //Enable B - LEFT Wheel
    GPIO_setAsOutputPin(GPIO_PORT_P5, GPIO_PIN7);
    GPIO_setAsOutputPin(GPIO_PORT_P6, GPIO_PIN6);
    GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN7);    // RED      IN3
    GPIO_setOutputHighOnPin(GPIO_PORT_P6, GPIO_PIN6);   // BROWN    IN4
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN5,
    GPIO_PRIMARY_MODULE_FUNCTION); //enb

    /* Configuring Timer_A0 to have a period of approximately 80ms and an initial duty cycle of 10% of that (1000 ticks)  */
    Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfigL);
    Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfigR);

    /*
     * Configuring Timer_A1 for Up Mode
     * Enabling interrupts
     */
    Timer_A_configureUpMode(TIMER_A1_BASE, &timerConfig);
    Interrupt_enableInterrupt(INT_TA1_0);
    Timer_A_clearTimer(TIMER_A1_BASE);
}

// Function to stop the vehicle - To be called by UART
void emergencyStop(void)
{
    pwmConfigL.dutyCycle = 0;
    pwmConfigR.dutyCycle = 0;
    Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfigL);
    Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfigR);
    Timer_A_stopTimer(TIMER_A1_BASE);
}

// API for movement
int MOV(int direction, int speed, int duration)
{
    /*
     * 0) Set pwmCompleted flag
     * 1) Set speed
     * 2) Set timer
     * 3) Set power config
     * 4) Start timer
     */
    pwmCompleted = false;
    int internal_speed_L = LOWSPEED[0];
    int internal_speed_R = LOWSPEED[1];
    // Set up timer duration
    Duration = duration;
    // Reset TimerCount
    TimerCount = 0;

    switch (speed)
    {
    case 0:
        internal_speed_L = LOWSPEED[0];
        internal_speed_R = LOWSPEED[1];
        Speed = 0;
        break;
    case 1:
        internal_speed_L = HIGHSPEED[0];
        internal_speed_R = HIGHSPEED[1];
        Speed = 1;
        break;
    default:
        return 2; //error - speed is invalid
    }

    switch (direction)
    {
    case 1:     //forwards
        if (Direction != direction)
        {
            GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN0);    // YELLOW   IN1
            GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN6);   // ORANGE   IN2
            GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN7);    // RED      IN3
            GPIO_setOutputHighOnPin(GPIO_PORT_P6, GPIO_PIN6);   // BROWN    IN4
        }
        pwmConfigR.dutyCycle = internal_speed_R;
        pwmConfigL.dutyCycle = internal_speed_L;
        break;

    case 2:     //reverse
        if (Direction != direction)
        {
            GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN0);    // YELLOW   IN1
            GPIO_setOutputHighOnPin(GPIO_PORT_P5, GPIO_PIN6);   // ORANGE   IN2
            GPIO_setOutputHighOnPin(GPIO_PORT_P5, GPIO_PIN7);    // RED      IN3
            GPIO_setOutputLowOnPin(GPIO_PORT_P6, GPIO_PIN6);   // BROWN    IN4
        }
        pwmConfigR.dutyCycle = internal_speed_R;
        pwmConfigL.dutyCycle = internal_speed_L;
        break;

    case 3:     // turn left
        pwmConfigR.dutyCycle = internal_speed_R;
        pwmConfigL.dutyCycle = 0000;
        if (facingDirection-- >= 1)
            facingDirection--;
        else
            facingDirection = 4;
        break;

    case 4:     //turn right
        pwmConfigR.dutyCycle = 0000;
        pwmConfigL.dutyCycle = internal_speed_L;
        if (facingDirection++ <= 4)
            facingDirection++;
        else
            facingDirection = 1;
        break;

    default:
        return 1; //error - direction is invalid
    }
    Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfigL);
    Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfigR);
    Timer_A_startCounter(TIMER_A1_BASE, TIMER_A_UP_MODE);
    return 0;
}

//TimerA1 handler
void TA1_0_IRQHandler(void)
{
    // CHECK DURATION
    if (TimerCount < Duration)
    {
        TimerCount++;
    }
    else
    {
        pwmConfigR.dutyCycle = 0000;
        pwmConfigL.dutyCycle = 0000;
        Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfigL);
        Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfigR);
        Timer_A_stopTimer(TIMER_A1_BASE);
        Timer_A_clearCaptureCompareInterrupt(TIMER_A1_BASE,
        TIMER_A_CAPTURECOMPARE_REGISTER_0);
        pwmCompleted = true;
    }

    // CHECK PID
    pid();

    Timer_A_clearCaptureCompareInterrupt(TIMER_A1_BASE,
    TIMER_A_CAPTURECOMPARE_REGISTER_0);
}
