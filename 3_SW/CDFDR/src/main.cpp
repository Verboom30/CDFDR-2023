#include "mbed.h"
#include "pinout.hpp"
#include "main_pkg.hpp"
#include "develop_ticker.hpp"
#include "Holonome.hpp"
#include "servo.hpp"
#include "lcd.hpp"
#include "lidar.hpp"
#include <string.h>
#include <math.h>
#define MAXIMUM_BUFFER_SIZE  32


BufferedSerial pc(USBTX, USBRX,230400);
//BufferedSerial uart(D1, D0,230400); //115200 Xbox_read 9600 blth


Holonome* RobotMove = new Holonome();
Lidar*    LidarLD19 = new Lidar(LIDAR_TX, LIDAR_RX,230400);

LiDARFrameTypeDef LidarPoints;

DigitalOut pc_activity(LED1);
DigitalOut uart_activity(LED2);

Thread serial_thread;
Thread show_pos_thread;

float Te=0.01;
float V1=0;
float V2=0;
float V3=0;

float R = 0.136; // robot wheel-base radius
float theta_dot = 0; // robot wheel-base radius
float percentage =0;

PwmOut ServoB1(SERVO_B1);
PwmOut ServoB2(SERVO_B2);
PwmOut ServoB3(SERVO_B3);

PwmOut Turbine1(TURB1);
PwmOut Turbine2(TURB2);
PwmOut Turbine3(TURB3);

PwmOut ServoB1P1(SERVO_B1_P1);
PwmOut ServoB1P2(SERVO_B1_P2);

PwmOut ServoB2P1(SERVO_B2_P1);
PwmOut ServoB2P2(SERVO_B2_P2);

PwmOut ServoB3P1(SERVO_B3_P1);
PwmOut ServoB3P2(SERVO_B3_P2);

LCD lcd(LCD_RS,LCD_EN,LCD_D4,LCD_D5,LCD_D6,LCD_D7, LCD16x2); // rs, e, d4-d7

DigitalIn  Button_init(BT_INIT);
DigitalOut Button_init_gnd(BT_INIT_GND);

DigitalIn  Tirette(BT_TIRETTE);


void showPostion(void)
{
  while (1)
  {
    // printf("PosX:%f PosY:%f Alpha:%f  SpeedX:%f SpeedY:%f SpeedAlpha:%f SpeedA:%f SpeedB:%f SpeedC:%f \n"
    // ,RobotMove->getPositionX(),RobotMove->getPositionY(),RobotMove->getAlpha(),RobotMove->getSpeedX(),RobotMove->getSpeedY(),
    // RobotMove->getSpeedAlpha(),RobotMove->getSpeedA(),RobotMove->getSpeedB(),RobotMove->getSpeedC()
    // );

    // printf("PosX:%f PosY:%f Alpha:%f  SpeedX:%f SpeedY:%f SpeedAlpha:%f PosA:%d PosB:%d PosC:%d \n"
    // ,RobotMove->getPositionX(),RobotMove->getPositionY(),RobotMove->getAlpha(),RobotMove->getSpeedX(),RobotMove->getSpeedY(),
    // RobotMove->getSpeedAlpha(),RobotMove->getPosA(),RobotMove->getPosB(),RobotMove->getPosC()
    // );

    // printf("PosX:%f PosY:%f Alpha:%f  SpeedX:%f SpeedY:%f SpeedAlpha:%f StepA:%d StepB:%d StepC:%d SpeedA:%f SpeedB:%f SpeedC:%f\n"
    // ,RobotMove->getPositionX(),RobotMove->getPositionY(),RobotMove->getAlpha(),RobotMove->getSpeedX(),RobotMove->getSpeedY(),
    // RobotMove->getSpeedAlpha(),RobotMove->getStepA(),RobotMove->getStepB(),RobotMove->getStepC(),RobotMove->getSpeedA(),RobotMove->getSpeedB(),RobotMove->getSpeedC()
    // );

    printf("%f;%f;%f\r\n",RobotMove->getPositionX(),RobotMove->getPositionY(),RobotMove->getAlpha());
 
  }
  
}

