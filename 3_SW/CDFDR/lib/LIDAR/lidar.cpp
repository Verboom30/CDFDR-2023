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
void Lidar::ReadLidar(void)
{
  uint8_t buffer[DATA_PACKET_SIZE*2] = {0};
  uint8_t DataPacket[DATA_PACKET_SIZE] = {0};
  if (readable()) {
    if (uint32_t num = read(buffer, sizeof(buffer))) {

      // printf("\nbuffer_hex: ");
      // for (uint32_t i = 0; i < num; i++)
      // {
      //     printf("%x",buffer[i]);
      // }
      uint8_t index;
      for (index = 0; index < DATA_PACKET_SIZE-1 && (buffer[index] != HEADER || buffer[index+1] != VERLEN); index++);
      printf("\nDATA_PACKET: ");
      for (uint8_t i = index; i < DATA_PACKET_SIZE+index; i++)
      {
        printf("%02X",buffer[i]);
        DataPacket[i] = buffer[i];
      }
      _dataPacket.header                = DataPacket[0];
      _dataPacket.ver_len               = DataPacket[1];
      _dataPacket.speed                 = DataPacket[2] & DataPacket[3];
      _dataPacket.start_angle           = DataPacket[4] & DataPacket[5];

      for (uint8_t i = 0; i < POINT_PER_PACK; i++)
      {
        _dataPacket.point[i].distance   = DataPacket[6+3*i] & DataPacket[7+3*i];
        _dataPacket.point[i].intensity  = DataPacket[8+3*i];
      }

      _dataPacket.end_angle             = DataPacket[42] & DataPacket[43];
      _dataPacket.timestamp             = DataPacket[44] & DataPacket[45];
      _dataPacket.crc8                  = DataPacket[46];
    }
  }
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
    while(1){
      ReadLidar();
    }
}