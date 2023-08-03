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
void Holonome::setSpeed(float speed)
{
    StepperA->setSpeed(speed);
    StepperB->setSpeed(speed);
    StepperC->setSpeed(speed);
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
