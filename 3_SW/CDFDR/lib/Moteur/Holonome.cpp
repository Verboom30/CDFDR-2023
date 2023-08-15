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
    _Alpha = 0;
    _positionY =0;
    _positionX =0;
    _SpeedX    =0;
    _SpeedY    =0;
   
}

//***********************************/************************************
//                                Get Set                               //
//***********************************/************************************
void Holonome::setSpeed(int Vx, int Vy, int theta)
{
    // StepperA->setSpeed((-RADIUS*theta - 0.5*(Vx) - sin(PI/3.0)*(Vy)));    
    // StepperB->setSpeed((-RADIUS*theta + (Vx))); 
    // StepperC->setSpeed((-RADIUS*theta - 0.5*(Vx) + sin(PI/3.0)*(Vy)));

    // StepperA->setDeceleration((-RADIUS*theta/DEC - 0.5*(Vx/DEC) - sin(PI/3.0)*(Vy/DEC)));
    // StepperB->setDeceleration((-RADIUS*theta/DEC + (Vx/DEC)));
    // StepperC->setDeceleration((-RADIUS*theta/DEC - 0.5*(Vx/DEC) + sin(PI/3.0)*(Vy/DEC)));

    // StepperA->setAcceleration((-RADIUS*theta/ACC - 0.5*(Vx/ACC) - sin(PI/3.0)*(Vy/ACC)));
    // StepperB->setAcceleration((-RADIUS*theta/ACC + (Vx/ACC)));
    // StepperC->setAcceleration((-RADIUS*theta/ACC - 0.5*(Vx/ACC) + sin(PI/3.0)*(Vy/ACC)));

    
    StepperA->setDeceleration(0);
    StepperB->setDeceleration(0);
    StepperC->setDeceleration(0);

    StepperA->setAcceleration(0);
    StepperB->setAcceleration(0);
    StepperC->setAcceleration(0);
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

float Holonome::getAcceleration(void)
{
    return  StepperA->getAcceleration();
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

void Holonome::getPosition(void)
{
   _positionX  =   (((2.0/3.0)*StepperB->getPosition()) - ((1.0/3.0)*StepperA->getPosition()) - ((1.0/3.0)*StepperC->getPosition())) * KSTP;     
   _positionY  =   ((-tan(PI/6.0)*StepperA->getPosition()) + (tan(PI/6.0)*StepperC->getPosition())) * KSTP;
   _Alpha      =   (((-1.0/(3.0*RADIUS))*StepperB->getPosition()) + ((-1.0/(3.0*RADIUS))*StepperA->getPosition()) + ((-1.0/(3.0*RADIUS))*StepperC->getPosition()))*KSTP/(PI/180.0);
}

float Holonome::getPositionX(void)
{
    return _positionX;
}

float Holonome::getPositionY(void)
{
    return _positionY;
}

float Holonome::getAlpha(void)
{
    return _Alpha;
}

float Holonome::getSpeedX(void)
{
    return _SpeedX;
}

float Holonome::getSpeedY(void)
{
    return _SpeedY;
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
    StepperA->goesTo((-RADIUS*Alpha*(PI/180.0)/KSTP - 0.5*positionX/KSTP - sin(PI/3.0)*positionY/KSTP));    
    StepperB->goesTo((-RADIUS*Alpha*(PI/180.0)/KSTP + positionX/KSTP)); 
    StepperC->goesTo((-RADIUS*Alpha*(PI/180.0)/KSTP - 0.5*positionX/KSTP + sin(PI/3.0)*positionY/KSTP));
}

void Holonome::move(int positionX, int positionY, int Alpha)
{
  
    _MovepositionX = positionX;
    _MovepositionY = positionY;
    _MoveAlpha = Alpha;
    _SpeedX = (float(positionX)/(positionX+positionY))*SPEED;
    _SpeedY = (float(positionY)/(positionX+positionY))*SPEED;
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

            StepperA->setSpeed((-RADIUS*_MoveAlpha - 0.5*(_SpeedX) - sin(PI/3.0)*(_SpeedY)));  
            StepperA->move((-RADIUS*_MoveAlpha*((PI/180.0)/KSTP) - 0.5*(_MovepositionX/KSTP) - sin(PI/3.0)*(_MovepositionY/KSTP))); 
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

            StepperB->setSpeed((-RADIUS*_MoveAlpha*SPEED + (_SpeedX))); 
            StepperB->move((-RADIUS*_MoveAlpha*((PI/180.0)/KSTP) + (_MovepositionX/KSTP)));
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
            StepperC->setSpeed((-RADIUS*_MoveAlpha*SPEED - 0.5*(_SpeedX) + sin(PI/3.0)*(_SpeedY)));
            StepperC->move((-RADIUS*_MoveAlpha*((PI/180.0)/KSTP) - 0.5*(_MovepositionX/KSTP) + sin(PI/3.0)*(_MovepositionY/KSTP)));
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