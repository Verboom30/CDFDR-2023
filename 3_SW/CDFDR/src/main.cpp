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
Lidar*    LidarLD19 = new Lidar(D1, D0,230400);

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





// void print(const std::string &str) {
//     pc.write(str.c_str(), str.length());
// }

// void Xbox_read(void)
// {
//   const char * separators = "\t\n";
//   char buffer[MAXIMUM_BUFFER_SIZE] = {0};
//   int LeftHY=0;
//   int LeftHX=0;
//   int RightHY=0;
//   int RightHX=0;
//   int LT=0;
//   int RT=0;
//   while (1) {
//     if (uint32_t num = uart.read(buffer, sizeof(buffer))) {
//       uart_activity = !uart_activity;
//       //pc.write(buffer,num);
      
//       //printf("%s",buffer);

//       // char * strToken = strtok ( buffer, separators );
//       // while ( strToken != NULL ) {
//       //   sscanf(strToken,"LeftHatY: %d",&LeftHY); 
//       //   sscanf(strToken,"LeftHatX: %d",&LeftHX); 
//       //   sscanf(strToken,"RightHatY: %d",&RightHY); 
//       //   sscanf(strToken,"RightHatX: %d",&RightHX); 
//       //   sscanf(strToken,"LT: %d",&LT); 
//       //   sscanf(strToken,"RT: %d",&RT); 

//       //   printf("LeftHX ==> %d ",LeftHX);
//       //   printf("LeftHY ==> %d ",LeftHY);
//       //   printf("RightHX ==> %d ",RightHX);
//       //   printf("RightHY ==> %d ",RightHY);
//       //   printf("LT ==> %d ",LT);
//       //   printf("RT ==> %d ",RT);
//       //   printf("\n");

//       //   if(strstr(buffer,"A")){
           
//       //   }
//       //   if(strstr(buffer,"B")){
           
//       //   }
//       //   if(strstr(buffer,"X")){
           
//       //   }
//       //   if(strstr(buffer,"Y")){
           
//       //   }
//       //   if(strstr(buffer,"Up")){
//       //       ServoB1.pulsewidth_us(500.0+(100.0/9.0)*180);
//       //   }
//       //   if(strstr(buffer,"Down")){
//       //       ServoB1.pulsewidth_us(500.0+(100.0/9.0)*120);
//       //   }
//       //   if(strstr(buffer,"Left")){
            
//       //   }
//       //   if(strstr(buffer,"Right")){
            
//       //   }
//       //   if(strstr(buffer,"View")){
            
//       //   }
//       //   if(strstr(buffer,"Menu")){
            
//       //   }
//       //   if(strstr(buffer,"Xbox")){
            
//       //   }
//       //   if(strstr(buffer,"LB")){
            
//       //   }
//       //   if(strstr(buffer,"RB")){
            
//       //   }
//       //   if(strstr(buffer,"L3")){
            
//       //   }
//       //    if(strstr(buffer,"R3")){
            
//       //   }
      
//       //   // On demande le token suivant.
//       //   strToken = strtok ( NULL, separators );
//       // }   
//       // for (int i = 0; i < MAXIMUM_BUFFER_SIZE; i++)
//       // {
//       //     buffer[i] = 0; // on vide le buffer
//       // }
//     }   
    
//   }  
    
// }


// void traitement_cmdblth( char *msg, int size)
// {
//   std::string cpy;
//   std::string cmd;
//   std::string PX;
//   std::string PY;
//   std::string Alpha;
//   cpy = msg;

//   // uart.write(GREEN, sizeof(GREEN));
//   // uart.write(msg, size);
//   // uart.write(WHITE, sizeof(WHITE));


//   cmd = cpy.substr(0,cpy.find(":"));
//   cpy.erase(0, cpy.find(": ")+2);
//   // uart.write(RED, sizeof(RED));
//   // uart.write(cmd.c_str(), cmd.length());
//   // uart.write(WHITE, sizeof(WHITE));
  
//   if(RobotMove->stopped()){
//     if(cmd =="GOTO" or cmd =="MOVE" ){
      
//       PX = cpy.substr(0,cpy.find(" "));
//       cpy.erase(0, cpy.find(" ")+1);
//       PY = cpy.substr(0,cpy.find(" "));
//       cpy.erase(0, cpy.find(" ")+1);
//       Alpha = cpy.substr(0,string::npos);
//       cpy.erase(0, string::npos);
      
