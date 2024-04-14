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
    {145, 90}, //Bras 1
    {150, 90}, //Bras 2
    {147, 90}, //Bras 3
};

const struct pos_servo Pince[]{
    {115, 150}, //Pince 1 Bras 1
    {105, 60}, //Pince 2 Bras 1
    {118, 160}, //Pince 1 Bras 2
    {85, 50}, //Pince 2 Bras 2
    {50, 95}, //Pince 1 Bras 3
    {115, 65}, //Pince 2 Bras 3
   
};

#endif // SERVO_H