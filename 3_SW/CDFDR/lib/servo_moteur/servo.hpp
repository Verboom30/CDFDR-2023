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
    {145, 85}, //Bras 1
    {150, 90}, //Bras 2
    {147, 90}, //Bras 3
};

const struct pos_servo Pince[]{
    {122, 146}, //Pince 1 Bras 1
    {90, 55}, //Pince 2 Bras 1
    {105, 133}, //Pince 1 Bras 2
    {30, 0}, //Pince 2 Bras 2
    {85, 115}, //Pince 1 Bras 3
    {122, 90}, //Pince 2 Bras 3
   
};

#endif // SERVO_H