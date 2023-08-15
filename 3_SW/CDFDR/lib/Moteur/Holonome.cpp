#include "Holonome.hpp"

//***********************************/************************************
//                         Constructors                                 //
//***********************************/************************************
Holonome::Holonome()
{
    StepperA_thread.start(callback(this, &Holonome::routine_stepperA));
    StepperB_thread.start(callback(this, &Holonome::routine_stepperB));
    StepperC_thread.start(callback(this, &Holonome::routine_stepperC));
    routine.start(callback(this, &Holonome::routine_holonome));

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

void Holonome::setPositionZero(void)
{
    StepperA->setPositionZero();
    StepperB->setPositionZero();
    StepperC->setPositionZero();
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
    _MovepositionX = positionX;
    _MovepositionY = positionY;
    _MoveAlpha = Alpha;
    _SpeedX = (float(_MovepositionX-_positionX)/((_MovepositionX-_positionX)+(_MovepositionY-_positionY)))*SPEED;
    _SpeedY = (float(_MovepositionY-_positionY)/((_MovepositionX-_positionX)+(_MovepositionY-_positionY)))*SPEED;
    _Cmd = "GOTO";
}

void Holonome::move(int positionX, int positionY, int Alpha)
{
  
    _MovepositionX = positionX;
    _MovepositionY = positionY;
    _MoveAlpha = Alpha;
    _SpeedX = (float(_MovepositionX)/(_MovepositionX+_MovepositionY))*SPEED;
    _SpeedY = (float(_MovepositionY)/(_MovepositionX+_MovepositionY))*SPEED;
    _Cmd = "MOVE";
   
    
}


//***********************************/************************************
//                          Protected Methods                           //
//***********************************/************************************
void Holonome::routine_stepperA(void)
{

    while (1)
    {   
        StepperA->setSpeed((-RADIUS*_MoveAlpha - 0.5*(_SpeedX) - sin(PI/3.0)*(_SpeedY))); 
        StepperA->setAcceleration(getSpeedA()/ACC);
        StepperA->setDeceleration(getSpeedA()/DEC); 

        if(_Cmd == "MOVE" and _AckStpA ==false){

            StepperA->move((-RADIUS*_MoveAlpha*((PI/180.0)/KSTP) - 0.5*(_MovepositionX/KSTP) - sin(PI/3.0)*(_MovepositionY/KSTP))); 
            _AckStpA = true;

        }else if (_Cmd == "GOTO" and _AckStpA ==false)
        {
             StepperA->goesTo((-RADIUS*_MoveAlpha*((PI/180.0)/KSTP) - 0.5*(_MovepositionX/KSTP) - sin(PI/3.0)*(_MovepositionY/KSTP))); 
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
        StepperB->setSpeed((-RADIUS*_MoveAlpha*SPEED + (_SpeedX))); 
        StepperB->setAcceleration(getSpeedB()/ACC);
        StepperB->setDeceleration(getSpeedB()/DEC); 

        if(_Cmd == "MOVE" and _AckStpB ==false){

            StepperB->move((-RADIUS*_MoveAlpha*((PI/180.0)/KSTP) + (_MovepositionX/KSTP)));
            _AckStpB = true;

        }else if (_Cmd == "GOTO" and _AckStpB ==false)
        {
             StepperB->goesTo((-RADIUS*_MoveAlpha*((PI/180.0)/KSTP) + (_MovepositionX/KSTP)));
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
        StepperC->setSpeed((-RADIUS*_MoveAlpha*SPEED - 0.5*(_SpeedX) + sin(PI/3.0)*(_SpeedY)));
        StepperC->setAcceleration(getSpeedC()/ACC);
        StepperC->setDeceleration(getSpeedC()/DEC); 

        if(_Cmd == "MOVE" and _AckStpC ==false){
            StepperC->move((-RADIUS*_MoveAlpha*((PI/180.0)/KSTP) - 0.5*(_MovepositionX/KSTP) + sin(PI/3.0)*(_MovepositionY/KSTP)));
            _AckStpC = true;

        }else if (_Cmd == "GOTO" and _AckStpC ==false)
        {
            StepperC->goesTo((-RADIUS*_MoveAlpha*((PI/180.0)/KSTP) - 0.5*(_MovepositionX/KSTP) + sin(PI/3.0)*(_MovepositionY/KSTP)));
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

void Holonome::routine_holonome(void)
{
    while (1)
    {
        _positionX  =   (((2.0/3.0)*StepperB->getPosition()) - ((1.0/3.0)*StepperA->getPosition()) - ((1.0/3.0)*StepperC->getPosition())) * KSTP;     
        _positionY  =   ((-tan(PI/6.0)*StepperA->getPosition()) + (tan(PI/6.0)*StepperC->getPosition())) * KSTP;
        _Alpha      =   (((-1.0/(3.0*RADIUS))*StepperB->getPosition()) + ((-1.0/(3.0*RADIUS))*StepperA->getPosition()) + ((-1.0/(3.0*RADIUS))*StepperC->getPosition()))*KSTP/(PI/180.0);
    }
}