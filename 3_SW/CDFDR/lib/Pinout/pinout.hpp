#define COEFF   1
#define PI      3.14159265
#define RADIUS  136.191// robot wheel-base radius
#define MSTEP   8
#define RSTEP   206
#define RWHEEL  30
#define REDUC   0.5

#ifndef __PINOUT_H_
#define __PINOUT_H_


// moteur 1
#define DIR_M1  D2
#define STEP_M1 D3

// moteur 2
#define DIR_M2  D4
#define STEP_M2 D5

// moteur 3
#define DIR_M3  D7
#define STEP_M3 D6 

// Bluetooth HC-06
#define UART_TX D1  
#define UART_RX D0

#endif // __PINOUT_H_