void showLidar(void)
{
  while (1)
  {
    LidarPoints = LidarLD19->GetPoints();
    for (uint8_t i = 0; i < POINT_PER_PACK; i++)
    {
      //printf("[%2d] Dis=%5d Intsy=%5d Agl=%5.f\r\n",i,LidarPoints.point[i].distance,LidarPoints.point[i].intensity,(LidarPoints.point[i].angle/100));
     
      if(LidarPoints.point[i].intensity >200)  printf("%f;%d\r\n",(LidarPoints.point[i].angle/100),LidarPoints.point[i].distance);
     
    }
    //printf("\r\n");
  }
  
}

void ShowLidarCoord(void)
{
  float PointLidarX =0;
  float PointLidarY =0;
 while (1)
  {
    LidarPoints = LidarLD19->GetPoints();
   
    for (uint8_t i = 0; i < POINT_PER_PACK; i++)
    {
     
      //printf("%5.f;%5d\r\n",i,(LidarPoints.point[i].angle/100),LidarPoints.point[i].distance);
      float LidarX = cos((PI/180)*float(LidarPoints.point[i].angle/100)+90)*LidarPoints.point[i].distance;
      float LidarY = -sin((PI/180)*float(LidarPoints.point[i].angle/100)+90)*LidarPoints.point[i].distance;
      if(LidarX>0 and LidarX<2000 and LidarY>0 and LidarY<3000) PointLidarX = LidarX,PointLidarY = LidarY;
      //printf("%f;%f;%f;%f;%f\r\n",RobotMove->getPositionX(),RobotMove->getPositionY(),RobotMove->getAlpha(),PointLidarX,PointLidarY);
     //printf("%f;%f;%f;%f;%f\r\n",RobotMove->getPositionX(),RobotMove->getPositionY(),RobotMove->getAlpha(),LidarX,LidarY);
     
     if(LidarPoints.point[i].intensity >200)printf("%f;%f;%f;%f;%d\r\n",RobotMove->getPositionX(),RobotMove->getPositionY(),RobotMove->getAlpha(),LidarPoints.point[i].angle/100,LidarPoints.point[i].distance);
      
      //printf("%f;%f;%f;%d\r\n",RobotMove->getPositionX(),RobotMove->getPositionY(),(LidarPoints.point[i].angle/100),LidarPoints.point[i].distance);
      //printf("%f;%f;%f;%f\r\n",RobotMove->getPositionX(),RobotMove->getPositionY(),LidarX,LidarY);
      //printf("%5.f;%5.f\r\n", RobotMove->getPositionX()+cos((PI/180)*(LidarPoints.point[i].angle/100)-90-RobotMove->getAlpha())*LidarPoints.point[i].distance,RobotMove->getPositionY()+sin((PI/180)*(LidarPoints.point[i].angle/100)+90-RobotMove->getAlpha())*LidarPoints.point[i].distance);
    }
    printf("\r\n");
  }
}



float theta2pluse(int theta)
{
  return 500.0+(100.0/9.0)*float(theta);
}


