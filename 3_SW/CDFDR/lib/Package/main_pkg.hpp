#ifndef __MAIN_PKG_H_
#define __MAIN_PKG_H_

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


enum {IDLE,START_UP,CAL,WAIT_MATCH,GAME,END}FsmState;  


#endif // __MAIN_PKG_H_