#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ESP8266.h>
#include <UART_Driver.h>
#include <MSPIO.h>

#include <sys_values.h>
#include <sys_API.h>

int Mutex = 1;
int Wifi = 1;
/*Initialise arrays with 0*/
int counterR[30] = { 0 };
int counterL[30] = { 0 };
int ET[30] = { 0 };
int currentStage = 0;

char HTTP_WebPage[] = "192.168.1.5";
//    char HTTP_WebPage[] = "172.20.10.6";
char Port[] = "80";
char HTTP_Request[] = "GET / \r\n\r\n";
//char HTTP_Request[] = "GET /ackR105L106E0.R0L45E0.R110L111E0.R0L45E0.R100L106E0.R0L43E0.R104L108E0.R114L112E0. \r\n\r\n";
//    char AP_Name[] = "iPhone";
//    char AP_Pwd[] = "baesbutt";
char AP_Name[] = "Magnifique";
char AP_Pwd[] = "berrypie41";
char ack[200] = "GET /ack";

uint32_t HTTP_Request_Size = sizeof(HTTP_Request) - 1;

eUSCI_UART_ConfigV1 UART0Config = {
        EUSCI_A_UART_CLOCKSOURCE_SMCLK, 13, 0, 37, EUSCI_A_UART_NO_PARITY,
        EUSCI_A_UART_LSB_FIRST,
        EUSCI_A_UART_ONE_STOP_BIT, EUSCI_A_UART_MODE,
        EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION };

eUSCI_UART_ConfigV1 UART2Config = {
        EUSCI_A_UART_CLOCKSOURCE_SMCLK, 13, 0, 37, EUSCI_A_UART_NO_PARITY,
        EUSCI_A_UART_LSB_FIRST,
        EUSCI_A_UART_ONE_STOP_BIT, EUSCI_A_UART_MODE,
        EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION };

void Delay(uint32_t loop)
{
    volatile uint32_t i;

    for (i = 0; i < loop; i++)
        ;
}

/*
 * This parses the input into tokens and checks for correct commands
 * Following which, it will execute them sequentially
 *
 */
void commandHandler(char *a)
{
    currentStage = 0;
    char str[9999] = "";
    const char s[2] = " ";
    char *token;
    char movement[4] = "MOV";
    char buzz[4] = "BUZ";

    char inputStr[99];

    char *movCmd;
    char *buzCmd;

    strcpy(str, a);

    printf("%s\n", str);
    fflush(stdout);

    /* Command Execution */
    token = strtok(str, s);

    /* walk through other tokens */
    while (token != NULL)
    {
        strcpy(inputStr, token);
        movCmd = strstr(token, movement);
        buzCmd = strstr(token, buzz);

        if (movCmd != NULL)
        {
            Mutex = 0;
            buzzer();               // TRYING BUZZER HERE
            while (Mutex == 0)
            {

            }
            Mutex = 0;
            int a = atoi(&movCmd[4]), b = atoi(&movCmd[6]), c = atoi(
                    &movCmd[8]);
            printf("Entering MOV a: %d b: %d c: %d\n", a, b, c);
            fflush(stdout);
            MOV(a, b, c);
            while (Mutex == 0)
            {
                int a = getHCSR04Distance();
                printf("Dist: %dcm\n", a);
                fflush(stdout);
                if (a < 5.00)
                {
                    emergencyTrig = 1;
                    Mutex = 1;
                }
            }
            printf("Im free\n");
            fflush(stdout);
            /*
             * Record movement details here!
             */
            counterR[currentStage] = COUNTR;
            counterL[currentStage] = COUNTL;
            ET[currentStage] = emergencyTrig;
            currentStage++;
        }
        else if (buzCmd != NULL)
        {
            printf("BUZ: %s\n", buzCmd);
            fflush(stdout);
        }

        token = strtok(NULL, s);
    }
}

/*
 * Sends a hard reset command to the ESP8266
 */
void hardReset()
{
    ESP8266_HardReset();
    __delay_cycles(48000000);
    UART_Flush(EUSCI_A2_BASE);
}

/*
 * Hardware initialisation for the ESP8266
 */
void initWifi()
{
    /*Ensure MSP432 is Running at 24 MHz*/
    FlashCtl_setWaitState(FLASH_BANK0, 2);
    FlashCtl_setWaitState(FLASH_BANK1, 2);
    PCM_setCoreVoltageLevel(PCM_VCORE1);
    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_24);

    /*Initialize required hardware peripherals for the ESP8266*/
    GPIO_setAsPeripheralModuleFunctionInputPin(
    GPIO_PORT_P1,
                                               GPIO_PIN2 | GPIO_PIN3,
                                               GPIO_PRIMARY_MODULE_FUNCTION);
    UART_initModule(EUSCI_A0_BASE, &UART0Config);
    UART_enableModule(EUSCI_A0_BASE);
    UART_enableInterrupt(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
    Interrupt_enableInterrupt(INT_EUSCIA0);

    GPIO_setAsPeripheralModuleFunctionInputPin(
    GPIO_PORT_P3,
                                               GPIO_PIN2 | GPIO_PIN3,
                                               GPIO_PRIMARY_MODULE_FUNCTION);
    UART_initModule(EUSCI_A2_BASE, &UART2Config);
    UART_enableModule(EUSCI_A2_BASE);
    UART_enableInterrupt(EUSCI_A2_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
    Interrupt_enableInterrupt(INT_EUSCIA2);

    /*Reset GPIO of the ESP8266*/
    GPIO_setAsOutputPin(GPIO_PORT_P6, GPIO_PIN1);

    /*Set up LED receiving*/
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2);
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2);

    /*Set up LED acknowledgement*/
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
}

