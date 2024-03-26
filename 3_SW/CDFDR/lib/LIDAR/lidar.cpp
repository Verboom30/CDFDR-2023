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


  // uint32_t charToUInt32(const char* src) {
  //   uint32_t ret = 0;
  //   char* dst = (char*)&ret;
  //   for(unsigned i = 0; (i < sizeof(src)) && (*src); ++i, ++src)
  //     dst[i] = *src;

  //   return ret;
  // }


void Lidar::routine_lidar(void)
{
    uint8_t buffer[DATA_PACKET_SIZE*2] = {0};
    while(1){
        if (readable()) {
          if (uint32_t num = read(buffer, sizeof(buffer))) {

            // printf("\nbuffer_hex: ");
            // for (uint32_t i = 0; i < num; i++)
            // {
            //     printf("%x",buffer[i]);
            // }
            uint8_t index;
            for (index = 0; index < DATA_PACKET_SIZE && buffer[index] != HEADER; index++);
            printf("\nDATA_PACKET: ");
            for (uint8_t i = index; i < DATA_PACKET_SIZE+index; i++)
            {
               printf("%02X",buffer[i]);
            }
            
          
            



          
          }
          
        }else{
          sync();
            
            //printf("LIDAR\n");
           
        }
    }
}