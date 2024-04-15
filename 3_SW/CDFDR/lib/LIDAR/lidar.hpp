#ifndef LIDAR_H
#define LIDAR_H

#include "mbed.h"
#include <string.h>

#define POINT_PER_PACK 12
#define HEADER 0x54
#define VERLEN 0x2C
#define DATA_PACKET_SIZE 47

struct   LidarPointStructDef{

    uint16_t distance;
    uint8_t  intensity;
    float    angle;

};

struct  LiDARFrameTypeDef{

    uint8_t             header;
    uint8_t             ver_len;
    uint16_t            speed;
    uint16_t            start_angle;
    LidarPointStructDef point[POINT_PER_PACK];
    uint16_t            end_angle;
    uint16_t            timestamp;
    uint8_t             crc8;

};

class Lidar : public BufferedSerial
{
    public:
        Lidar(PinName tx, PinName rx, int baud = MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE);
        void ReadLidar(void);
        void ShowDisAndIntsy(void);
        LiDARFrameTypeDef GetPoints(void);
        LiDARFrameTypeDef _dataPacket;
        
    private : 
       
        

        Thread Lidar_thread;
        //void routine_lidar(void);
        
        
};
#endif