/*
 * Indicates to website that commands have been received and executed
 * Can be checked on the website using IPaddress/ack
 */
int acknowledge()
{
    uint32_t HTTP_Request_Size = strlen(ack);
    printf("Strlen: %d", HTTP_Request_Size);
    fflush(stdout);
    char *ESP8266_Data = ESP8266_GetBuffer();
    UART_Flush(EUSCI_A2_BASE);
    GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);

    if (!ESP8266_EstablishConnection('0', TCP, HTTP_WebPage, Port))
    {
        commandHandler(ESP8266_Data);
        return 9;
    }

    if (!ESP8266_SendData('0', ack, HTTP_Request_Size))
    {
        commandHandler(ESP8266_Data);
        return 10;
    }
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
    return 0;
}

/*
 * Sequential series of commands to connect to the WIFI
 * Stages:
 * 0) Hard reset to clear previous settings
 * 1) Check UART connectivity with ESP8266
 * 2) Connect to Access Point
 * 3) Enable Mult.Connections to allow for sending & receiving
 * 4) Establish TCP connection with Server
 * 5) Send data over
 *
 * Uses LED to visually update users
 */

int runWifi()
{
    hardReset();

    char *ESP8266_Data = ESP8266_GetBuffer();

    if (!ESP8266_CheckConnection())
    {
        GPIO_setOutputLowOnPin(GPIO_PORT_P2,
        GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2);
        return 1;
    }

    clearBits();
    onRedLED();

    if (!ESP8266_ConnectToAP(AP_Name, AP_Pwd))
    {
        GPIO_setOutputLowOnPin(GPIO_PORT_P2,
        GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2);
        return 2;
    }

    clearBits();
    onGreenLED();

    if (!ESP8266_EnableMultipleConnections(false))
    {
        GPIO_setOutputLowOnPin(GPIO_PORT_P2,
        GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2);
        return 3;
    }

    clearBits();
    onYellowLED();

    if (!ESP8266_EstablishConnection('0', TCP, HTTP_WebPage, Port))
    {
        GPIO_setOutputLowOnPin(GPIO_PORT_P2,
        GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2);
        return 4;
    }

    clearBits();
    onBlueLED();

    if (!ESP8266_SendData('0', HTTP_Request, HTTP_Request_Size))
    {
        GPIO_setOutputLowOnPin(GPIO_PORT_P2,
        GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2);
        return 5;
    }

    commandHandler(ESP8266_Data);

    char reset[] = "GET /ack";
    strcpy(ack, reset);

    char str[30] = "";
    char end[13] = " \r\n\r\n";

    // append ch to str
    int i;
    for (i = 0; i < currentStage; i++)
    {
        sprintf(str, "R%dL%dE%d.", counterR[i], counterL[i], ET[i]);
        strncat(ack, str, 30);
    }

    strncat(ack, end, 13);
    printf("%s\n", ack);
    printf("Ack status: %d\n", acknowledge());
    fflush(stdout);

    clearBits();
    return 0;
}

void test()
{
    initWifi();
    commandHandler("MOV[1:1:3] MOV[4:1:2] MOV[3:1:2] MOV[1:1:3]");

    char str[11] = "";
    char end[13] = " \\r\\n\\r\\n";

    // append ch to str
    int i;
    for (i = 0; i < currentStage; i++)
    {
        sprintf(str, "R%dL%dE%d,", counterR[i], counterL[i], ET[i]);
        strncat(ack, str, 11);
    }

    strncat(ack, end, 13);
    printf("%s\n", ack);
}

void main()
{
    WDT_A_holdTimer();

    initialisePWM();
    Initialise_USM();
    Interrupt_enableMaster();
//    initWifi();
//    buzzer();
//    while (1)
//    {
//
//    }
//    printf("Size of %d Size of %d\n", strlen(HTTP_Request),sizeof(HTTP_Request));
//    fflush(stdout);
    /*
     * If status is anything other than 0 (correct code execution)
     * Terminate and rerun the Wifi module.
     */
//    test();
//    while(1)
//    {
//
//    }
    int status = 1;
    while (1)
    {
        if (status == 1)
        {
            initWifi();
            status = 0;
        }
        else
        {
            status = runWifi();
            printf("Status: %d\n", status);
            fflush(stdout);
        }
    }
}
