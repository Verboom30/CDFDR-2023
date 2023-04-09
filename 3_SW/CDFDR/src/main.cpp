#include "mbed.h"
#include "pinout.hpp"
#include "Stepper.hpp"
#include "tim.h"
#include <string.h>
#define MAXIMUM_BUFFER_SIZE  32
#define COEFF  0.01
#define LO  0//0.132 //132 cm
#define SPEED  4000
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
        // StepperA.move(3000);
        // while(!StepperA.stopped());

    }
}
void stepperB(void){
  
    while (1)
    {
        StepperB.move(-3000);
        while(!StepperB.stopped());
        // StepperB.move(3000);
        // while(!StepperB.stopped());
    }
}
void stepperC(void){
    while (1)
    {
        StepperC.move(-3000);
        while(!StepperC.stopped());
        // StepperC.move(3000);
        // while(!StepperC.stopped());
    }
}

int main()
{ 
    

    //StepperC.setSpeed(1000);

    
    // StepperA.setSpeed(SPEED);
    // StepperA.rotate(0);
    // StepperB.setSpeed(SPEED);
    // StepperB.rotate(0);
    // StepperC.setSpeed(SPEED);
    // StepperC.rotate(0);
    // StepperA.stop();
    // StepperA.setPositionZero();
    // StepperB.stop();
    // StepperB.setPositionZero();
    // StepperC.stop();
    // StepperC.setPositionZero();
   





    // serial_thread.start(conCharReceived);
    // //time_upA.attach(stepperA,Te);
    // StepperA_thread.start(stepperA);
    // StepperB_thread.start(stepperB);
    // StepperC_thread.start(stepperC);
    printf("INIT_Timer\n");
    MX_TIM1_Init();
    
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
    htim1.Instance->CCR1 = 40;
    while (1)
    {
    //   omni_move_xbox(Vx,Vy,&Va,&Vb,&Vc);
    //   printf("Va:%f Vd:%f Vc:%f\n",Va,Vb,Vc);


    }
}