//       uart.write(GREEN, sizeof(GREEN));
//       uart.write("PX:",sizeof("PX:"));
//       uart.write(PX.c_str(),PX.length());
//       uart.write(" PY:",sizeof(" PY:"));
//       uart.write(PY.c_str(),PY.length());
//       uart.write(" Alpha:",sizeof(" Alpha:"));
//       uart.write(Alpha.c_str(),Alpha.length());
//       uart.write(WHITE, sizeof(WHITE));
//       if(cmd =="GOTO"){
//         RobotMove->goesTo(stoi(PX),stoi(PY),stoi(Alpha));
//         while(!RobotMove->waitAck());
//       }else if (cmd =="MOVE")
//       {
//         RobotMove->move(stoi(PX),stoi(PY),stoi(Alpha));
//         while(!RobotMove->waitAck());
//       }
      
      
//       //while(!RobotMove->stopped()); 


        
//       /* code */
//     }else if (cmd =="STOP")
//     {
//       /* code */
//     }else{
//       uart.write(RED, sizeof(RED));
//       char msg_error[] = "\nCommande invalide\n";
//       uart.write(msg_error, sizeof(msg_error));
//       uart.write(WHITE, sizeof(WHITE));
//     }
//   }else{
//     uart.write(RED, sizeof(RED));
//     char msg_error[] = "\nCommande annulée Robot toujours en mouvement\n";
//     uart.write(msg_error, sizeof(msg_error));
//     uart.write(WHITE, sizeof(WHITE));
//   }
  
  



//   // const char * separators = ": \n";
//   // char * strToken = strtok ( msg, separators);

//   // while ( strToken != NULL ) {
//   //       uart.write(RED, sizeof(RED));
//   //       uart.write(strToken, sizeof(strToken));
//   //       uart.write(WHITE, sizeof(WHITE));
//   //       strToken = strtok ( NULL, separators );
//   //     } 
  
 


  

  



  

// }
// void BluetoothCmd(void)
// {
//     char msg[] = "\rConnexion bluetooth disponible\r\n";
//     char buffer[MAXIMUM_BUFFER_SIZE] = {0};
//     char CmdBlth[MAXIMUM_BUFFER_SIZE*2] = {0};
//     uart.write(msg, sizeof(msg));
//     uint32_t snum =0;
//     bool flag = false;
//     while (1) {
//       if (uart.readable()) {
//         if (uint32_t num = uart.read(buffer, MAXIMUM_BUFFER_SIZE)) {
          
//           //printf("snum:%d, num:%d, buffer:%s\n",snum,num, buffer);
//           for (int i = 0; i < num; i++)
//           {
//               CmdBlth[snum+i] = buffer[i]; // on vide le buffer}
//               //printf("sumbuffer[%d]:%c buffer[%d] :%c\n",(snum+i),sumbuffer[snum+i],i,buffer[i]);
//               if(buffer[i] =='\n') flag =true;
//           }

//           for (int i = 0; i < MAXIMUM_BUFFER_SIZE; i++)
//           {
//               buffer[i] = 0; // on vide le buffer

//           }
//           snum =snum+num;
//         }
//       }else{
//         uart.sync();
//         if(flag ==true){
//           //printf("sumbuffer :%s\n",sumbuffer);
//           uart.write(YELLOW, sizeof(YELLOW));
//           uart.write(CmdBlth, sizeof(CmdBlth));
//           uart.write(CYAN, sizeof(CYAN));
//           uart.write("ACK_BLTH\n", 10);
//           uart.write(WHITE, sizeof(WHITE));
//           traitement_cmdblth(CmdBlth,sizeof(CmdBlth));
//           for (int i = 0; i < MAXIMUM_BUFFER_SIZE*2; i++)
//           {
//               CmdBlth[i] = 0; // on vide le buffer
//           }
//           flag =false;
//           snum=0;
//         }
//       }
//     }
// }
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
  }
  
}

void showLidar(void)
{
  char* msg = {0};
  while (1)
  {
    LidarLD19->ReadLidar(&msg);
    printf("DataLidar: %s\n",msg);
  }
  
}

