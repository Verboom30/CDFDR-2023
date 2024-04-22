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
        float   getPosCibleX(void);
        float   getPosCibleY(void);
        float   getAlpha(void);
        float   getSpeedX(void);
        float   getSpeedY(void);
        float   getSpeedAlpha(void);
        
    
        void  setPosition(int positionX, int positionY, int Alpha);
        void  setPositionZero(void);
        bool  stopped(void);
        bool  waitAck(void);
        bool  PosCibleDone(void);

        void  HolonomeSpeed(int SpeedX, int SpeedY, int W);
        void  goesTo(int positionX, int positionY, int Alpha); 
        void  move(int positionX, int positionY, int Alpha); 
        void  stop(void);
        void  run(void);
        float getSpeedA(void);
        float getSpeedB(void);
        float getSpeedC(void);
        int getPosA(void);
        int getPosB(void);
        int getPosC(void);
        int getStepA(void);
        int getStepB(void);
        int getStepC(void);
        
    private : 
        float _acc;                             //Acceleration [step/s²]
        float _dec;                             //Decceleration [step/s²]
        float _spd;                             //Speed [step/s]
        float _Alpha;
        float _positionX;
        float _positionY;
        float _MoveAlpha; 
        float _MovepositionX;
        float _MovepositionY;
        float _SpeedX;
        float _SpeedY;
        float _SpeedAlpha;
        
        string _Cmd;
        bool _AckStpA;
        bool _AckStpB;
        bool _AckStpC;

        Thread StepperA_thread;
        Thread StepperB_thread;
        Thread StepperC_thread;
        Thread routine;
        
        void routine_stepperA(void);
        void routine_stepperB(void);
        void routine_stepperC(void);
        void routine_holonome(void);
        
        
};
#endif


