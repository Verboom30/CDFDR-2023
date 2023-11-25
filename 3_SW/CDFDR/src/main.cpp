#include "mbed.h"
#include "pinout.hpp"
#include "Stepper.hpp"
#include "tim.h"
#include "develop_ticker.hpp"
#include <string.h>
#include <math.h>
#define MAXIMUM_BUFFER_SIZE  32
#define COEFF  0.1
#define SPEED  20000 // max 50000 Mstepper 16 3200Ma
#define DIS    100000
#define PI 3.14159265
BufferedSerial pc(USBTX, USBRX,115200);
BufferedSerial uart(D1, D0,115200); //115200 Xbox_read 9600 blth

//Stepper StepperA(STEP_M1, DIR_M1);
//Stepper StepperB(STEP_M2, DIR_M2);
//Stepper StepperC(STEP_M3, DIR_M3);
TestTicker test_ticker(STEP_M1);
TestTicker test_ticker1(STEP_M2);
TestTicker test_ticker2(STEP_M3);

DigitalOut pc_activity(LED1);
DigitalOut uart_activity(LED2);

DigitalOut M1(DIR_M1);
DigitalOut M2(DIR_M2);
DigitalOut M3(DIR_M3);
Thread serial_thread;
Thread StepperA_thread;
Thread StepperB_thread;
Thread StepperC_thread;
Ticker time_upA;
Ticker time_upB;
Ticker time_upC;
float Te=0.01;
float V1=0;
float V2=0;
float V3=0;
int Vx =0;
int Vy =0;

float R = 0.136; // robot wheel-base radius
float theta_dot = 0; // robot wheel-base radius
float percentage =0;

PwmOut ServoB1(PA_0);
PwmOut ServoB2(PD_12);
PwmOut ServoB3(PD_13);


void print(const std::string &str) {
    pc.write(str.c_str(), str.length());
}

void conCharReceived(void)
{
  const char * separators = "\t\n";
  char buffer[MAXIMUM_BUFFER_SIZE] = {0};
   char CmdBlth[MAXIMUM_BUFFER_SIZE*2] = {0};
  int LeftHY=0;
  int LeftHX=0;
  int RightHY=0;
  int RightHX=0;
  int LT=0;
  int RT=0;
  size_t data_written = 0;
  while (1) {
    if (uint32_t num = uart.read(buffer, sizeof(buffer))) {
      uart_activity = !uart_activity;
      //pc.write(buffer,num);

      data_written =0;
      do
      {
        //printf("%c",buffer[data_written]);
        CmdBlth[data_written] = buffer[data_written]; 
        data_written++;
      } while (data_written < num);
      
    
      printf("%s",CmdBlth);

    

      // char * strToken = strtok ( buffer, separators );
      // while ( strToken != NULL ) {
      //   sscanf(strToken,"LeftHatY: %d",&LeftHY); 
      //   sscanf(strToken,"LeftHatX: %d",&LeftHX); 
      //   sscanf(strToken,"RightHatY: %d",&RightHY); 
      //   sscanf(strToken,"RightHatX: %d",&RightHX); 
      //   sscanf(strToken,"LT: %d",&LT); 
      //   sscanf(strToken,"RT: %d",&RT);
      //    Vx = LeftHX;
      //    Vy = -LeftHY;
      //    wait_us(500);

        // printf("LeftHX ==> %d ",LeftHX);
        // printf("LeftHY ==> %d ",LeftHY);
        // printf("RightHX ==> %d ",RightHX);
        // printf("RightHY ==> %d ",RightHY);
        // printf("LT ==> %d ",LT);
        // printf("RT ==> %d ",RT);
        // printf("\n");

      //   if(strstr(buffer,"A")){
           
      //   }
      //   if(strstr(buffer,"B")){
           
      //   }
      //   if(strstr(buffer,"X")){
           
      //   }
      //   if(strstr(buffer,"Y")){
           
      //   }
      //   if(strstr(buffer,"Up")){
      //       ServoB1.pulsewidth_us(500.0+(100.0/9.0)*180);
      //   }
      //   if(strstr(buffer,"Down")){
      //       ServoB1.pulsewidth_us(500.0+(100.0/9.0)*120);
      //   }
      //   if(strstr(buffer,"Left")){
            
      //   }
      //   if(strstr(buffer,"Right")){
            
      //   }
      //   if(strstr(buffer,"View")){
            
      //   }
      //   if(strstr(buffer,"Menu")){
            
      //   }
      //   if(strstr(buffer,"Xbox")){
            
      //   }
      //   if(strstr(buffer,"LB")){
            
      //   }
      //   if(strstr(buffer,"RB")){
            
      //   }
      //   if(strstr(buffer,"L3")){
            
      //   }
      //    if(strstr(buffer,"R3")){
            
      //   }
      
      //   // On demande le token suivant.
      //   strToken = strtok ( NULL, separators );
      // }   
   
        for (size_t i = 0; i < data_written; i++)
      {
         CmdBlth[i] = 0; // on vide le buffer
      }
      
    }   
    
  }  
    
}

