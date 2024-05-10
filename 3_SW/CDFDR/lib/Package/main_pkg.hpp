#ifndef __MAIN_PKG_H_
#define __MAIN_PKG_H_

#define PI      3.14159265
#define RADIUS  121.0 // robot wheel-base radius
#define MSTEP   16.0
#define RSTEP   198.5
#define RWHEEL  27.5 
#define REDUC   0.5
#define KSTP     ((PI*2.0*RWHEEL/(RSTEP*MSTEP))*REDUC)
#define SPEED   800.0 // max 50000 Mstepper 16 3200Ma
#define THETA   60.0 // trop angle sur le A

#define RED     "\033[0;31m"
#define CYAN    "\033[0;36m"
#define WHITE   "\033[0;37m"
#define GREEN   "\033[0;32m"
#define YELLOW  "\033[0;33m"

#define ACC    4.0
#define DEC    4.0


#define LIDAR_DIS_MIN        250.0
#define LIDAR_DIS_MAX        800.0
#define NB_LIDAR_PACK_READ   LIDAR_ANGLE_MARGIN*4.0
#define LIDAR_ANGLE_MARGIN   60.0
#define LIDAR_PC_ON   5.0
#define LIDAR_PC_OFF  95.0

enum {IDLE,START_UP,CAL,WAIT_MATCH,GAME,END}FsmState;  




#endif // __MAIN_PKG_H_