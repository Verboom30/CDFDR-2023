#include "mbed.h"
#include "pinout.hpp"
#include "main_pkg.hpp"
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
DigitalOut led_lidar(LED_LIDAR);

Thread serial_thread;
Thread show_pos_thread;
Thread game_thread;
Thread lcd_thread;


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
DigitalIn  Switch_Team(SWITCH_TEAM);

float PointLidarX =0;
float PointLidarY =0;
float AngleCible  =0;
float AngleCible_Down =0;
float AngleCible_Top  =0;


int   NbDetecLidarPack =0;
int   NbNoDetecLidarPack =0;
bool   Stop             =false;
int   SumNoStop        =0;
int   SumStop          =0;
int   Couleur_Team     =0; // 0 bleu 1 jaune


float AngleLidar      =0;
int   DistanceLidar   =0;

volatile bool end_match = false;
Timeout endMatch;

int score = 0;
int offset_posX =0;
int offset_Alpha =1;

void endMatchProcess(){
      end_match = true;
}


void showPostion(void)
{
  while (1)
  {
    // printf("PosX:%f PosY:%f Alpha:%f  SpeedX:%f SpeedY:%f SpeedAlpha:%f SpeedA:%f SpeedB:%f SpeedC:%f \n"
    // ,RobotMove->getPositionX(),RobotMove->getPositionY(),RobotMove->getAlpha(),RobotMove->getSpeedX(),RobotMove->getSpeedY(),
    // RobotMove->getSpeedAlpha(),RobotMove->getSpeedA(),RobotMove->getSpeedB(),RobotMove->getSpeedC()
    // );

    printf("PosX:%f PosY:%f Alpha:%f  SpeedX:%f SpeedY:%f SpeedAlpha:%f PosA:%d PosB:%d PosC:%d \n"
    ,RobotMove->getPositionX(),RobotMove->getPositionY(),RobotMove->getAlpha(),RobotMove->getSpeedX(),RobotMove->getSpeedY(),
    RobotMove->getSpeedAlpha(),RobotMove->getPosA(),RobotMove->getPosB(),RobotMove->getPosC()
    );

    // printf("PosX:%f PosY:%f Alpha:%f  SpeedX:%f SpeedY:%f SpeedAlpha:%f StepA:%d StepB:%d StepC:%d SpeedA:%f SpeedB:%f SpeedC:%f\n"
    // ,RobotMove->getPositionX(),RobotMove->getPositionY(),RobotMove->getAlpha(),RobotMove->getSpeedX(),RobotMove->getSpeedY(),
    // RobotMove->getSpeedAlpha(),RobotMove->getStepA(),RobotMove->getStepB(),RobotMove->getStepC(),RobotMove->getSpeedA(),RobotMove->getSpeedB(),RobotMove->getSpeedC()
    // );

    //printf("%f;%f;%f\r\n",RobotMove->getPositionX(),RobotMove->getPositionY(),RobotMove->getAlpha());
 
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
  float LidarX =0;
  float LidarY =0;
  while (1)
  {
    NbDetecLidarPack = 0;
    NbNoDetecLidarPack =0;
    for (uint8_t j = 0; j < NB_LIDAR_PACK_READ; j++)
    {
      LidarPoints = LidarLD19->GetPoints();
      for (uint8_t i = 0; i < POINT_PER_PACK; i++)
      {
        //LidarPoints.point[i].intensity >100 
        if(sqrt(pow(float(RobotMove->getPosCibleX()-(RobotMove->getPositionX())),2.0)+pow(float(RobotMove->getPosCibleY()-(RobotMove->getPositionY())),2.0)) >1){
          //printf("%5.f;%5d\r\n",i,(LidarPoints.point[i].angle/100),LidarPoints.point[i].distance);

          //printf("%f;%f;%f;%f;%f\r\n",RobotMove->getPositionX(),RobotMove->getPositionY(),RobotMove->getAlpha(),PointLidarX,PointLidarY);
          //printf("%f;%f;%f;%f;%f\r\n",RobotMove->getPositionX(),RobotMove->getPositionY(),RobotMove->getAlpha(),LidarX,LidarY);

          //if(LidarPoints.point[i].intensity >200)printf("%f;%f;%f;%f;%f\r\n",RobotMove->getPositionX(),RobotMove->getPositionY(),RobotMove->getAlpha(),PointLidarX,PointLidarY);
          LidarX = RobotMove->getPositionX()+sin((PI/180)*(float(LidarPoints.point[i].angle/100)+RobotMove->getAlpha()))*LidarPoints.point[i].distance;
          LidarY = RobotMove->getPositionY()+cos((PI/180)*(float(LidarPoints.point[i].angle/100)+RobotMove->getAlpha()))*LidarPoints.point[i].distance;
          
        
          //if(sqrt(pow(float(RobotMove->getPosCibleX()-(RobotMove->getPositionX())),2.0)+pow(float(RobotMove->getPosCibleY()-(RobotMove->getPositionY())),2.0)) >10.0){
            AngleCible = ((180/PI) *atan2((RobotMove->getPosCibleX()-(RobotMove->getPositionX())),(RobotMove->getPosCibleY()-(RobotMove->getPositionY()))))-RobotMove->getAlpha();
          //}
          
         
          //printf("module=%f\n",sqrt(pow(float(RobotMove->getPosCibleX()-(RobotMove->getPositionX())),2.0)+pow(float(RobotMove->getPosCibleY()-(RobotMove->getPositionY())),2.0)));
       
          
         
          if(AngleCible<0) AngleCible =360+AngleCible;
          AngleCible_Down = AngleCible-LIDAR_ANGLE_MARGIN;
          AngleCible_Top  = AngleCible+LIDAR_ANGLE_MARGIN;
          if(AngleCible_Down<0) AngleCible_Down =360+AngleCible_Down;
          if(AngleCible_Top>360)  AngleCible_Top  =AngleCible_Top-360;
      
          if(LidarX>0 and LidarX<(3000-100) and LidarY>0 and LidarY<(2000-100)){
            if(AngleCible_Top > AngleCible_Down){
                if(float(LidarPoints.point[i].angle/100) <= AngleCible_Top and float(LidarPoints.point[i].angle/100) >= AngleCible_Down){
                  if(LidarPoints.point[i].distance > LIDAR_DIS_MIN and LidarPoints.point[i].distance <LIDAR_DIS_MAX){
                    NbDetecLidarPack+=50;
                    DistanceLidar = LidarPoints.point[i].distance ;
                    AngleLidar    = float(LidarPoints.point[i].angle/100);
                    PointLidarX   = LidarX;
                    PointLidarY   = LidarY;
                    
                    //printf("STOP1!=%d, SumStop=%d, distance=%d, AngleCible_Top=%f, Anglelidar=%f, AngleCible_Down=%f\n",Stop,SumStop,LidarPoints.point[i].distance,AngleCible_Top,float(LidarPoints.point[i].angle/100),AngleCible_Down);
                  }else{
                    NbNoDetecLidarPack++;
                  }
                }
            }else{
              if(float(LidarPoints.point[i].angle/100) <= AngleCible_Top or float(LidarPoints.point[i].angle/100) >= AngleCible_Down){
                  if(LidarPoints.point[i].distance > LIDAR_DIS_MIN and LidarPoints.point[i].distance <LIDAR_DIS_MAX){
                    NbDetecLidarPack+=50;
                    DistanceLidar = LidarPoints.point[i].distance ;
                    AngleLidar    = float(LidarPoints.point[i].angle/100);
                    PointLidarX   = LidarX;
                    PointLidarY   = LidarY;
                    
                  //printf("STOP2!=%d, SumStop=%d, distance=%d, AngleCible_Top=%f, Anglelidar=%f, AngleCible_Down=%f\n",Stop,SumStop,LidarPoints.point[i].distance,AngleCible_Top,float(LidarPoints.point[i].angle/100),AngleCible_Down);
                  }else{
                    NbNoDetecLidarPack++;
                  }
                }
            }
          }
          if((NbNoDetecLidarPack+NbNoDetecLidarPack) != 0){
            //printf("pourcentageON:=%f,pourcentageOFF=%f\n",float((NbDetecLidarPack*100)/(NbNoDetecLidarPack+NbDetecLidarPack)),float((NbNoDetecLidarPack*100)/(NbNoDetecLidarPack+NbDetecLidarPack)));
        
            if(float((NbDetecLidarPack*100)/(NbNoDetecLidarPack+NbDetecLidarPack))>LIDAR_PC_ON and Stop == 0){
              Stop = true;
            }
            if(float((NbNoDetecLidarPack*100)/(NbNoDetecLidarPack+NbDetecLidarPack))>LIDAR_PC_OFF and Stop == 1){
              Stop = false;
            }
          }
        }else{
          Stop = false;
        }
      }
    } 
    
    led_lidar =Stop;
    
  }
}



float theta2pluse(int theta)
{
  return 500.0+(100.0/9.0)*float(theta);
}

void print_serial(void)
{
  while(1){
    //printf("Stop:%d,  distance=%d, AngleCible_Top=%f, Anglelidar=%f, AngleCible_Down=%f\n",Stop,DistanceLidar,AngleCible_Top,AngleLidar,AngleCible_Down);

    printf("%d;%d;%d;%f;%f;%f;%f;%f;%f\r\n",Stop,int(RobotMove->getPositionX()),int(RobotMove->getPositionY()),RobotMove->getAlpha(),PointLidarX,PointLidarY,RobotMove->getPosCibleX(),RobotMove->getPosCibleY(),AngleCible);
  }
}

int LidargoesTo(int positionX, int positionY, int Alpha){
          if(Stop == 0){
            RobotMove->goesTo(positionX,positionY,Alpha);
            while(!RobotMove->waitAck());
            while(!RobotMove->stopped() and Stop == 0);
            if(RobotMove->PosCibleDone()){
              return 1;
            }else{
              return 0;
            }
            
          }else{
            RobotMove->stop();
            while(!RobotMove->waitAck());
            return 0;
          }   
}
void print_lcd(void){

  while (1)
  {
    if(!end_match){
       lcd.locate(0,1);
       lcd.printf("Time :%d\n",int(endMatch.remaining_time().count())/1000000);
       lcd.locate(7,0);
       lcd.printf("Score :%d\n",score);
    }
  }
}

void main_thread(void)
{
    FsmState = IDLE;
    lcd.printf("Wait Calibration\n");
    lcd.locate(0,1);
    Couleur_Team =!Switch_Team;
    if(Couleur_Team ==0){
      lcd.printf("Team Blue\n");
      offset_posX = 0;
      offset_Alpha =1;
    }else{
      lcd.printf("Team Yellow\n");
      offset_posX = 3000;
      offset_Alpha =-1;
    }

    RobotMove->stop();
    while(!RobotMove->waitAck());
    RobotMove->setPositionZero();
    while(!RobotMove->waitAck());
  
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
    RobotMove->setPosition(abs(offset_posX-225),225,0*offset_Alpha);
    while(!RobotMove->waitAck());

    

    // ServoB1P1.pulsewidth_us(theta2pluse(Pince[0].pos_open));
    // ServoB1P2.pulsewidth_us(theta2pluse(Pince[1].pos_open));

    // ServoB2P1.pulsewidth_us(theta2pluse(Pince[2].pos_open));
    // ServoB2P2.pulsewidth_us(theta2pluse(Pince[3].pos_open));

    // ServoB3P1.pulsewidth_us(theta2pluse(Pince[4].pos_close));
    // ServoB3P2.pulsewidth_us(theta2pluse(Pince[5].pos_close));
    
    
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

          // ServoB1P1.pulsewidth_us(theta2pluse(Pince[0].pos_close));
          // ServoB1P2.pulsewidth_us(theta2pluse(Pince[1].pos_close));

          // ServoB2P1.pulsewidth_us(theta2pluse(Pince[2].pos_close));
          // ServoB2P2.pulsewidth_us(theta2pluse(Pince[3].pos_close));

          // ServoB3P1.pulsewidth_us(theta2pluse(Pince[4].pos_close));
          // ServoB3P2.pulsewidth_us(theta2pluse(Pince[5].pos_close));

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
          RobotMove->move(0,0,-30*offset_Alpha);
          while(!RobotMove->waitAck());
          while(!RobotMove->stopped());

          RobotMove->move(-150*offset_Alpha,0,0*offset_Alpha);
          while(!RobotMove->waitAck());
          while(!RobotMove->stopped());

          RobotMove->move(0*offset_Alpha,-80,0*offset_Alpha);
          while(!RobotMove->waitAck());
          while(!RobotMove->stopped());

          RobotMove->move(-80*offset_Alpha,0,0*offset_Alpha);
          while(!RobotMove->waitAck());
          while(!RobotMove->stopped());

          RobotMove->move(0*offset_Alpha,52,0*offset_Alpha);
          while(!RobotMove->waitAck());
          while(!RobotMove->stopped());

          RobotMove->move(100*offset_Alpha,0,0*offset_Alpha);
          while(!RobotMove->waitAck());
          while(!RobotMove->stopped());

          RobotMove->move(0*offset_Alpha,0,30*offset_Alpha);
          while(!RobotMove->waitAck());
          while(!RobotMove->stopped()); 

          RobotMove->setPosition(abs(offset_posX-225),225,0*offset_Alpha);
          while(!RobotMove->waitAck());
 
          FsmState = WAIT_MATCH;
          lcd.cls();
          lcd.printf("Wait Match !\n");
          break;

        case WAIT_MATCH :
          if(Tirette != 1 and FsmState != END){
            endMatch.attach(endMatchProcess, 90s);
            lcd.cls();
            lcd.printf("GAME !\n");
            lcd_thread.start(print_lcd);
            FsmState = GAME; //Lancement du match !
          }
          break;

        case GAME :
              

              //while(!LidargoesTo(225,1000,0));
              // while(!LidargoesTo(225,225,0));
          
              RobotMove->goesTo(abs(offset_posX-320),270,0*offset_Alpha);
              while(!RobotMove->waitAck());
              while(!RobotMove->stopped());
              ServoB1P1.pulsewidth_us(theta2pluse(Pince[0].pos_close));
              ServoB1P2.pulsewidth_us(theta2pluse(Pince[1].pos_close));
              HAL_Delay (200); // Attente de 2 secondes 
              ServoB1.pulsewidth_us(theta2pluse(Bras[0].pos_down));
              HAL_Delay (500); // Attente de 2 secondes 
              ServoB1.pulsewidth_us(theta2pluse(Bras[0].pos_up));

              score += 5;

              RobotMove->goesTo(abs(offset_posX-545),265,0*offset_Alpha);
              while(!RobotMove->waitAck());
              while(!RobotMove->stopped());
              ServoB1.pulsewidth_us(theta2pluse(Bras[0].pos_down));
              HAL_Delay (500); // Attente de 2 secondes 
              ServoB1.pulsewidth_us(theta2pluse(Bras[0].pos_up));

              score += 5;

              RobotMove->goesTo(abs(offset_posX-770),260,0*offset_Alpha);
              while(!RobotMove->waitAck());
              while(!RobotMove->stopped());
              ServoB1.pulsewidth_us(theta2pluse(Bras[0].pos_down));
              HAL_Delay (500); // Attente de 2 secondes 
              ServoB1.pulsewidth_us(theta2pluse(Bras[0].pos_up));
              score += 5;

              ServoB1P1.pulsewidth_us(theta2pluse(Pince[0].pos_open));
              ServoB1P2.pulsewidth_us(theta2pluse(Pince[1].pos_open));
              
              RobotMove->goesTo(abs(offset_posX-650),680,0*offset_Alpha);
              while(!RobotMove->waitAck());
              while(!RobotMove->stopped());

              // // ***********************************/************************************
              // //                         Prise Plante 1 stock 1                       //
              // // ***********************************/************************************
              RobotMove->goesTo(abs(offset_posX-650),680,30*offset_Alpha);
              while(!RobotMove->waitAck());
              while(!RobotMove->stopped());
           
            if(Couleur_Team ==0){
              Turbine3.pulsewidth_us(1870);
              ServoB3.pulsewidth_us(theta2pluse(Bras[2].pos_down));
              RobotMove->goesTo(abs(offset_posX-780),680,30*offset_Alpha);
              while(!RobotMove->waitAck());
              while(!RobotMove->stopped());
              HAL_Delay (1000); // Attente de 2 secondes 
              ServoB3P1.pulsewidth_us(theta2pluse(Pince[4].pos_close));
              ServoB3P2.pulsewidth_us(theta2pluse(Pince[5].pos_close));
              HAL_Delay (500); // Attente de 2 secondes 
              ServoB3.pulsewidth_us(theta2pluse(Bras[2].pos_up));
              Turbine3.pulsewidth_us(1000);
            }else{
              Turbine2.pulsewidth_us(1870);
              ServoB2.pulsewidth_us(theta2pluse(Bras[1].pos_down));
              RobotMove->goesTo(abs(offset_posX-780),680,30*offset_Alpha);
              while(!RobotMove->waitAck());
              while(!RobotMove->stopped());
              HAL_Delay (1000); // Attente de 2 secondes 
              ServoB2P1.pulsewidth_us(theta2pluse(Pince[2].pos_close));
              ServoB2P2.pulsewidth_us(theta2pluse(Pince[3].pos_close));
              HAL_Delay (500); // Attente de 2 secondes 
              ServoB2.pulsewidth_us(theta2pluse(Bras[2].pos_up));
              Turbine2.pulsewidth_us(1000);
            }

              RobotMove->goesTo(abs(offset_posX-650),680,30*offset_Alpha);
              while(!RobotMove->waitAck());
              while(!RobotMove->stopped());

            // ***********************************/************************************
            //                         Prise Plante 2 stock 1                       //
            // ***********************************/************************************

              RobotMove->goesTo(abs(offset_posX-650),600,30*offset_Alpha);
              while(!RobotMove->waitAck());
              while(!RobotMove->stopped());

              RobotMove->goesTo(abs(offset_posX-650),600,150*offset_Alpha);
              while(!RobotMove->waitAck());
              while(!RobotMove->stopped());

            if(Couleur_Team ==0){
              Turbine2.pulsewidth_us(1820);
              ServoB2.pulsewidth_us(theta2pluse(Bras[1].pos_down));
              RobotMove->goesTo(abs(offset_posX-780),600,150*offset_Alpha);
              while(!RobotMove->waitAck());
              while(!RobotMove->stopped());
              HAL_Delay (1000); // Attente de 2 secondes 
              ServoB2P1.pulsewidth_us(theta2pluse(Pince[2].pos_close));
              ServoB2P2.pulsewidth_us(theta2pluse(Pince[3].pos_close));
              HAL_Delay (500); // Attente de 2 secondes 
              ServoB2.pulsewidth_us(theta2pluse(Bras[1].pos_up));
              Turbine2.pulsewidth_us(1000);
            }else{
              Turbine3.pulsewidth_us(1820);
              ServoB3.pulsewidth_us(theta2pluse(Bras[2].pos_down));
            
              RobotMove->goesTo(abs(offset_posX-780),600,150*offset_Alpha);
              while(!RobotMove->waitAck());
              while(!RobotMove->stopped());
              HAL_Delay (1000); // Attente de 2 secondes 
              ServoB3P1.pulsewidth_us(theta2pluse(Pince[4].pos_close));
              ServoB3P2.pulsewidth_us(theta2pluse(Pince[5].pos_close));
              HAL_Delay (500); // Attente de 2 secondes 
              ServoB3.pulsewidth_us(theta2pluse(Bras[2].pos_up));
              Turbine3.pulsewidth_us(1000);
            }

              RobotMove->goesTo(abs(offset_posX-650),600,150*offset_Alpha);
              while(!RobotMove->waitAck());
              while(!RobotMove->stopped());

              // //***********************************/************************************
              // //                         Prise Plante 3 stock 1                       //
              // //***********************************/************************************

              RobotMove->goesTo(abs(offset_posX-650),795,150*offset_Alpha);
              while(!RobotMove->waitAck());
              while(!RobotMove->stopped());

              RobotMove->goesTo(abs(offset_posX-650),795,270*offset_Alpha);
              while(!RobotMove->waitAck());
              while(!RobotMove->stopped());

              Turbine1.pulsewidth_us(1820);
              HAL_Delay (100); // Attente de 2 secondes 
              ServoB1.pulsewidth_us(theta2pluse(Bras[0].pos_down));
              RobotMove->goesTo(abs(offset_posX-780),795,270*offset_Alpha);
              while(!RobotMove->waitAck());
              while(!RobotMove->stopped());
              HAL_Delay (1000); // Attente de 2 secondes 
              ServoB1P1.pulsewidth_us(theta2pluse(Pince[0].pos_close));
              ServoB1P2.pulsewidth_us(theta2pluse(Pince[1].pos_close));
              HAL_Delay (500); // Attente de 2 secondes 
              ServoB1.pulsewidth_us(theta2pluse(Bras[0].pos_up));
              Turbine1.pulsewidth_us(1000);

            //   RobotMove->goesTo(abs(offset_posX-700),790,270*offset_Alpha);
            //   while(!RobotMove->waitAck());
            //   while(!RobotMove->stopped());
              

            //   // // ***********************************/************************************
            //   // //                         END Prise Plante  stock 1                    //
            //   // // ***********************************/************************************

            //   RobotMove->goesTo(abs(offset_posX-700),790,210*offset_Alpha);
            //   while(!RobotMove->waitAck());
            //   while(!RobotMove->stopped());

            //   RobotMove->goesTo(abs(offset_posX-132),600,210*offset_Alpha);
            //   while(!RobotMove->waitAck());
            //   while(!RobotMove->stopped());

            //   if(Couleur_Team ==0){
            //     ServoB3.pulsewidth_us(theta2pluse(Bras[2].pos_down));
            //     HAL_Delay (500); // Attente de 2 secondes 
            //     ServoB3P1.pulsewidth_us(theta2pluse(Pince[4].pos_open));
            //     ServoB3P2.pulsewidth_us(theta2pluse(Pince[5].pos_open));
            //     ServoB3.pulsewidth_us(theta2pluse(Bras[2].pos_up));
            //     HAL_Delay (500); // Attente de 2 secondes 
            //   }else{
            //     ServoB2.pulsewidth_us(theta2pluse(Bras[1].pos_down));
            //     HAL_Delay (500); // Attente de 2 secondes 
            //     ServoB2P1.pulsewidth_us(theta2pluse(Pince[2].pos_open));
            //     ServoB2P2.pulsewidth_us(theta2pluse(Pince[3].pos_open));
            //     ServoB2.pulsewidth_us(theta2pluse(Bras[1].pos_up));
            //     HAL_Delay (500); // Attente de 2 secondes 
            //   }

            //   score += 4;

            //   RobotMove->goesTo(abs(offset_posX-250),600,210*offset_Alpha);
            //   while(!RobotMove->waitAck());
            //   while(!RobotMove->stopped());

            //   RobotMove->goesTo(abs(offset_posX-250),600,90*offset_Alpha);
            //   while(!RobotMove->waitAck());
            //   while(!RobotMove->stopped());

            //   RobotMove->goesTo(abs(offset_posX-132),550,90*offset_Alpha);
            //   while(!RobotMove->waitAck());
            //   while(!RobotMove->stopped());

            //   ServoB1.pulsewidth_us(theta2pluse(Bras[0].pos_down));
            //   HAL_Delay (500); // Attente de 2 secondes 
            //   ServoB1P1.pulsewidth_us(theta2pluse(Pince[0].pos_open));
            //   ServoB1P2.pulsewidth_us(theta2pluse(Pince[1].pos_open));

            //   ServoB1.pulsewidth_us(theta2pluse(Bras[0].pos_up));
            //   HAL_Delay (500); // Attente de 2 secondes 

            //   score += 4;

            //   RobotMove->goesTo(abs(offset_posX-250),550,90*offset_Alpha);
            //   while(!RobotMove->waitAck());
            //   while(!RobotMove->stopped());

            //   RobotMove->goesTo(abs(offset_posX-250),550,-30*offset_Alpha);
            //   while(!RobotMove->waitAck());
            //   while(!RobotMove->stopped());

            //   RobotMove->goesTo(abs(offset_posX-132),650,-30*offset_Alpha);
            //   while(!RobotMove->waitAck());
            //   while(!RobotMove->stopped());

            //   if(Couleur_Team ==0){
            //     ServoB2.pulsewidth_us(theta2pluse(Bras[1].pos_down));
            //     HAL_Delay (500); // Attente de 2 secondes 
            //     ServoB2P1.pulsewidth_us(theta2pluse(Pince[2].pos_open));
            //     ServoB2P2.pulsewidth_us(theta2pluse(Pince[3].pos_open));
            //     ServoB2.pulsewidth_us(theta2pluse(Bras[1].pos_up));
            //     HAL_Delay (500); // Attente de 2 secondes 
            //   }else{
            //     ServoB3.pulsewidth_us(theta2pluse(Bras[2].pos_down));
            //     HAL_Delay (500); // Attente de 2 secondes 
            //     ServoB3P1.pulsewidth_us(theta2pluse(Pince[4].pos_open));
            //     ServoB3P2.pulsewidth_us(theta2pluse(Pince[5].pos_open));
            //     ServoB3.pulsewidth_us(theta2pluse(Bras[2].pos_up));
            //     HAL_Delay (500); // Attente de 2 secondes 
            //   }

            //   score += 4;


            //   RobotMove->goesTo(abs(offset_posX-225),225,-30*offset_Alpha);
            //   while(!RobotMove->waitAck());
            //   while(!RobotMove->stopped());

            //   score += 10;
              
            //   RobotMove->goesTo(abs(offset_posX-225),225,0*offset_Alpha);
            //   while(!RobotMove->waitAck());
            //   while(!RobotMove->stopped());

              

              // RobotMove->goesTo(200,1800,270);
              // while(!RobotMove->waitAck());
              // while(!RobotMove->stopped());

              // RobotMove->goesTo(200,1000,270);
              // while(!RobotMove->waitAck());
              // while(!RobotMove->stopped());

              // RobotMove->goesTo(200,1320,270);
              // while(!RobotMove->waitAck());
              // while(!RobotMove->stopped());

              // RobotMove->goesTo(250,1320,270);
              // while(!RobotMove->waitAck());
              // while(!RobotMove->stopped());

              // // ***********************************/************************************
              // //                         Depose Plante                                //
              // // ***********************************/************************************

              // RobotMove->goesTo(200,1320,210);
              // while(!RobotMove->waitAck());
              // while(!RobotMove->stopped());

              // RobotMove->goesTo(110,1320,210);
              // while(!RobotMove->waitAck());
              // while(!RobotMove->stopped());

              // ServoB3.pulsewidth_us(theta2pluse(Bras[2].pos_down));
              // HAL_Delay (500); // Attente de 2 secondes 
              // ServoB3P1.pulsewidth_us(theta2pluse(Pince[4].pos_open));
              // ServoB3P2.pulsewidth_us(theta2pluse(Pince[5].pos_open));
              // HAL_Delay (500); // Attente de 2 secondes 
              // ServoB3.pulsewidth_us(theta2pluse(Bras[2].pos_down+10));
              // HAL_Delay (200); // Attente de 2 secondes 
              // ServoB3.pulsewidth_us(theta2pluse(Bras[2].pos_down));
              // HAL_Delay (200); // Attente de 2 secondes 
              // ServoB3.pulsewidth_us(theta2pluse(Bras[2].pos_down+10));
              // HAL_Delay (200); // Attente de 2 secondes 
              // ServoB3.pulsewidth_us(theta2pluse(Bras[2].pos_down));
              // HAL_Delay (200); // Attente de 2 secondes 
              // ServoB3.pulsewidth_us(theta2pluse(Bras[2].pos_up));

              // RobotMove->goesTo(250,1400,210);
              // while(!RobotMove->waitAck());
              // while(!RobotMove->stopped());

              // RobotMove->goesTo(250,1400,90);
              // while(!RobotMove->waitAck());
              // while(!RobotMove->stopped());

              // RobotMove->goesTo(250,1400,90);
              // while(!RobotMove->waitAck());
              // while(!RobotMove->stopped());

              // RobotMove->goesTo(110,1400,90);
              // while(!RobotMove->waitAck());
              // while(!RobotMove->stopped());

              // ServoB1.pulsewidth_us(theta2pluse(Bras[0].pos_down));
              // HAL_Delay (500); // Attente de 2 secondes 
              // ServoB1P1.pulsewidth_us(theta2pluse(Pince[0].pos_open));
              // ServoB1P2.pulsewidth_us(theta2pluse(Pince[1].pos_open));
              // HAL_Delay (500); // Attente de 2 secondes 
              // ServoB1.pulsewidth_us(theta2pluse(Bras[0].pos_down+10));
              // HAL_Delay (200); // Attente de 2 secondes 
              // ServoB1.pulsewidth_us(theta2pluse(Bras[0].pos_down));
              // HAL_Delay (200); // Attente de 2 secondes 
              //  ServoB1.pulsewidth_us(theta2pluse(Bras[0].pos_down+10));
              // HAL_Delay (200); // Attente de 2 secondes 
              // ServoB1.pulsewidth_us(theta2pluse(Bras[0].pos_down));
              // HAL_Delay (200); // Attente de 2 secondes 
              // ServoB1.pulsewidth_us(theta2pluse(Bras[0].pos_up));

              // RobotMove->goesTo(250,1450,90);
              // while(!RobotMove->waitAck());
              // while(!RobotMove->stopped());

              // RobotMove->goesTo(250,1450,-30);
              // while(!RobotMove->waitAck());
              // while(!RobotMove->stopped());

              // RobotMove->goesTo(250,1450,-30);
              // while(!RobotMove->waitAck());
              // while(!RobotMove->stopped());

              // RobotMove->goesTo(110,1450,-30);
              // while(!RobotMove->waitAck());
              // while(!RobotMove->stopped());

              // ServoB2.pulsewidth_us(theta2pluse(Bras[1].pos_down));
              // HAL_Delay (500); // Attente de 2 secondes 
              // ServoB2P1.pulsewidth_us(theta2pluse(Pince[2].pos_open));
              // ServoB2P2.pulsewidth_us(theta2pluse(Pince[3].pos_open));
              // HAL_Delay (500); // Attente de 2 secondes 
              // ServoB2.pulsewidth_us(theta2pluse(Bras[1].pos_down+10));
              // HAL_Delay (200); // Attente de 2 secondes 
              // ServoB2.pulsewidth_us(theta2pluse(Bras[1].pos_down));
              // HAL_Delay (200); // Attente de 2 secondes 
              // ServoB2.pulsewidth_us(theta2pluse(Bras[1].pos_down+10));
              // HAL_Delay (200); // Attente de 2 secondes 
              // ServoB2.pulsewidth_us(theta2pluse(Bras[1].pos_down));
              // HAL_Delay (200); // Attente de 2 secondes 
              // ServoB2.pulsewidth_us(theta2pluse(Bras[1].pos_up));

              // RobotMove->goesTo(250,1450,-30);
              // while(!RobotMove->waitAck());
              // while(!RobotMove->stopped());

              // RobotMove->goesTo(225,225,-30);
              // while(!RobotMove->waitAck());
              // while(!RobotMove->stopped());

              // RobotMove->goesTo(700,1350,-90);
              // while(!RobotMove->waitAck());
              // while(!RobotMove->stopped());

              // RobotMove->goesTo(700,1350,-210);
              // while(!RobotMove->waitAck());
              // while(!RobotMove->stopped());

              // RobotMove->goesTo(700,1350,-330);
              // while(!RobotMove->waitAck());
              // while(!RobotMove->stopped());

              // RobotMove->goesTo(750,1750,-330);
              // while(!RobotMove->waitAck());
              // while(!RobotMove->stopped());

              // RobotMove->goesTo(750,1750,-300);
              // while(!RobotMove->waitAck());
              // while(!RobotMove->stopped());

              // RobotMove->goesTo(750,1800,-300);
              // while(!RobotMove->waitAck());
              // while(!RobotMove->stopped());

              // RobotMove->goesTo(750,1750,-300);
              // while(!RobotMove->waitAck());
              // while(!RobotMove->stopped());

              // RobotMove->goesTo(750,1750,-180);
              // while(!RobotMove->waitAck());
              // while(!RobotMove->stopped());

              // RobotMove->goesTo(750,1800,-180);
              // while(!RobotMove->waitAck());
              // while(!RobotMove->stopped());

              // RobotMove->goesTo(750,1750,-180);
              // while(!RobotMove->waitAck());
              // while(!RobotMove->stopped());

              // RobotMove->goesTo(750,1750,-60);
              // while(!RobotMove->waitAck());  
              // while(!RobotMove->stopped());

              // RobotMove->goesTo(750,1800,-60);
              // while(!RobotMove->waitAck());
              // while(!RobotMove->stopped());

              // RobotMove->goesTo(750,1750,-60);
              // while(!RobotMove->waitAck());
              // while(!RobotMove->stopped());

              // RobotMove->goesTo(225,1750,-60);
              // while(!RobotMove->waitAck());
              // while(!RobotMove->stopped());

              // RobotMove->goesTo(225,1750,0);
              // while(!RobotMove->waitAck());
              // while(!RobotMove->stopped());

              // RobotMove->goesTo(225,225,0);
              // while(!RobotMove->waitAck());
              // while(!RobotMove->stopped());

          lcd_thread.terminate();
          lcd.cls();
          lcd.printf("END !\n");
          lcd.locate(0,1);
          lcd.printf("Time :%d\n",int(endMatch.remaining_time().count())/1000000);
          lcd.locate(7,0);
          lcd.printf("Score :%d\n",score);
          FsmState = END;
          break;

        case END :
           
          break;
      }
    }
}
int main()
{ 
    Button_init.mode(PullUp);
    Tirette.mode(PullUp);
    Switch_Team.mode(PullUp);
    Button_init_gnd = 0;
    
    //serial_thread.start(Xbox_read);
    //serial_thread.start(BluetoothCmd);
    //show_pos_thread.start(showPostion);
    //show_pos_thread.start(showLidar);
    serial_thread.start(print_serial);
    show_pos_thread.start(ShowLidarCoord);
    game_thread.start(main_thread);
    
 
    
    while (1)
    {
     if(end_match){
      lcd_thread.terminate();
      game_thread.terminate();
      RobotMove->stop();
      while(!RobotMove->waitAck());
      lcd.cls();
      lcd.printf("END TIMOUT !\n");
      lcd.locate(0,1);
      lcd.printf("Score :%d\n",score);
     }
    }
}