float theta2pluse(int theta)
{
  return 500.0+(100.0/9.0)*float(theta);
}


int main()
{ 
    Button_init.mode(PullUp);
    Button_init_gnd = 0;
    FsmState = IDLE;
    lcd.printf("Wait Calibration\n");
    
    //serial_thread.start(Xbox_read);
    //serial_thread.start(BluetoothCmd);
    //show_pos_thread.start(showPostion);
    //show_pos_thread.start(showLidar);

    RobotMove->stop();
    RobotMove->setPositionZero();
    

    ServoB1.period_ms(20);
    ServoB2.period_ms(20);
    ServoB3.period_ms(20);
    // Turbine1.period_ms(20);
    // Turbine2.period_ms(20);
    // Turbine3.period_ms(20);
    ServoB1P1.period_ms(20);
    ServoB1P2.period_ms(20);
    ServoB2P1.period_ms(20);
    ServoB2P2.period_ms(20);
    ServoB3P1.period_ms(20);
    ServoB3P2.period_ms(20);
    // Turbine1.pulsewidth_us(1000);
    // Turbine2.pulsewidth_us(1000);
    // Turbine3.pulsewidth_us(1000);
    // ServoB1.pulsewidth_us(theta2pluse(Bras[0].pos_up));
    // ServoB2.pulsewidth_us(theta2pluse(Bras[1].pos_up));
    // ServoB3.pulsewidth_us(theta2pluse(Bras[2].pos_up));

    // ServoB1P1.pulsewidth_us(theta2pluse(Pince[0].pos_open));
    // ServoB1P2.pulsewidth_us(theta2pluse(Pince[1].pos_open));
   

  
    // RobotMove->goesTo(300,0,0);
    //  while(!RobotMove->waitAck());
    //  while(!RobotMove->stopped()); 
    // RobotMove->move(0,100,0);
    // while(!RobotMove->waitAck());
    // while(!RobotMove->stopped()); 

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
          HAL_Delay (3000); // Attente de 2 secondes 
          lcd.cls();
          lcd.printf("Calibration !\n");
          FsmState = CAL;
          break;
            
        
        case CAL :
          RobotMove->goesTo(300,0,0);
          while(!RobotMove->waitAck());
          while(!RobotMove->stopped()); 
          RobotMove->goesTo(0,0,0);
          while(!RobotMove->waitAck());
          while(!RobotMove->stopped()); 
          RobotMove->goesTo(0,-300,0);
          while(!RobotMove->waitAck());
          while(!RobotMove->stopped()); 
          RobotMove->goesTo(0,0,0);
          while(!RobotMove->waitAck());
          while(!RobotMove->stopped()); 
          FsmState = WAIT_MATCH;
          lcd.cls();
          lcd.printf("Wait Match !\n");
          break;

        case WAIT_MATCH :
          break;

        case GAME :
          break;

        case END :
          break;
      }
     
    
     
     
    

      // ServoB1P1.pulsewidth_us(theta2pluse(Pince[0].pos_open));
      // ServoB1P2.pulsewidth_us(theta2pluse(Pince[1].pos_open));
      // wait_us(5000000);
      // ServoB1P1.pulsewidth_us(theta2pluse(Pince[0].pos_close));
      // ServoB1P2.pulsewidth_us(theta2pluse(Pince[1].pos_close));
      // wait_us(2000000);
      // ServoB1.pulsewidth_us(theta2pluse(Bras[0].pos_down));
      // wait_us(2000000);
      // ServoB1P1.pulsewidth_us(theta2pluse(Pince[0].pos_open));
      // ServoB1P2.pulsewidth_us(theta2pluse(Pince[1].pos_open));
      // wait_us(2000000);
      // ServoB1.pulsewidth_us(theta2pluse(Bras[0].pos_up));

      // Turbine1.pulsewidth_us(1000);
      // wait_us(5000000);
      // Turbine1.pulsewidth_us(1550);
      // wait_us(5000000);

      
      
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

      

       
    //  RobotMove->move(300,300,0);
    //  while(!RobotMove->waitAck());
    //  while(!RobotMove->stopped()); 
        
   
    // RobotMove->goesTo(0,0,0);
    // while(!RobotMove->waitAck());
    // while(!RobotMove->stopped()); 
    
    


    
  

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