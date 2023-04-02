#include "mbed.h"
#include "pinout.hpp"
#include "Stepper.hpp"
#include <string.h>
#define MAXIMUM_BUFFER_SIZE  32
#define COEFF  0.01
#define LO  0//0.132 //132 cm
#define SPEED  40000
BufferedSerial pc(USBTX, USBRX,115200);
BufferedSerial uart(D1, D0,115200);

Stepper StepperA(STEP_M1, DIR_M1);
Stepper StepperB(STEP_M2, DIR_M2);
Stepper StepperC(STEP_M3, DIR_M3);

// DigitalOut dirM1(DIR_M1);
// DigitalOut dirM2(DIR_M2);
// DigitalOut dirM3(DIR_M3);
// DigitalOut StpM1(STEP_M1);
// DigitalOut StpM2(STEP_M2);
// DigitalOut StpM3(STEP_M3);
DigitalOut pc_activity(LED1);
DigitalOut uart_activity(LED2);
Thread serial_thread;
Thread StepperA_thread;
Thread StepperB_thread;
Thread StepperC_thread;
Ticker time_upA;
Ticker time_upB;
Ticker time_upC;
float Te=0.01;
float Va=0;
float Vb=0;
float Vc=0;
int Vx=0;
int Vy=0;

void conCharReceived(void)
{
     char buffer[MAXIMUM_BUFFER_SIZE] = {0};
     while(1){
        uart.read(buffer, sizeof(buffer));
        uart_activity = !uart_activity;

        sscanf(buffer,"Y: %d",&Vy); 
        sscanf(buffer,"X: %d",&Vx); 

        //if((12000>Vx) and (Vx>-12000)) Vx =0;
        //if((12000>Vy) and (Vy>-12000)) Vy =0;

        if(strstr(buffer,"A")){
            Vy =0;   
            Vx =0; 
            StepperA.stop();
            StepperB.stop();
            StepperC.stop();
            //StepperA.stop();
            //StepperA.setSpeed(10000);
        }                
        //printf("X==>%d Y==>%d\n",Vx,Vy);
     }
}
void omni_move_xbox(const int Vx,const int Vy, float *Va, float *Vb, float *Vc)
{
    *Va = ((-1/2)*Vx + (sqrt(3)/2)*Vy +LO)*COEFF;
    *Vb = ((-1/2)*Vx - (sqrt(3)/2)*Vy +LO)*COEFF;
    *Vc = (Vx + LO)*COEFF;
}

void stepperA(void){
   
    while (1)
    {
        StepperA.move(-3000);
        while(!StepperA.stopped());
        StepperA.move(3000);
        while(!StepperA.stopped());

    }
}
void stepperB(void){
    
    StepperB.rotate(0);
    StepperB.setSpeed(20000);
    StepperB.setAcceleration(8000);
    StepperB.setDeceleration(8000);
    StepperB.stop();
    StepperB.setPositionZero();
    while (1)
    {
        StepperB.move(-3000);
        while(!StepperB.stopped());
        StepperB.move(3000);
        while(!StepperB.stopped());
    }
}
void stepperC(void){
    StepperC.rotate(0);
    StepperC.setSpeed(20000);
    StepperC.setSpeed(20000);
    StepperC.setAcceleration(8000);
    StepperC.setDeceleration(8000);
    StepperC.stop();
    StepperC.setPositionZero();
    while (1)
    {
        StepperC.move(-3000);
        while(!StepperC.stopped());
        StepperC.move(3000);
        while(!StepperC.stopped());
    }
}

int main()
{ 
    

    //StepperC.setSpeed(1000);

    
    StepperA.setSpeed(SPEED);
    StepperA.rotate(0);
    StepperB.setSpeed(SPEED);
    StepperB.rotate(0);
    StepperC.setSpeed(SPEED);
    StepperC.rotate(0);
    //StepperA.setAcceleration(8000);
    //StepperA.setDeceleration(8000);
    //StepperA.stop();
    //StepperA.setPositionZero();




    serial_thread.start(conCharReceived);
    //time_upA.attach(stepperA,Te);
    //StepperB_thread.start(stepperB);
    //StepperC_thread.start(stepperC);
    while (1)
    {
     
       omni_move_xbox(Vx,Vy,&Va,&Vb,&Vc);
       printf("Va:%f Vd:%f Vc:%f\n",Va,Vb,Vc);

      
   
    


        // if(Va>20){
        //     StepperA.rotate(1);
        // }else if(Va<-20){
        //     StepperA.rotate(0);
        // }
        // if(Vb>20){
        //     StepperB.rotate(1);
        // }else if(Vb<-20){
        //     StepperB.rotate(0);
        // }
        // if(Vc>20){
        //     StepperC.rotate(1);
        // }else if(Vc<-20){
        //     StepperC.rotate(0);
        // }
        // if(abs(Va)>50){
        //     StepperA.stop(); 
        //       StepperA.setSpeed(abs(Va*5));
        // }else{
        //       StepperA.stop(); 
        // }
        // if(abs(Vb)>50){
        //         StepperB.stop();
        //        StepperB.setSpeed(abs(Vb*5));
        // }else{
        //       StepperB.stop(); 
        // }
        // if(abs(Vc)>50){
        //     StepperC.stop(); 
        //     StepperC.setSpeed(abs(Vc*5));
        // }else{
        //      StepperC.stop(); 
        // }
      
      
      
        

      
        
        
  
    }
}