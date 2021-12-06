//PWM methods
extern void initialisePWM();
extern void emergencyStop(void);
extern int MOV(int direction, int speed, int duration);

//USM methods
extern void Initialise_USM(void);
extern float getHCSR04Distance(void);

//UART methods
extern initialiseUART();

extern void Delay();

//LED methods
extern void clearBits(void);
extern void onRedLED(void);
extern void onGreenLED(void);
extern void onBlueLED(void);
extern void onYellowLED(void);
extern void onPurpleLED(void);
extern void onCyanLED(void);
extern void onWhiteLED(void);
extern void onChangingLEDColor(void);
extern void onRedLED_Blink(void);
extern void onGreenLED_Blink(void);
extern void onBlueLED_Blink(void);
extern void onYellowLED_Blink(void);
extern void onPurpleLED_Blink(void);
extern void onCyanLED_Blink(void);
extern void onWhiteLED_Blink(void);

//Buzzer methods
extern void buzzer(void);

//IR Sensor methods
extern void irLineTracking_UsingTimer(void);
extern void boundsCheck(void);
