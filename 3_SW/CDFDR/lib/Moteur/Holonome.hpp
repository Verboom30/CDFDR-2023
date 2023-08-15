#ifndef HOLONOME_H
#define HOLONOME_H

#include "mbed.h"
#include "pinout.hpp"
#include "Stepper.hpp"
#include <string.h>

class Holonome 
{
    public:
        Holonome();
        Stepper* StepperA = new Stepper(STEP_M1, DIR_M1);
        Stepper* StepperB = new Stepper(STEP_M2, DIR_M2);
        Stepper* StepperC = new Stepper(STEP_M3, DIR_M3);
      

        void    getPosition(void);
        float   getPositionX(void);
        float   getPositionY(void);
        float   getAlpha(void);
        float   getAcceleration(void);
        float   getDeceleration(void);
        float   getSpeedX(void);
        float   getSpeedY(void);
        
        void  setSpeed(int Vx, int Vy, int theta);
        void  setAcceleration(float acc);
        void  setDeceleration(float dec);
        void  setPositionZero(void);
        bool  stopped(void);
        bool  waitAck(void);

        void  HolonomeSpeed(int SpeedX, int SpeedY, int W);
        void  goesTo(int positionX, int positionY, int Alpha); 
        void  move(int positionX, int positionY, int Alpha); 
        void  stop(void);
        void  run(void);
        

        

       
        


       
    private : 
        float _acc;                             //Acceleration [step/s²]
        float _dec;                             //Decceleration [step/s²]
        float _spd;                             //Speed [step/s]
        float _Alpha;
        float _positionX;
        float _positionY;
        int   _MoveAlpha; 
        int   _MovepositionX;
        int   _MovepositionY;
        float _SpeedX;
        float _SpeedY;
        
        string _Cmd;
        bool _AckStpA;
        bool _AckStpB;
        bool _AckStpC;

        Thread StepperA_thread;
        Thread StepperB_thread;
        Thread StepperC_thread;
        
        void routine_stepperA(void);
        void routine_stepperB(void);
        void routine_stepperC(void);
        float getSpeedA(void);
        float getSpeedB(void);
        float getSpeedC(void);

        
};
#endif


