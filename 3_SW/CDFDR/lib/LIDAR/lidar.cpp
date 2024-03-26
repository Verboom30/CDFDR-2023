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


  uint32_t charToUInt32(const char* src) {
    uint32_t ret = 0;
    char* dst = (char*)&ret;
    for(unsigned i = 0; (i < sizeof(src)) && (*src); ++i, ++src)
      dst[i] = *src;

    return ret;
  }


void Lidar::routine_lidar(void)
{
    char buffer[DATA_PACKET_SIZE*2] = {0};
    char buffer_hex[DATA_PACKET_SIZE*2] = {0};
    //BufferedSerial pc(USBTX, USBRX,230400);
    while(1){
      
        if (readable()) {
          //int dataPacket = 0;
          if (uint32_t num = read(buffer, sizeof(buffer))) {

            printf("\nbuffer_hex: ");
            for (uint32_t i = 0; i < num; i++)
            {
                memcpy(&buffer_hex[i], buffer+i, 1);
                printf("%x",buffer_hex[i]);
            }
          }
          
        }else{
          sync();
            
            //printf("LIDAR\n");
           
        }
    }
}