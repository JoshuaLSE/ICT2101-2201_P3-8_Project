;
//PWM methods
extern void initialisePWM(int facingDirection);
extern void emergencyStop(void);
extern int MOV(int direction, int speed, int duration);

//USM methods
extern void Initialise_USM(void);
extern float getHCSR04Distance(void);

//UART methods
extern initialiseUART();

extern void Delay();
