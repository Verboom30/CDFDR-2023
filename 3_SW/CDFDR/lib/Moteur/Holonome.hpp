#ifndef HOLONOME_H
#define HOLONOME_H

#include "mbed.h"
#include "pinout.hpp"
#include "Stepper.hpp"

#define COEFF  1
#define PI 3.14159265
#define RADIUS 0.136 // robot wheel-base radius


class Holonome 
{
    public:
        Holonome();
        Stepper* StepperA = new Stepper(STEP_M1, DIR_M1);
        Stepper* StepperB = new Stepper(STEP_M2, DIR_M2);
        Stepper* StepperC = new Stepper(STEP_M3, DIR_M3);

        void getPosition(int *positionX, int *positionY, int *Alpha);
        float getAcceleration(void);
        float getDeceleration(void);
        float getSpeed(void);
        void setSpeed(float speed);
        void setAcceleration(float acc);
        void setDeceleration(float dec);
        void setPositionZero(void);
        bool stopped(void);

        void HolonomeSpeed(int SpeedX, int SpeedY, int W);
        void goesTo(int positionX, int positionY, int Alpha); 
        void stop(void);
        void run(void);


       
    private : 
        float _acc;                             //Acceleration [step/s²]
        float _dec;                             //Decceleration [step/s²]
        float _spd;                             //Speed [step/s]
};
#endif