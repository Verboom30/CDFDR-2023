#include "mbed.h"
#include "pinout.hpp"
#include "tim.h"
#include "develop_ticker.hpp"
#include "Holonome.hpp"
#include <string.h>
#include <math.h>
#define MAXIMUM_BUFFER_SIZE  128
// #define COEFF  0.1
#define SPEED  3000 // max 50000 Mstepper 16 3200Ma
#define DIS    300
#define ANGLE  90
#define ACC    4
#define DEC    4

// #define PI 3.14159265
BufferedSerial pc(USBTX, USBRX,115200);
BufferedSerial uart(D1, D0,115200);

// Stepper StepperA(STEP_M1, DIR_M1);
// Stepper StepperB(STEP_M2, DIR_M2);
// Stepper StepperC(STEP_M3, DIR_M3);
Holonome* RobotMove = new Holonome();

DigitalOut pc_activity(LED1);
DigitalOut uart_activity(LED2);


AnalogIn   Poten(A0);
Thread serial_thread;
Thread plot_thread;
// Thread StepperA_thread;
// Thread StepperB_thread;
// Thread StepperC_thread;

float Te=0.01;
float V1=0;
float V2=0;
float V3=0;
int Dstep=0;
int Astep=0;
int Vx=0;
int Vy=0;
int Vx2=0;
int Vy2=0;
float R = 0.136; // robot wheel-base radius
float theta_dot = 0; // robot wheel-base radius
float percentage =0;


void print(const std::string &str) {
    pc.write(str.c_str(), str.length());
}

void conCharReceived(void)
{
  const char * separators = "\t\n";
  char buffer[MAXIMUM_BUFFER_SIZE] = {0};
  while (1) {
    if (uint32_t num = uart.read(buffer, sizeof(buffer))) {
      uart_activity = !uart_activity;
      //pc.write(buffer,num);

      char * strToken = strtok ( buffer, separators );
      while ( strToken != NULL ) {
          sscanf(strToken,"LeftHatY: %d",&Vy); 
          sscanf(strToken,"LeftHatX: %d",&Vx); 
          if(Vx>1000 or Vx<-1000)Vx2 =Vx;
          if(Vy>1000 or Vy<-1000)Vy2 =-Vy;   
          //printf("X==>%d Y==>%d\n",Vx2,Vy2);
          printf("V1:%f V2:%f V3:%f\n",V1,V2,V3);
          // On demande le token suivant.
          strToken = strtok ( NULL, separators );
      }    
    }   
    
  }  
    
}

int main()
{ 


    //serial_thread.start(conCharReceived);
    //plot_thread.start(show_thread);
  
    Dstep = DIS/((PI*2*RWHEEL/(RSTEP*MSTEP))*REDUC);
    Astep = (ANGLE*(PI/180))/((PI*2*RWHEEL/(RSTEP*MSTEP))*REDUC);
    RobotMove->setSpeed(0,0,SPEED/20);
    RobotMove->setAcceleration(ACC);
    RobotMove->setDeceleration(DEC);
    RobotMove->stop();
    RobotMove->setPositionZero();
    
  
 
   
   
    while (1)
    {

    RobotMove->setSpeed(0,SPEED,0);
    RobotMove->move(0,Dstep,0);
    while(!RobotMove->waitAck());
    while(!RobotMove->stopped()); 
    
    
    RobotMove->setSpeed(0,0,SPEED/30);
    RobotMove->move(0,0,Astep);
    while(!RobotMove->waitAck());
    while(!RobotMove->stopped()); 

    
  

      // RobotMove->move(0,Dstep,0);
      // while(!RobotMove->waitAck());
      // while(!RobotMove->stopped()); 

   

      // RobotMove->move(-Dstep,0,0);
      // while(!RobotMove->waitAck());
      // while(!RobotMove->stopped()); 

      // RobotMove->move(0,-Dstep,0);
      // while(!RobotMove->waitAck());
      // while(!RobotMove->stopped()); 
   
    }
}