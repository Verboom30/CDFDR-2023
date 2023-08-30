#include "Holonome.hpp"

//***********************************/************************************
//                         Constructors                                 //
//***********************************/************************************
Holonome::Holonome()
{
    StepperA_thread.start(callback(this, &Holonome::routine_stepperA));
    StepperB_thread.start(callback(this, &Holonome::routine_stepperB));
    StepperC_thread.start(callback(this, &Holonome::routine_stepperC));
    StepperD_thread.start(callback(this, &Holonome::routine_stepperD));
    routine.start(callback(this, &Holonome::routine_holonome));

    _AckStpA =false;
    _AckStpB =false;
    _AckStpC =false;
    _AckStpD =false;
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

float Holonome::getSpeedD(void)
{
    return StepperD->getSpeed();
}

void Holonome::setPositionZero(void)
{
    StepperA->setPositionZero();
    StepperB->setPositionZero();
    StepperC->setPositionZero();
    StepperD->setPositionZero();
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
float Holonome::getSpeedAlpha(void)
{
    return _SpeedAlpha;
}

bool Holonome::stopped(void)
{
    return (StepperA->stopped() == true and StepperB->stopped()== true and StepperC->stopped() == true and StepperD->stopped() == true) ? true : false;
}

bool Holonome::waitAck(void)
{
    if(_AckStpA ==true and _AckStpB ==true and _AckStpC == true and _AckStpD == true){
      
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
     StepperD->run();        
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
    _SpeedX = (float(_MovepositionX-_positionX)/((_MovepositionX-_positionX)+(_MovepositionY-_positionY)+(_MoveAlpha-_Alpha)))*SPEED;
    _SpeedY = (float(_MovepositionY-_positionY)/((_MovepositionX-_positionX)+(_MovepositionY-_positionY)+(_MoveAlpha-_Alpha)))*SPEED;
    _SpeedAlpha = (float(_MoveAlpha-_Alpha)/((_MovepositionX-_positionX)+(_MovepositionY-_positionY)+(_MoveAlpha-_Alpha)))*SPEED;
    _Cmd = "GOTO";
}

void Holonome::move(int positionX, int positionY, int Alpha)
{
  
    _MovepositionX = positionX;
    _MovepositionY = positionY;
    _MoveAlpha = Alpha;
    _SpeedX = (float(_MovepositionX)/(_MovepositionX+_MovepositionY+_MoveAlpha))*SPEED;
    _SpeedY = (float(_MovepositionY)/(_MovepositionX+_MovepositionY+_MoveAlpha))*SPEED;
    _SpeedAlpha = (float(_MoveAlpha)/(_MovepositionX+_MovepositionY+_MoveAlpha))*SPEED;
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
            StepperA->setSpeed((sin(PI/4.0)*_SpeedX)+(cos(PI/4.0)*_SpeedY)+(_SpeedAlpha)); 
            StepperA->setAcceleration(getSpeedA()/ACC);
            StepperA->setDeceleration(getSpeedA()/DEC); 
            StepperA->move(int(sin(PI/4.0)*(float(_MovepositionX)/KSTP)+cos(PI/4.0)*(float(_MovepositionY)/KSTP)+(RADIUS*float(_MoveAlpha)*((PI/180.0)/KSTP)))); 
            _AckStpA = true;

        }else if (_Cmd == "GOTO" and _AckStpA ==false)
        {   StepperA->setSpeed((sin(PI/4.0)*_SpeedX)+(cos(PI/4.0)*_SpeedY)+(_SpeedAlpha)); 
            StepperA->setAcceleration(getSpeedA()/ACC);
            StepperA->setDeceleration(getSpeedA()/DEC); 
            StepperA->goesTo(int(sin(PI/4.0)*(float(_MovepositionX)/KSTP)+cos(PI/4.0)*(float(_MovepositionY)/KSTP)+(RADIUS*float(_MoveAlpha)*((PI/180.0)/KSTP)))); 
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
            StepperB->setSpeed(sin((3.0*PI)/4.0)*_SpeedX+cos((3.0*PI)/4.0)*_SpeedY+(_SpeedAlpha)); 
            StepperB->setAcceleration(getSpeedB()/ACC);
            StepperB->setDeceleration(getSpeedB()/DEC); 
            StepperB->move(int(sin((3.0*PI)/4.0)*(float(_MovepositionX)/KSTP)+cos((3.0*PI)/4.0)*(float(_MovepositionY)/KSTP)+(RADIUS*float(_MoveAlpha)*((PI/180.0)/KSTP))));  
            _AckStpB = true;

        }else if (_Cmd == "GOTO" and _AckStpB ==false)
        {    StepperB->setSpeed(sin((3.0*PI)/4.0)*_SpeedX+cos((3.0*PI)/4.0)*_SpeedY+(_SpeedAlpha)); 
            StepperB->setAcceleration(getSpeedB()/ACC);
            StepperB->setDeceleration(getSpeedB()/DEC); 
            StepperB->goesTo(int(sin((3.0*PI)/4.0)*(float(_MovepositionX)/KSTP)+cos((3.0*PI)/4.0)*(float(_MovepositionY)/KSTP)+(RADIUS*float(_MoveAlpha)*((PI/180.0)/KSTP))));  
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
            StepperC->setSpeed(sin((5.0*PI)/4.0)*_SpeedX+cos((5.0*PI)/4.0)*_SpeedY+(_SpeedAlpha)); 
            StepperC->setAcceleration(getSpeedC()/ACC);
            StepperC->setDeceleration(getSpeedC()/DEC); 
            StepperC->move(int(sin((5.0*PI)/4.0)*(float(_MovepositionX)/KSTP)+cos((5.0*PI)/4.0)*(float(_MovepositionY)/KSTP)+(RADIUS*float(_MoveAlpha)*((PI/180.0)/KSTP))));
            _AckStpC = true;

        }else if (_Cmd == "GOTO" and _AckStpC ==false)
        {   StepperC->setSpeed(sin((5.0*PI)/4.0)*_SpeedX+cos((5.0*PI)/4.0)*_SpeedY+(_SpeedAlpha)); 
            StepperC->setAcceleration(getSpeedC()/ACC);
            StepperC->setDeceleration(getSpeedC()/DEC); 
            StepperC->goesTo(int(sin((5.0*PI)/4.0)*(float(_MovepositionX)/KSTP)+cos((5.0*PI)/4.0)*(float(_MovepositionY)/KSTP)+(RADIUS*float(_MoveAlpha)*((PI/180.0)/KSTP))));
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


void Holonome::routine_stepperD(void)
{
    while (1) 
    {
      

        if(_Cmd == "MOVE" and _AckStpD ==false){
            StepperD->setSpeed(sin((7.0*PI)/4.0)*_SpeedX+cos((7.0*PI)/4.0)*_SpeedY+(_SpeedAlpha)); 
            StepperD->setAcceleration(getSpeedD()/ACC);
            StepperD->setDeceleration(getSpeedD()/DEC); 
            StepperD->move(int(sin((7.0*PI)/4.0)*(float(_MovepositionX)/KSTP)+cos((7.0*PI)/4.0)*(float(_MovepositionY)/KSTP)+(RADIUS*float(_MoveAlpha)*((PI/180.0)/KSTP))));
            _AckStpD = true;

        }else if (_Cmd == "GOTO" and _AckStpD ==false)
        {
            StepperD->setSpeed(sin((7.0*PI)/4.0)*_SpeedX+cos((7.0*PI)/4.0)*_SpeedY+(_SpeedAlpha)); 
            StepperD->setAcceleration(getSpeedD()/ACC);
            StepperD->setDeceleration(getSpeedD()/DEC); 
            StepperD->goesTo(int(sin((7.0*PI)/4.0)*(float(_MovepositionX)/KSTP)+cos((7.0*PI)/4.0)*(float(_MovepositionY)/KSTP)+(RADIUS*float(_MoveAlpha)*((PI/180.0)/KSTP))));
            _AckStpD = true;

        }else if (_Cmd == "STOP")
        {
            StepperD->stop();
            
        }else if (_Cmd == "ACK")
        {
            _AckStpD = false;
        }
        
    }    
}

void Holonome::routine_holonome(void)
{
    while (1)
    {
        _positionX  = ((sqrt(2.0)/4.0)*StepperA->getPosition() + (sqrt(2.0)/4.0)*StepperB->getPosition() - (sqrt(2.0)/4.0)*StepperC->getPosition() - (sqrt(2.0)/4.0)*StepperD->getPosition()) * KSTP;     
        _positionY  = ((sqrt(2.0)/4.0)*StepperA->getPosition() - (sqrt(2.0)/4.0)*StepperB->getPosition() - (sqrt(2.0)/4.0)*StepperC->getPosition() + (sqrt(2.0)/4.0)*StepperD->getPosition()) * KSTP;  
        _Alpha      = (StepperA->getPosition() + StepperB->getPosition() + StepperC->getPosition() + StepperD->getPosition())*(1.0/(4.0*RADIUS))*KSTP/(PI/180.0);
    }
}