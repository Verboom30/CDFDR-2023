#ifndef __PINOUT_H_
#define __PINOUT_H_

#define PI      3.14159265
#define RADIUS  136 // robot wheel-base radius
#define MSTEP   16
#define RSTEP   200.0
#define RWHEEL  29 
#define REDUC   0.5
#define KSTP     ((PI*2.0*RWHEEL/(RSTEP*MSTEP))*REDUC)
#define SPEED   150 // max 50000 Mstepper 16 3200Ma
#define THETA_A 60 // trop angle sur le A
#define THETA_C 60

#define RED     "\033[0;31m"
#define CYAN    "\033[0;36m"
#define WHITE   "\033[0;37m"
#define GREEN   "\033[0;32m"
#define YELLOW  "\033[0;33m"

#define ACC    2.0
#define DEC    2.0


// moteur A
#define DIR_M1  D37
#define STEP_M1 D38

// moteur B
#define DIR_M2  D2
#define STEP_M2 D3

// moteur C
#define DIR_M3  D4
#define STEP_M3 D5 

// moteur D
#define DIR_M4  D7
#define STEP_M4 D6

//Servo_bras 1
#define SERVO_B1  PA_0
//Servo_bras 2
#define SERVO_B2  PD_12
//Servo_bras 3
#define SERVO_B3  PD_13

//Servo_bras_1_prise_1
#define SERVO_B1_P1  PC_8
//Servo_bras_1_prise_2
#define SERVO_B1_P2  PE_6

//Servo_bras_2_prise_1
#define SERVO_B2_P1  PC_9
//Servo_bras_2_prise_2
#define SERVO_B2_P2  PB_10

//Servo_bras_3_prise_1
#define SERVO_B3_P1  PE_5
//Servo_bras_3_prise_2
#define SERVO_B3_P2  PE_11

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
