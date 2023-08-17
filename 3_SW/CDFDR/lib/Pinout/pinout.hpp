
#define PI      3.14159265
#define RADIUS  136.191// robot wheel-base radius
#define MSTEP   16
#define RSTEP   206.0
#define RWHEEL  30.0
#define REDUC   0.5
#define KSTP    ((PI*2.0*RWHEEL/(RSTEP*MSTEP))*REDUC)
#define SPEED   5000 // max 50000 Mstepper 16 3200Ma

#define RED     "\033[0;31m"
#define CYAN    "\033[0;36m"
#define WHITE   "\033[0;37m"
#define GREEN   "\033[0;32m"
#define YELLOW  "\033[0;33m"

#define ACC    4.0
#define DEC    4.0

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
