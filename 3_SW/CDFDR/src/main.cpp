#include "mbed.h"
#include "pinout.hpp"
#include "develop_ticker.hpp"
#include "Holonome.hpp"
#include <string.h>
#include <math.h>
#define MAXIMUM_BUFFER_SIZE  32


BufferedSerial pc(USBTX, USBRX,115200);
BufferedSerial uart(D1, D0,9600);

Holonome* RobotMove = new Holonome();

DigitalOut pc_activity(LED1);
DigitalOut uart_activity(LED2);


AnalogIn   Poten(A0);
Thread serial_thread;
Thread plot_thread;
Thread show_pos_thread;

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

void Xbox_read(void)
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


void traitement_cmdblth( char *msg, int size)
{
  std::string cpy;
  std::string cmd;
  std::string PX;
  std::string PY;
  std::string Alpha;
  cpy = msg;

  // uart.write(GREEN, sizeof(GREEN));
  // uart.write(msg, size);
  // uart.write(WHITE, sizeof(WHITE));


  cmd = cpy.substr(0,cpy.find(":"));
  cpy.erase(0, cpy.find(": ")+2);
  // uart.write(RED, sizeof(RED));
  // uart.write(cmd.c_str(), cmd.length());
  // uart.write(WHITE, sizeof(WHITE));
  
  if(RobotMove->stopped()){
    if(cmd =="GOTO" or cmd =="MOVE" ){
      
      PX = cpy.substr(0,cpy.find(" "));
      cpy.erase(0, cpy.find(" ")+1);
      PY = cpy.substr(0,cpy.find(" "));
      cpy.erase(0, cpy.find(" ")+1);
      Alpha = cpy.substr(0,string::npos);
      cpy.erase(0, string::npos);
      
      uart.write(GREEN, sizeof(GREEN));
      uart.write("PX:",sizeof("PX:"));
      uart.write(PX.c_str(),PX.length());
      uart.write(" PY:",sizeof(" PY:"));
      uart.write(PY.c_str(),PY.length());
      uart.write(" Alpha:",sizeof(" Alpha:"));
      uart.write(Alpha.c_str(),Alpha.length());
      uart.write(WHITE, sizeof(WHITE));

      RobotMove->goesTo(stoi(PX),stoi(PY),stoi(Alpha));
      while(!RobotMove->waitAck());
      //while(!RobotMove->stopped()); 


        
      /* code */
    }else if (cmd =="STOP")
    {
      /* code */
    }else{
      uart.write(RED, sizeof(RED));
      char msg_error[] = "\nCommande invalide\n";
      uart.write(msg_error, sizeof(msg_error));
      uart.write(WHITE, sizeof(WHITE));
    }
  }else{
    uart.write(RED, sizeof(RED));
    char msg_error[] = "\nCommande annulée Robot toujours en mouvement\n";
    uart.write(msg_error, sizeof(msg_error));
    uart.write(WHITE, sizeof(WHITE));
  }
  
  



  // const char * separators = ": \n";
  // char * strToken = strtok ( msg, separators);

  // while ( strToken != NULL ) {
  //       uart.write(RED, sizeof(RED));
  //       uart.write(strToken, sizeof(strToken));
  //       uart.write(WHITE, sizeof(WHITE));
  //       strToken = strtok ( NULL, separators );
  //     } 
  
 


  

  



  

}
void BluetoothCmd(void)
{
    char msg[] = "\rConnexion bluetooth disponible\r\n";
    char buffer[MAXIMUM_BUFFER_SIZE] = {0};
    char CmdBlth[MAXIMUM_BUFFER_SIZE*2] = {0};
    uart.write(msg, sizeof(msg));
    uint32_t snum =0;
    bool flag = false;
    while (1) {
      if (uart.readable()) {
        if (uint32_t num = uart.read(buffer, MAXIMUM_BUFFER_SIZE)) {
          
          //printf("snum:%d, num:%d, buffer:%s\n",snum,num, buffer);
          for (int i = 0; i < num; i++)
          {
              CmdBlth[snum+i] = buffer[i]; // on vide le buffer}
              //printf("sumbuffer[%d]:%c buffer[%d] :%c\n",(snum+i),sumbuffer[snum+i],i,buffer[i]);
              if(buffer[i] =='\n') flag =true;
          }

          for (int i = 0; i < MAXIMUM_BUFFER_SIZE; i++)
          {
              buffer[i] = 0; // on vide le buffer

          }
          snum =snum+num;
        }
      }else{
        uart.sync();
        if(flag ==true){
          //printf("sumbuffer :%s\n",sumbuffer);
          uart.write(YELLOW, sizeof(YELLOW));
          uart.write(CmdBlth, sizeof(CmdBlth));
          uart.write(CYAN, sizeof(CYAN));
          uart.write("ACK_BLTH\n", 10);
          uart.write(WHITE, sizeof(WHITE));
          traitement_cmdblth(CmdBlth,sizeof(CmdBlth));
          for (int i = 0; i < MAXIMUM_BUFFER_SIZE*2; i++)
          {
              CmdBlth[i] = 0; // on vide le buffer
          }
          flag =false;
          snum=0;
        }
      }
    }
}
void showPostion(void)
{
  while (1)
  {
    printf("PosX:%f PosY:%f Alpha:%f  SpeedX:%f SpeedY:%f SpeedAlpha:%f SpeedA:%f SpeedB:%f SpeedC:%f SpeedD:%f \n"
    ,RobotMove->getPositionX(),RobotMove->getPositionY(),RobotMove->getAlpha(),RobotMove->getSpeedX(),RobotMove->getSpeedY(),
    RobotMove->getSpeedAlpha(),RobotMove->getSpeedA(),RobotMove->getSpeedB(),RobotMove->getSpeedC(),RobotMove->getSpeedB());
  }
  
}

int main()
{ 


    //serial_thread.start(Xbox_read);
    serial_thread.start(BluetoothCmd);
    //plot_thread.start(show_thread);
    show_pos_thread.start(showPostion);
  
   
    RobotMove->stop();
    RobotMove->setPositionZero();

  
    // RobotMove->goesTo(300,0,0);
    //  while(!RobotMove->waitAck());
    //  while(!RobotMove->stopped()); 
    RobotMove->move(0,100,45);
    while(!RobotMove->waitAck());
    while(!RobotMove->stopped()); 

        
  
  
    while (1)
    {

         
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