#ifndef __MAIN_PKG_H_
#define __MAIN_PKG_H_

#define PI      3.14159265
#define RADIUS  136.0 // robot wheel-base radius
#define MSTEP   16.0
#define RSTEP   200.0
#define RWHEEL  29.0 
#define REDUC   0.5
#define KSTP     ((PI*2.0*RWHEEL/(RSTEP*MSTEP))*REDUC)
#define SPEED   500.0 // max 50000 Mstepper 16 3200Ma
#define THETA   60.0 // trop angle sur le A

#define RED     "\033[0;31m"
#define CYAN    "\033[0;36m"
#define WHITE   "\033[0;37m"
#define GREEN   "\033[0;32m"
#define YELLOW  "\033[0;33m"

#define ACC    2.0
#define DEC    2.0


#define LIDAR_DIS_MIN        200.0
#define LIDAR_DIS_MAX        500.0
#define NB_LIDAR_PACK_READ   LIDAR_ANGLE_MARGIN*4.0
#define LIDAR_ANGLE_MARGIN   45.0
#define LIDAR_PC_ON   2.0
#define LIDAR_PC_OFF  80.0

enum {IDLE,START_UP,CAL,WAIT_MATCH,GAME,END}FsmState;  




#endif // __MAIN_PKG_H_