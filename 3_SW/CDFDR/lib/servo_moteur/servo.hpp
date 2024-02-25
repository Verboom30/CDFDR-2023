#ifndef SERVO_H
#define SERVO_H

struct pos_bras {
    int pos_up;
    int pos_down;
};

struct pos_servo {
    int pos_open;
    int pos_close;
};

const struct pos_bras Bras[]{
    {180, 120}, //Bras 1
    {180, 150}, //Bras 2
    {180, 150}, //Bras 3
};

const struct pos_servo Pince[]{
    {142, 180}, //Pince 1 Bras 1
    {95, 50}, //Pince 2 Bras 1
    {180, 90}, //Pince 1 Bras 2
    {180, 90}, //Pince 2 Bras 2
    {180, 90}, //Pince 1 Bras 3
    {180, 90}, //Pince 2 Bras 3
   
};

#endif // SERVO_H