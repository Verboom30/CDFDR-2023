#include "lidar.hpp"
//***********************************/************************************
//                         Constructors                                 //
//***********************************/************************************

Lidar::Lidar(PinName tx, PinName rx, int baud) : BufferedSerial(tx, rx, baud)
{
    Lidar_thread.start(callback(this, &Lidar::routine_lidar));
    LiDARFrameTypeDef _dataPacket;
}

//***********************************/************************************
//                                Get Set                               //
//***********************************/************************************
void Lidar::ReadLidar(char** DataPacket)
{
   
}

void on_rx_interrupt(void)
{
    
}


//***********************************/************************************
//                             Public Methods                           //
//***********************************/************************************


//***********************************/************************************
//                          Protected Methods                           //
//***********************************/************************************

void Lidar::routine_lidar(void)
{
    char buffer[DATA_PACKET_SIZE*2] = {0};
    //BufferedSerial pc(USBTX, USBRX,230400);
    //const char * separators = (const char *)(HEADER);
    while(1){
      
        if (readable()) {
       
      
            //int dataPacket = 0;
            if (uint32_t num = read(buffer, sizeof(buffer))) {
                //*DataPacket = "TEST";//strtok(buffer, separators);
                //pc.write(buffer,num);
                printf("%s\n",buffer);
                //printf("DataLidar: %x\n",strtok(buffer, separators));
            
                //sscanf(strToken,"LeftHatY: %x",&dataPacket);
                //return dataPacket;
                }
        }else{
            sync();
            //printf("LIDAR\n");
           
        }
    }
}