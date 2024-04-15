#include "lidar.hpp"
#include "main_pkg.hpp"
//***********************************/************************************
//                         Constructors                                 //
//***********************************/************************************

Lidar::Lidar(PinName tx, PinName rx, int baud) : BufferedSerial(tx, rx, baud)
{
    //Lidar_thread.start(callback(this, &Lidar::routine_lidar));
    LiDARFrameTypeDef _dataPacket;
}

//***********************************/************************************
//                                Get Set                               //
//***********************************/************************************
void Lidar::ReadLidar(void)
{
  uint8_t  buffer[DATA_PACKET_SIZE*2] = {0};
  uint8_t  DataPacket[DATA_PACKET_SIZE] = {0};
  float    step = 0;
  uint8_t  crc =0;
  if (readable()) {
    if (uint32_t num = read(buffer, sizeof(buffer))) {

      // printf("\nbuffer_hex: ");
      // for (uint32_t i = 0; i < num; i++)
      // {
      //     printf("%x",buffer[i]);
      // }
      uint8_t index;
      for (index = 0; index < DATA_PACKET_SIZE-1 && (buffer[index] != HEADER || buffer[index+1] != VERLEN); index++);
      //printf("\nDATA_PACKET: ");
      for (uint8_t i = index; i < DATA_PACKET_SIZE+index; i++)
      {
        DataPacket[i-index] = buffer[i];
        //printf("%02X",buffer[i]);
      }
      //printf("\r\n");
    
      _dataPacket.header                = DataPacket[0];
      _dataPacket.ver_len               = DataPacket[1];
      _dataPacket.speed                 = ((uint16_t)DataPacket[3] << 8) | DataPacket[2];
      _dataPacket.start_angle           = ((uint16_t)DataPacket[5] << 8) | DataPacket[4];

      _dataPacket.end_angle             = ((uint16_t)DataPacket[43] << 8) | DataPacket[42];
      _dataPacket.timestamp             = ((uint16_t)DataPacket[45] << 8) | DataPacket[44];

      _dataPacket.crc8                  = DataPacket[46];

      _dataPacket.start_angle           = _dataPacket.start_angle % 36000; //modulo 360°
      _dataPacket.end_angle             = _dataPacket.end_angle   % 36000; //modulo 360°
     
  
    
      if(_dataPacket.end_angle > _dataPacket.start_angle){
        step = float(abs(_dataPacket.end_angle - _dataPacket.start_angle))/float(POINT_PER_PACK-1);
      }else{
         step = float(abs((36000 -_dataPacket.end_angle) - _dataPacket.start_angle))/float(POINT_PER_PACK-1);
      }
      //printf("end_angle=%5d start_angle=%5d \r\n",((uint16_t)DataPacket[43] << 8) | DataPacket[42],((uint16_t)DataPacket[5] << 8) | DataPacket[4]);
      //printf("Step=%f modulo_end_angle=%5d modulo_start_angle=%5d \r\n",step/100,(_dataPacket.end_angle/100),(_dataPacket.start_angle/100));
      
   
      if(step <=100){ //filtrage point
        for (uint8_t i = 0; i < POINT_PER_PACK; i++)
        {
          _dataPacket.point[i].distance   = ((uint16_t)DataPacket[7+3*i] << 8) | DataPacket[6+3*i];
          _dataPacket.point[i].intensity  = DataPacket[8+3*i];

          _dataPacket.point[i].angle = _dataPacket.start_angle + float(step*i);
          _dataPacket.point[i].angle = int(_dataPacket.point[i].angle) % 36000;

          //printf("Step=%f Angle=%f\n",step/100,_dataPacket.point[i].angle/100);
          //printf("EndAngle=%5d StartAngle=%5d Step[%2d]= %4.2f angle[%2d]=%4.2f dis[%2d]=%d \n",_dataPacket.end_angle,_dataPacket.start_angle,i,step,i,_dataPacket.point[i].angle,i,_dataPacket.point[i].distance);
          //printf("Angle[%2d]=%4.2f dis[%2d]=%d\n",i,(_dataPacket.point[i].angle/100),i,_dataPacket.point[i].distance);
        }
      }
      //printf("\n");
      
    }
  }
}


void Lidar::ShowDisAndIntsy(void)
{
  for (uint8_t i = 0; i < POINT_PER_PACK; i++)
  {
    //printf("[%2d] Dis=%4d Intsy=%4d Agl=%3.2f\n",i,_dataPacket.point[i].distance,_dataPacket.point[i].intensity,_dataPacket.point[i].angle);
    printf("%f;%5d\r\n",(float(_dataPacket.point[i].angle/100)),_dataPacket.point[i].distance);
  }
  
  
}


//***********************************/************************************
//                             Public Methods                           //
//***********************************/************************************
LiDARFrameTypeDef Lidar::GetPoints(void)
{
  ReadLidar();
  return _dataPacket;
}

//***********************************/************************************
//                          Protected Methods                           //
//***********************************/************************************


// void Lidar::routine_lidar(void)
// {
//     while(1){
//       ReadLidar();
//       ShowDisAndIntsy();
//     }
// }