void omni_move_xbox(const int Vx,const int Vy, float *V1, float *V2, float *V3)
{
    //*V1 = ((-1/2)*Vx - (sqrt(3)/2)*Vy +LO)*COEFF;
    //*V3 = ((-1/2)*Vx + (sqrt(3)/2)*Vy +LO)*COEFF;
    //*V2 = (Vx + LO)*COEFF;

   *V2 = (-R*theta_dot + Vx)*COEFF ;
   *V1 = (-R*theta_dot - 0.5*Vx - sin(PI/3.0)*Vy)*COEFF;
   *V3 = (-R*theta_dot - 0.5*Vx + sin(PI/3.0)*Vy)*COEFF;

    

    // *V1 = ((sqrt(pow(Vx,2)+pow(Vy,2)))*(sin(acos((Vx)/(sqrt(pow(Vx,2)+pow(Vy,2)))) - 90)))*COEFF;

    // *V2 = ((sqrt(pow(Vx,2)+pow(Vy,2)))*(sin(acos((Vx)/(sqrt(pow(Vx,2)+pow(Vy,2)))) + 30)))*COEFF;

    // *V3 = ((sqrt(pow(Vx,2)+pow(Vy,2)))*(sin(acos((Vx)/(sqrt(pow(Vx,2)+pow(Vy,2)))) + 150)))*COEFF;

}


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
        printf("poten: %3.3f%%, %f\n",percentage, 20000*(percentage/100));
    }
}


void stepperA(void){
    test_ticker.setSpeed(2000);
    test_ticker.run();
    while (1)
    {
         test_ticker.setSpeed((int)V1);
    }
}

void stepperB(void){
    test_ticker1.setSpeed(2000);
    test_ticker1.run();
    while (1)
    {
         test_ticker1.setSpeed((int)V2);
    }
}
void stepperC(void){
    test_ticker2.setSpeed(2000);
    test_ticker2.run();
    while (1)
    {
         test_ticker2.setSpeed((int)V3);
    }
}

int main()
{ 
    
    //serial_thread.start(BluetoothCmd);
    //show_pos_thread.start(showPostion);

  
    // StepperA.setSpeed(SPEED);
    // StepperA.setAcceleration(1500);
    // StepperA.rotate(0);
    // StepperA.stop();
    // StepperA.setPositionZero();
  
    ServoB1.period_ms(20);
    ServoB2.period_ms(20);
    ServoB3.period_ms(20);
    ServoB1.pulsewidth_us(500.0+(100.0/9.0)*180);
    ServoB2.pulsewidth_us(500.0+(100.0/9.0)*180);
    ServoB3.pulsewidth_us(500.0+(100.0/9.0)*180);
   



    serial_thread.start(conCharReceived);
    // //time_upA.attach(stepperA,Te);
    
    //test_ticker1.setSpeed(2000);
    //test_ticker1.run();
    //test_ticker2.setSpeed(2000);
    //test_ticker2.run();
    StepperA_thread.start(stepperA);
    StepperB_thread.start(stepperB);
    StepperC_thread.start(stepperC);


    // printf("INIT_Timer\n");
    // MX_TIM1_Init();
    
    // HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    // HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
    // htim1.Instance->ARR = 2;
    // htim1.Instance->CCR1 = 2/2;
    //serial_thread.start(show_thread);
 

   
   
    while (1)
    {
      // ServoB1.pulsewidth_us(500.0+(100.0/9.0)*180);
      // ServoB2.pulsewidth_us(500.0+(100.0/9.0)*180);
      // ServoB3.pulsewidth_us(500.0+(100.0/9.0)*180);
      // wait_us(2000000);
      // ServoB1.pulsewidth_us(500.0+(100.0/9.0)*110);
      // ServoB2.pulsewidth_us(500.0+(100.0/9.0)*110);
      // ServoB3.pulsewidth_us(500.0+(100.0/9.0)*110);
      // wait_us(2000000);
    
      //   wait_us(2000000);
      //  Myservo.pulsewidth_us(500.0+(100.0/9.0)*110);
  
      //   wait_us(2000000);

        omni_move_xbox(Vx,Vy,&V1,&V2,&V3);
        if(V1>0){
             M1 =1;
        }else{
            M1 =0;
        }
        
        if(V2>0){
             M2 =1;
        }else{
            
            M2 =0;
        }
        
        if(V3>0){
             M3 =1;
        }else{
            
            M3 =0;
        }
           
        
        
       
        
        //wait_us(1000);
        //n=n+1;
      
        //htim1.Instance->PSC =abs(Vc);
        
      
      
        

       
    }
}