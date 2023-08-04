#include "Holonome.hpp"


//***********************************/************************************
//                         Constructors                                 //
//***********************************/************************************
Holonome::Holonome()
{
   
   
}

//***********************************/************************************
//                                Get Set                               //
//***********************************/************************************
void Holonome::setSpeed(int Vx, int Vy, int theta)
{
    StepperA->setSpeed((-RADIUS*theta - 0.5*Vx - sin(PI/3.0)*Vy)*COEFF);    
    StepperB->setSpeed((-RADIUS*theta + Vx)*COEFF); 
    StepperC->setSpeed((-RADIUS*theta - 0.5*Vx + sin(PI/3.0)*Vy)*COEFF);
}

float Holonome::getSpeed(void)
{
    return StepperA->getSpeed();
}

void Holonome::setAcceleration(float acc)
{
    StepperA->setAcceleration(acc);
    StepperB->setAcceleration(acc);
    StepperC->setAcceleration(acc);
}

float Holonome::getAcceleration(void)
{
    return  StepperA->getAcceleration();
}

void Holonome::setDeceleration(float dec)
{
    StepperA->setDeceleration(dec);
    StepperB->setDeceleration(dec);
    StepperC->setDeceleration(dec);
}

float Holonome::getDeceleration(void)
{
    return StepperA->getDeceleration();
}

void Holonome::setPositionZero(void)
{
    StepperA->setPositionZero();
    StepperB->setPositionZero();
    StepperC->setPositionZero();
}

void Holonome::getPosition(int *positionX, int *positionY, int *Alpha)
{
    *positionX  =   StepperA->getPosition();
    *positionY  =   StepperB->getPosition();
    *Alpha      =   StepperC->getPosition();
}

bool Holonome::stopped(void)
{
    return (StepperA->stopped() == true and StepperB->stopped()== true and StepperC->stopped() == true) ? true : false;
}

//***********************************/************************************
//                             Public Methods                           //
//***********************************/************************************

void Holonome::run(void)
{
     StepperA->run();    
     StepperB->run();    
     StepperC->run();    
}

void Holonome::stop(void)
{   
     StepperA->stop();    
     StepperB->stop();    
     StepperC->stop();  
}

void Holonome::goesTo(int positionX, int positionY, int Alpha)
{
    StepperA->goesTo((-RADIUS*Alpha - 0.5*positionX - sin(PI/3.0)*positionY)*COEFF);    
    StepperB->goesTo((-RADIUS*Alpha + positionX)*COEFF); 
    StepperC->goesTo((-RADIUS*Alpha - 0.5*positionX + sin(PI/3.0)*positionY)*COEFF);
}

void Holonome::move(int positionX, int positionY, int Alpha)
{
  
    _positionX = positionX;
    _positionY = positionY;
    _Alpha = Alpha;
    StepperA_thread.start(callback(this, &Holonome::routine_stepperA));
    StepperB_thread.start(callback(this, &Holonome::routine_stepperB));
    StepperC_thread.start(callback(this, &Holonome::routine_stepperC));
   
    
}


//***********************************/************************************
//                          Protected Methods                           //
//***********************************/************************************
void Holonome::routine_stepperA(void)
{
    StepperA->move((-RADIUS*_Alpha - 0.5*_positionX - sin(PI/3.0)*_positionY)*COEFF);    
    while (1)
    {
        //StepperA->run(); 
    }
    
}

void Holonome::routine_stepperB(void)
{
    StepperB->move((-RADIUS*_Alpha + _positionX)*COEFF); 
    while (1)
    {
        // StepperB->run();    
    }
}

void Holonome::routine_stepperC(void)
{
    StepperC->move((-RADIUS*_Alpha - 0.5*_positionX + sin(PI/3.0)*_positionY)*COEFF);
    while (1)
    {   
         //StepperC->run();    
    }
}