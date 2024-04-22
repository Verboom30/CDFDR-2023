#include "Holonome.hpp"
#include "main_pkg.hpp"
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
    _Cmd ="STOP";
    _Alpha = 0;
    _positionY  =0;
    _positionX  =0;
    _SpeedX     =0;
    _SpeedY     =0;
    _SpeedAlpha =0;
    _MovepositionY =0;
    _MovepositionX =0;
    _MoveAlpha=0;

   
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


int Holonome::getPosA(void)
{
    return StepperA->getPosition();
}

int Holonome::getPosB(void)
{
    return StepperB->getPosition();
}

int Holonome::getPosC(void)
{
    return StepperC->getPosition();
}

int Holonome::getStepA(void)
{
    return StepperA->getStep();
}

int Holonome::getStepB(void)
{
    return StepperB->getStep();
}

int Holonome::getStepC(void)
{
    return StepperC->getStep();
}

void Holonome::setPosition(int positionX, int positionY, int Alpha)
{
    _MovepositionX =positionX;
    _MovepositionY =positionY;
    _MoveAlpha =Alpha;
    StepperA->setPosition(int(((-RADIUS*Alpha*(PI/180.0)) - (positionX))/KSTP));
    StepperB->setPosition(int(((-RADIUS*Alpha*(PI/180.0)) + 0.5*positionX - sin((PI/180.0)*THETA_A)*positionY)/KSTP));
    StepperC->setPosition(int(((-RADIUS*Alpha*(PI/180.0)) + 0.5*positionX + sin((PI/180.0)*THETA_A)*positionY)/KSTP));
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

float Holonome::getPosCibleX(void)
{
    return _MovepositionX;
}

float Holonome::getPosCibleY(void)
{
    return _MovepositionY;
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
float Holonome::getSpeedAlpha(void)
{
    return _SpeedAlpha;
}

bool Holonome::stopped(void)
{
    return (StepperA->stopped() == true and StepperB->stopped()== true and StepperC->stopped() == true) ? true : false;
}

bool Holonome::waitAck(void)
{
    if(_AckStpA ==true and _AckStpB ==true and _AckStpC ==true ){
      
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
    _MovepositionX = positionX         ; 
    _MovepositionY = positionY         ; 
    _MoveAlpha     = Alpha; // Deg vers Rad 
    _SpeedX = (float((_MovepositionX-_positionX))/(abs(_MovepositionX-_positionX)+abs(_MovepositionY-_positionY)+abs(_MoveAlpha-_Alpha)))*SPEED;
    _SpeedY = (float((_MovepositionY-_positionY))/(abs(_MovepositionX-_positionX)+abs(_MovepositionY-_positionY)+abs(_MoveAlpha-_Alpha)))*SPEED;
    _SpeedAlpha = (float((_MoveAlpha-_Alpha))/(abs(_MovepositionX-_positionX)+abs(_MovepositionY-_positionY)+abs(_MoveAlpha-_Alpha)))*float(3*SPEED/RADIUS);
    _Cmd = "GOTO";
}

void Holonome::move(int positionX, int positionY, int Alpha)
{
    _MovepositionX = positionX         ; 
    _MovepositionY = positionY         ; 
    _MoveAlpha     = Alpha             ; // Deg vers Rad  
    _SpeedX = (float((_MovepositionX))/(abs(_MovepositionX)+abs(_MovepositionY)+abs(_MoveAlpha)))*SPEED;
    _SpeedY = (float((_MovepositionY))/(abs(_MovepositionX)+abs(_MovepositionY)+abs(_MoveAlpha)))*SPEED;
    _SpeedAlpha = (float(_MoveAlpha)/(abs(_MovepositionX)+abs(_MovepositionY)+abs(_MoveAlpha)))*float(3*SPEED/RADIUS);
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

            StepperA->setSpeed(((-RADIUS*_SpeedAlpha) + (_SpeedX))/KSTP);
            StepperA->setAcceleration(getSpeedA()/ACC);
            StepperA->setDeceleration(getSpeedA()/DEC); 
            StepperA->move(int(((-RADIUS*_MoveAlpha*(PI/180.0)) - (_MovepositionX))/KSTP));
            _AckStpA = true;

        }else if (_Cmd == "GOTO" and _AckStpA ==false){    
            StepperA->setSpeed(((-RADIUS*_SpeedAlpha) + (_SpeedX))/KSTP);
            StepperA->setAcceleration(getSpeedA()/ACC);
            StepperA->setDeceleration(getSpeedA()/DEC); 
            StepperA->goesTo(int(((-RADIUS*_MoveAlpha*(PI/180.0)) - (_MovepositionX))/KSTP));
            _AckStpA = true;

        }else if (_Cmd == "STOP" and _AckStpA ==false){
            StepperB->stop();
            _AckStpA = true;
          
        }else if (_Cmd == "ACK"){
            _AckStpA = false;
        }
    }
}

void Holonome::routine_stepperB(void)
{
   while (1)
   {
        if(_Cmd == "MOVE" and _AckStpB ==false){
            StepperB->setSpeed(((-RADIUS*_SpeedAlpha) + 0.5*_SpeedX - sin((PI/180.0)*THETA_A)*_SpeedY)/KSTP); 
            StepperB->setAcceleration(getSpeedB()/ACC);
            StepperB->setDeceleration(getSpeedB()/DEC); 
            StepperB->move(int(((-RADIUS*_MoveAlpha*(PI/180.0)) + 0.5*_MovepositionX - sin((PI/180.0)*THETA_A)*_MovepositionY)/KSTP)); 
            _AckStpB = true;

        }else if (_Cmd == "GOTO" and _AckStpB ==false){   
            StepperB->setSpeed(((-RADIUS*_SpeedAlpha) + 0.5*_SpeedX - sin((PI/180.0)*THETA_A)*_SpeedY)/KSTP); 
            StepperB->setAcceleration(getSpeedB()/ACC);
            StepperB->setDeceleration(getSpeedB()/DEC); 
            StepperB->goesTo(int(((-RADIUS*_MoveAlpha*(PI/180.0)) + 0.5*_MovepositionX - sin((PI/180.0)*THETA_A)*_MovepositionY)/KSTP)); 
            _AckStpB = true;

        }else if (_Cmd == "STOP" and _AckStpB ==false){
            StepperC->stop();
            _AckStpB = true;
            
        }else if (_Cmd == "ACK"){
            _AckStpB = false;
        } 
    }
}

void Holonome::routine_stepperC(void)
{
    while (1) 
    {
        if(_Cmd == "MOVE" and _AckStpC == false){
            StepperC->setSpeed(((-RADIUS*_SpeedAlpha) + 0.5*_SpeedX + sin((PI/180.0)*THETA_A)*_SpeedY)/KSTP); 
            StepperC->setAcceleration(getSpeedC()/ACC);
            StepperC->setDeceleration(getSpeedC()/DEC); 
            StepperC->move(int(((-RADIUS*_MoveAlpha*(PI/180.0)) + 0.5*_MovepositionX + sin((PI/180.0)*THETA_A)*_MovepositionY)/KSTP)); 

            _AckStpC = true;

        }else if (_Cmd == "GOTO" and _AckStpC == false){  
            StepperC->setSpeed(((-RADIUS*_SpeedAlpha) + 0.5*_SpeedX + sin((PI/180.0)*THETA_A)*_SpeedY)/KSTP); 
            StepperC->setAcceleration(getSpeedC()/ACC);
            StepperC->setDeceleration(getSpeedC()/DEC); 
            StepperC->goesTo(int(((-RADIUS*_MoveAlpha*(PI/180.0)) + 0.5*_MovepositionX + sin((PI/180.0)*THETA_A)*_MovepositionY)/KSTP)); 
            _AckStpC = true;

        }else if (_Cmd == "STOP" and _AckStpC ==false){
            StepperC->stop();
            _AckStpC = true;
           
        }else if (_Cmd == "ACK"){
            _AckStpC = false;
        }
    }    
}

void Holonome::routine_holonome(void)
{
    while (1)
    {
        _positionX  =   -(((2.0/3.0)*StepperA->getPosition()) - ((1.0/3.0)*StepperB->getPosition()) - ((1.0/3.0)*StepperC->getPosition())) * KSTP;     
        _positionY  =   ((-tan(PI/6.0)*StepperB->getPosition()) + (tan(PI/6.0)*StepperC->getPosition())) * KSTP;
        _Alpha      =   (((-1.0/(3.0*RADIUS))*StepperB->getPosition()) + ((-1.0/(3.0*RADIUS))*StepperA->getPosition()) + ((-1.0/(3.0*RADIUS))*StepperC->getPosition()))*KSTP/(PI/180.0);
    }
}