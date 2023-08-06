#include "Holonome.hpp"

//***********************************/************************************
//                         Constructors                                 //
//***********************************/************************************
Holonome::Holonome()
{
    StepperA_thread.start(callback(this, &Holonome::routine_stepperA));
    StepperB_thread.start(callback(this, &Holonome::routine_stepperB));
    StepperC_thread.start(callback(this, &Holonome::routine_stepperC));
    _AckStpA =false;
    _AckStpB =false;
    _AckStpC =false;
    _Cmd =" ";
   
}

//***********************************/************************************
//                                Get Set                               //
//***********************************/************************************
void Holonome::setSpeed(int Vx, int Vy, int theta)
{
    StepperA->setSpeed((-RADIUS*theta - 0.5*Vx - sin(PI/3.0)*Vy));    
    StepperB->setSpeed((-RADIUS*theta + Vx)); 
    StepperC->setSpeed((-RADIUS*theta - 0.5*Vx + sin(PI/3.0)*Vy));

    StepperA->setAcceleration((-RADIUS*theta - 0.5*Vx - sin(PI/3.0)*Vy)/4);
    StepperB->setAcceleration((-RADIUS*theta + Vx)/4);
    StepperC->setAcceleration((-RADIUS*theta - 0.5*Vx + sin(PI/3.0)*Vy)/4);

    StepperA->setDeceleration((-RADIUS*theta - 0.5*Vx - sin(PI/3.0)*Vy)/4);
    StepperB->setDeceleration((-RADIUS*theta + Vx)/4);
    StepperC->setDeceleration((-RADIUS*theta - 0.5*Vx + sin(PI/3.0)*Vy)/4);
}

float Holonome::getSpeedA(void)
{
    return StepperA->getSpeed();
}

float Holonome::getSpeedB(void)
{
    return StepperB->getSpeed();
}

float Holonome::getSpeedC(void)
{
    return StepperC->getSpeed();
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

bool Holonome::waitAck(void)
{
    if(_AckStpA ==true and _AckStpB ==true and _AckStpC == true){
      
        _Cmd ="ACK";
        return 1;
    }else{
        return 0;
    }
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
    _Cmd = "STOP";
     
}

void Holonome::goesTo(int positionX, int positionY, int Alpha)
{
    StepperA->goesTo((-RADIUS*Alpha - 0.5*positionX - sin(PI/3.0)*positionY));    
    StepperB->goesTo((-RADIUS*Alpha + positionX)); 
    StepperC->goesTo((-RADIUS*Alpha - 0.5*positionX + sin(PI/3.0)*positionY));
}

void Holonome::move(int positionX, int positionY, int Alpha)
{
  
    _positionX = positionX;
    _positionY = positionY;
    _Alpha = Alpha;
    _Cmd = "MOVE";
   
    
}


//***********************************/************************************
//                          Protected Methods                           //
//***********************************/************************************
void Holonome::routine_stepperA(void)
{

    while (1)
    {
        if(_Cmd == "MOVE" and _AckStpA ==false){

            StepperA->move((-RADIUS*_Alpha - 0.5*_positionX - sin(PI/3.0)*_positionY)); 
            _AckStpA = true;

        }else if (_Cmd == "STOP")
        {
            StepperA->stop();
           
        }else if (_Cmd == "ACK")
        {
            _AckStpA = false;
        }
        
    }
    
}

void Holonome::routine_stepperB(void)
{
  
   while (1)
   {
        if(_Cmd == "MOVE" and _AckStpB ==false){

            StepperB->move((-RADIUS*_Alpha + _positionX));
            _AckStpB = true;

        }else if (_Cmd == "STOP")
        {
            StepperB->stop();
          
        }else if (_Cmd == "ACK")
        {
            _AckStpB = false;
        }
        
    }
}

void Holonome::routine_stepperC(void)
{
    while (1) 
    {
        if(_Cmd == "MOVE" and _AckStpC ==false){

            StepperC->move((-RADIUS*_Alpha - 0.5*_positionX + sin(PI/3.0)*_positionY));
            _AckStpC = true;

        }else if (_Cmd == "STOP")
        {
            StepperC->stop();
            
        }else if (_Cmd == "ACK")
        {
            _AckStpC = false;
        }
        
    }    
}