int main()
{ 
    Button_init.mode(PullUp);
    Tirette.mode(PullUp);
    Button_init_gnd = 0;
    FsmState = IDLE;
    lcd.printf("Wait Calibration\n");
    
    //serial_thread.start(Xbox_read);
    //serial_thread.start(BluetoothCmd);
    //show_pos_thread.start(showPostion);
    //show_pos_thread.start(showLidar);
    show_pos_thread.start(ShowLidarCoord);
 
    RobotMove->stop();
    RobotMove->setPositionZero();
    

    ServoB1.period_ms(20);
    ServoB2.period_ms(20);
    ServoB3.period_ms(20);
    Turbine1.period_ms(20);
    Turbine2.period_ms(20);
    Turbine3.period_ms(20);   
    ServoB1P1.period_ms(20);
    ServoB1P2.period_ms(20);
    ServoB2P1.period_ms(20);
    ServoB2P2.period_ms(20);
    ServoB3P1.period_ms(20);
    ServoB3P2.period_ms(20);
    Turbine1.pulsewidth_us(1000);
    Turbine2.pulsewidth_us(1000);
    Turbine3.pulsewidth_us(1000);
    RobotMove->setPosition(1500,1000,0);
    
    while (1)
    {
     
    
      switch(FsmState){
        case IDLE :
          if(Button_init != 1){ // Attente boutton d'init 
            lcd.cls();
            lcd.printf("Start Up !\n");
            FsmState = START_UP; //Allumage actionneurs 
          } 
          break;

        case START_UP :
         
          ServoB1.pulsewidth_us(theta2pluse(Bras[0].pos_up));
          ServoB2.pulsewidth_us(theta2pluse(Bras[1].pos_up));
          ServoB3.pulsewidth_us(theta2pluse(Bras[2].pos_up));

          ServoB1P1.pulsewidth_us(theta2pluse(Pince[0].pos_open));
          ServoB1P2.pulsewidth_us(theta2pluse(Pince[1].pos_open));

          ServoB2P1.pulsewidth_us(theta2pluse(Pince[2].pos_open));
          ServoB2P2.pulsewidth_us(theta2pluse(Pince[3].pos_open));

          ServoB3P1.pulsewidth_us(theta2pluse(Pince[4].pos_open));
          ServoB3P2.pulsewidth_us(theta2pluse(Pince[5].pos_open));
          HAL_Delay (2000); // Attente de 2 secondes 
          lcd.cls();
          lcd.printf("Calibration !\n");
          FsmState = CAL;
          break;
            
        
        case CAL :
          // RobotMove->move(200,0,0);
          // while(!RobotMove->waitAck());
          // while(!RobotMove->stopped());
          // RobotMove->move(-200,0,0);
          // while(!RobotMove->waitAck());
          // while(!RobotMove->stopped());
          // while(!RobotMove->stopped()); 
          // RobotMove->goesTo(0,0,0);
          // while(!RobotMove->waitAck());
          // while(!RobotMove->stopped()); 
          // RobotMove->goesTo(0,-300,0);
          // while(!RobotMove->waitAck());
          // while(!RobotMove->stopped()); 
          // RobotMove->goesTo(0,0,0);
          // while(!RobotMove->waitAck());
          // while(!RobotMove->stopped());
          
          FsmState = WAIT_MATCH;
          lcd.cls();
          lcd.printf("Wait Match !\n");
          break;

        case WAIT_MATCH :
          if(Tirette != 1){
            lcd.cls();
            lcd.printf("GAME !\n");
            FsmState = GAME; //Lancement du match !
          }
          break;

        case GAME :
        

          RobotMove->goesTo(1000,1000,0);
          while(!RobotMove->waitAck());
          while(!RobotMove->stopped());

          RobotMove->goesTo(1000,1000,90);
          while(!RobotMove->waitAck());
          while(!RobotMove->stopped());

          RobotMove->goesTo(2000,1000,90);
          while(!RobotMove->waitAck());
          while(!RobotMove->stopped());

          RobotMove->goesTo(2000,1000,180);
          while(!RobotMove->waitAck());
          while(!RobotMove->stopped());







          // ServoB1P1.pulsewidth_us(theta2pluse(Pince[0].pos_open));
          // ServoB1P2.pulsewidth_us(theta2pluse(Pince[1].pos_open));
          // ServoB2P1.pulsewidth_us(theta2pluse(Pince[2].pos_open));
          // ServoB2P2.pulsewidth_us(theta2pluse(Pince[3].pos_open));
          // ServoB3P1.pulsewidth_us(theta2pluse(Pince[4].pos_open));
          // ServoB3P2.pulsewidth_us(theta2pluse(Pince[5].pos_open));
          // ServoB1.pulsewidth_us(theta2pluse(Bras[0].pos_up));
          // ServoB2.pulsewidth_us(theta2pluse(Bras[1].pos_up));
          // ServoB3.pulsewidth_us(theta2pluse(Bras[2].pos_up));
          // HAL_Delay (1000); // Attente de 2 secondes 
          // ServoB1P1.pulsewidth_us(theta2pluse(Pince[0].pos_close));
          // ServoB1P2.pulsewidth_us(theta2pluse(Pince[1].pos_close));
          // ServoB2P1.pulsewidth_us(theta2pluse(Pince[2].pos_close));
          // ServoB2P2.pulsewidth_us(theta2pluse(Pince[3].pos_close));
          // ServoB3P1.pulsewidth_us(theta2pluse(Pince[4].pos_close));
          // ServoB3P2.pulsewidth_us(theta2pluse(Pince[5].pos_close));
          // ServoB1.pulsewidth_us(theta2pluse(Bras[0].pos_down));
          // ServoB2.pulsewidth_us(theta2pluse(Bras[1].pos_down));
          // ServoB3.pulsewidth_us(theta2pluse(Bras[2].pos_down));
          // HAL_Delay (1000); // Attente de 2 secondes 


          FsmState = END; //Lancement du match !
          break;

        case END :
          break;
      }
    }
}