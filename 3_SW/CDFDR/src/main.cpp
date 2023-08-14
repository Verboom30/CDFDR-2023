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
#define DIS    1000
#define ANGLE  90



BufferedSerial pc(USBTX, USBRX,115200);
BufferedSerial uart(D1, D0,115200);

Holonome* RobotMove = new Holonome();

DigitalOut pc_activity(LED1);
DigitalOut uart_activity(LED2);


AnalogIn   Poten(A0);
Thread serial_thread;
Thread plot_thread;
Thread show_pos_thread;
// Thread StepperA_thread;
// Thread StepperB_thread;
// Thread StepperC_thread;

float Te=0.01;
float V1=0;
float V2=0;
float V3=0;
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
void showPostion(void)
{
  while (1)
  {
    RobotMove->getPosition();
    printf("PosX:%f PosY:%f Alpha:%f\n",RobotMove->getPositionX(),RobotMove->getPositionY(),RobotMove->getAlpha());
  }
  
}

int main()
{ 


    //serial_thread.start(conCharReceived);
    //plot_thread.start(show_thread);
    show_pos_thread.start(showPostion);
  
    //RobotMove->setAcceleration(ACC);
    //RobotMove->setDeceleration(DEC);
    RobotMove->stop();
    RobotMove->setPositionZero();
    
    RobotMove->setSpeed(SPEED,0,0);
    RobotMove->move(DIS,0,0);
    while(!RobotMove->waitAck());
    while(!RobotMove->stopped()); 

    // RobotMove->setSpeed(0,0,SPEED);
    // RobotMove->move(0,0,ANGLE);
    // while(!RobotMove->waitAck());
    // while(!RobotMove->stopped()); 
  
 
   
   
    while (1)
    {

    
    
    
    


    
  

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