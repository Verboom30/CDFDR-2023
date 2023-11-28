
#define PI      3.14159265
#define RADIUS  148.5// robot wheel-base radius
#define MSTEP   16
#define RSTEP   206.0
#define RWHEEL  30.0
#define REDUC   0.5
#define KSTP    ((PI*2.0*RWHEEL/(RSTEP*MSTEP))*REDUC)
#define SPEED   15000 // max 50000 Mstepper 16 3200Ma

#define RED     "\033[0;31m"
#define CYAN    "\033[0;36m"
#define WHITE   "\033[0;37m"
#define GREEN   "\033[0;32m"
#define YELLOW  "\033[0;33m"

#define ACC    2.0
#define DEC    2.0

#ifndef __PINOUT_H_
#define __PINOUT_H_


// moteur 1
#define DIR_M1  D7
#define STEP_M1 D6

// moteur 2
#define DIR_M2  D4
#define STEP_M2 D5

// moteur 3
#define DIR_M3  D2
#define STEP_M3 D3 

// moteur 4
#define DIR_M4  D37
#define STEP_M4 D38

//Servo_bras 1
#define SERVO_B1  PA_0
//Servo_bras 2
#define SERVO_B2  PD_12
//Servo_bras 3
#define SERVO_B3  PD_13

//Servo_prise 1
#define SERVO_P1  PC_8
//Servo_prise 2
#define SERVO_P2  PC_9
//Servo_prise 3
#define SERVO_P3  PE_5

//Turbine 1
#define TURB1  PF_8
//Turbine 2
#define TURB2  PF_7
//Turbine 3
#define TURB3  PF_9


// Bluetooth HC-06
#define UART_TX D1  
#define UART_RX D0

#endif // __PINOUT_H_
