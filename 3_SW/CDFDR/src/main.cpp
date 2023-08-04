#include "mbed.h"
#include "pinout.hpp"
#include "tim.h"
#include "develop_ticker.hpp"
#include "Holonome.hpp"
#include <string.h>
#include <math.h>
#define MAXIMUM_BUFFER_SIZE  128
// #define COEFF  0.1
#define SPEED  2000 // max 50000 Mstepper 16 3200Ma
#define DIS    200

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

// void omni_move_xbox(const int Vx,const int Vy, float *V1, float *V2, float *V3)
// {

//    *V2 = (-R*theta_dot + Vx)*COEFF ;
//    *V1 = (-R*theta_dot - 0.5*Vx - sin(PI/3.0)*Vy)*COEFF;
//    *V3 = (-R*theta_dot - 0.5*Vx + sin(PI/3.0)*Vy)*COEFF;
// }

// void stepperA(void){
//     StepperA.setSpeed(2000);
//     StepperA.run();
//     while (1)
//     {
//          StepperA.setSpeed((int)15000*(percentage/100));
//     }
// }

// void stepperB(void){
//     StepperB.setSpeed(2000);
//     StepperB.run();
//     while (1)
//     {
//          StepperB.setSpeed((int)15000*(percentage/100));
//     }
// }
// void stepperC(void){
//     StepperC.setSpeed(2000);
//     StepperC.run();
//     while (1)
//     {
//          StepperC.setSpeed((int)15000*(percentage/100));
//     }
// }

// void show_thread(void){
//     while (1)
//     {
        
//         printf("poten: %3.3f%%, %f\n",percentage, 15000*(percentage/100));
//     }
// }


int main()
{ 


    //serial_thread.start(conCharReceived);
    //plot_thread.start(show_thread);
  
    //StepperA_thread.start(stepperA);
    //StepperB_thread.start(stepperB);
    //StepperC_thread.start(stepperC);
    // StepperA.setSpeed(20000);
    // StepperA.setAcceleration(4000);
    // StepperA.setDeceleration(4000);
    // StepperA.stop();
    // StepperA.setPositionZero();
    // StepperA.goesTo(16000);
    // while(!StepperA.stopped());  
    Dstep = DIS/((PI*2*RWHEEL/(RSTEP*MSTEP))*REDUC);
    RobotMove->setSpeed(SPEED,0,0);
    RobotMove->setAcceleration(0);
    RobotMove->setDeceleration(0);
    RobotMove->stop();
    RobotMove->setPositionZero();
   
   
    while (1)
    {
  
      RobotMove->move(Dstep,0,0);
      while(!RobotMove->waitAck());
      while(!RobotMove->stopped()); 

      RobotMove->move(-Dstep,0,0);
      while(!RobotMove->waitAck());
      while(!RobotMove->stopped()); 
   
    }
}