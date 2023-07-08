#include "mbed.h"
#include "pinout.hpp"
#include "Stepper.hpp"
#include "tim.h"
#include "develop_ticker.hpp"
#include <string.h>
#define MAXIMUM_BUFFER_SIZE  32
#define COEFF  0.01
#define LO  0//0.132 //132 cm
#define SPEED  20000 // max 50000 Mstepper 16 3200Ma
#define DIS    100000
BufferedSerial pc(USBTX, USBRX,115200);
BufferedSerial uart(D1, D0,115200);

//Stepper StepperA(STEP_M1, DIR_M1);
Stepper StepperB(STEP_M2, DIR_M2);
Stepper StepperC(STEP_M3, DIR_M3);
TestTicker test_ticker(STEP_M1);

// DigitalOut dirM1(DIR_M1);
// DigitalOut dirM2(DIR_M2);
// DigitalOut dirM3(DIR_M3);
// DigitalOut StpM1(STEP_M1);
// DigitalOut StpM2(STEP_M2);
// DigitalOut StpM3(STEP_M3);
DigitalOut pc_activity(LED1);
DigitalOut uart_activity(LED2);
AnalogIn   Poten(A0);
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
float percentage =0;
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
            //StepperA.stop();
            //StepperB.stop();
            //StepperC.stop();
            //StepperA.stop();
            //StepperA.setSpeed(10000);
        }                
        //printf("X==>%d Y==>%d\n",Vx,Vy);
     }
}
// void omni_move_xbox(const int Vx,const int Vy, float *Va, float *Vb, float *Vc)
// {
//     // *Va = ((-1/2)*Vx + (sqrt(3)/2)*Vy +LO)*COEFF;
//     // *Vb = ((-1/2)*Vx - (sqrt(3)/2)*Vy +LO)*COEFF;
//     // *Vc = (Vx + LO)*COEFF;

//     *Va = Vy;
   
// }


// void stepperB(void){
  
//     while (1)
//     {
//         StepperB.move(-DIS);
//         while(!StepperB.stopped());
//         StepperB.move(DIS);
//         while(!StepperB.stopped());
//     }
// }
// void stepperC(void){
//     while (1)
//     {
//         StepperC.move(-DIS);
//         while(!StepperC.stopped());
//         StepperC.move(DIS);
//         while(!StepperC.stopped());
//     }
// }

void show_thread(void){
    while (1)
    {
        
        printf("poten: %3.3f%%, %f\n",percentage, 15000*(percentage/100));
    }
}


void stepperA(void){
   
    while (1)
    {
         test_ticker.setSpeed(15000*(percentage/100));
    }
}

int main()
{ 
    
    
    //StepperC.setSpeed(1000);

  
    // StepperA.setSpeed(SPEED);
    // StepperA.setAcceleration(1500);
    // StepperA.rotate(0);
    // StepperA.stop();
    // StepperA.setPositionZero();
  



    //serial_thread.start(conCharReceived);
    // //time_upA.attach(stepperA,Te);
    test_ticker.setSpeed(2000);
    test_ticker.run();
    StepperA_thread.start(stepperA);


    // printf("INIT_Timer\n");
    // MX_TIM1_Init();
    
    // HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    // HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
    // htim1.Instance->ARR = 2;
    // htim1.Instance->CCR1 = 2/2;
    serial_thread.start(show_thread);
 

   
   
    while (1)
    {
        percentage = Poten.read()*100.0f;
        //omni_move_xbox(Vx,Vy,&Va,&Vb,&Vc);
        
        //wait_us(1000);
        //n=n+1;
        //printf("Va:%f Vd:%f Vc:%f\n",Va,Vb,Vc);
        //htim1.Instance->PSC =abs(Vc);
        
      
      
        

       
    }
}