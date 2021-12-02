#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <stdlib.h>

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ESP8266.h>
#include <UART_Driver.h>
#include <MSPIO.h>

#include <sys_values.h>
#include <sys_API.h>

int Mutex = 1;
int Wifi = 1;

char HTTP_WebPage[] = "192.168.1.5";
char Port[] = "80";
char HTTP_Request[] = "GET / \r\n\r\n";

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

void commandHandler(char *a)
{
    char str[9999] = "";
    const char s[2] = " ";
    char *token;
    char movement[4] = "MOV";
    char light[4] = "LED";
    char buzzer[4] = "BUZ";

    char inputStr[99];

    char *movCmd;
    char *ledCmd;
    char *buzCmd;

    strcpy(str, a);

    printf("%s\n", str);
    fflush(stdout);

    /* get the first token */
    token = strtok(str, s);

    /* walk through other tokens */
    while (token != NULL)
    {
        strcpy(inputStr, token);
        movCmd = strstr(token, movement);
        ledCmd = strstr(token, light);
        buzCmd = strstr(token, buzzer);

        if (movCmd != NULL)
        {
            printf("mov received\n");
            fflush(stdout);
            Mutex = 0;
            int a = atoi(&movCmd[4]), b = atoi(&movCmd[6]), c = atoi(
                    &movCmd[8]);
            printf("Entering MOV a: %d b: %d c: %d\n", a, b, c);
            fflush(stdout);
            MOV(a, b, c);
            while (Mutex == 0)
            {
                float a = getHCSR04Distance();
                printf("Distance: %.3f\n", a);
                if(a < 5.00){
                    emergencyTrig = 1;
                    Mutex = 1;
                }
            }
        }
        else if (ledCmd != NULL)
        {
            printf("LED: %s\n", ledCmd);
        }
        else if (buzCmd != NULL)
        {
            printf("BUZ: %s\n", buzCmd);
        }
        fflush(stdout);

        token = strtok(NULL, s);
    }
}

void hardReset()
{
    ESP8266_HardReset();
    __delay_cycles(48000000);
    UART_Flush(EUSCI_A2_BASE);
}

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

    /*Set up LED feedback*/
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2);
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2);
}

int runWifi()
{
    /*Hard reset & flush to get rid of junk inputs*/
    hardReset();

    /*Pointer to ESP8266 global buffer*/
    char *ESP8266_Data = ESP8266_GetBuffer();

    int RGB = 0;
    P2OUT = RGB; //RED

    /*Check UART connection to MSP432*/
    if (!ESP8266_CheckConnection())
    {
        GPIO_setOutputLowOnPin(GPIO_PORT_P2,
        GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2);
        return 1;
    }

    RGB++;
    P2OUT = RGB; //GREEN

    Delay(100000);
    /*Check available Access Points*/
    if (!ESP8266_AvailableAPs())
    {
        GPIO_setOutputLowOnPin(GPIO_PORT_P2,
        GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2);
        return 2;
    }

    RGB++;
    P2OUT = RGB; //YELLOW

    Delay(100000);
    /*Connect to Access Point if necessary here*/
    if (!ESP8266_ConnectToAP("networknamehere", "networkpwd"))
    {
        GPIO_setOutputLowOnPin(GPIO_PORT_P2,
        GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2);
        return 3;
    }

    RGB++;
    P2OUT = RGB; //BLUE

    Delay(100000);
    if (!ESP8266_EnableMultipleConnections(false))
    {
        GPIO_setOutputLowOnPin(GPIO_PORT_P2,
        GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2);
        return 4;
    }

    RGB++;
    P2OUT = RGB; //PURPLE

    Delay(100000);
    /*Try to establish TCP connection to a HTTP server*/
    if (!ESP8266_EstablishConnection('0', TCP, HTTP_WebPage, Port))
    {
        GPIO_setOutputLowOnPin(GPIO_PORT_P2,
        GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2);
        return 5;
    }

    RGB++;
    P2OUT = RGB; //TURQUOISE

    Delay(10000);
    /*Query data to connected HTTP server, in order to do this look for an API and request a key*/
    if (!ESP8266_SendData('0', HTTP_Request, HTTP_Request_Size))
    {
        GPIO_setOutputLowOnPin(GPIO_PORT_P2,
        GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2);
        return 6;
    }

    commandHandler(ESP8266_Data);
    RGB++;
    P2OUT = RGB; //WHITE

    return 0;
}

void main()
{
    WDT_A_holdTimer();

    initialisePWM(1);
    Initialise_USM();
    int status = 1;
    Interrupt_enableMaster();
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
