#include "Holonome.hpp"

//***********************************/************************************
//                         Constructors                                 //
//***********************************/************************************
Holonome::Holonome()
{
    StepperA_thread.start(callback(this, &Holonome::routine_stepperA));
    // StepperB_thread.start(callback(this, &Holonome::routine_stepperB));
    // StepperC_thread.start(callback(this, &Holonome::routine_stepperC));
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
    if(_AckStpA ==true ){
      
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
        if(_Cmd == "MOVE" and _AckStpA == false){
            // StepperA->setSpeed((-RADIUS*_SpeedAlpha) - (0.5*(_SpeedX)) - (sin(PI/3.0)*(_SpeedY))); 
            // StepperA->setAcceleration(getSpeedA()/ACC);
            // StepperA->setDeceleration(getSpeedA()/DEC); 
            // StepperA->move(int((-RADIUS*float(_MoveAlpha)*((PI/180.0)/KSTP)) - (0.5*(float(_MovepositionX)/KSTP)) - (sin(PI/3.0)*(float(_MovepositionY)/KSTP)))); 

            // StepperC->setSpeed((-RADIUS*_SpeedAlpha) - (0.5*(_SpeedX)) + (sin(PI/3.0)*(_SpeedY))); 
            // StepperC->setAcceleration(getSpeedC()/ACC);
            // StepperC->setDeceleration(getSpeedC()/DEC); 
            // StepperC->move(int((-RADIUS*float(_MoveAlpha)*((PI/180.0)/KSTP)) - (0.5*(float(_MovepositionX)/KSTP)) + (sin(PI/3.0)*(float(_MovepositionY)/KSTP))));

            // StepperB->setSpeed((-RADIUS*_SpeedAlpha) + (_SpeedX));
            // StepperB->setAcceleration(getSpeedB()/ACC);
            // StepperB->setDeceleration(getSpeedB()/DEC); 
            // StepperB->move(int(-RADIUS*float(_MoveAlpha)*((PI/180.0)/KSTP) + (float(_MovepositionX)/KSTP))); 


             StepperA->setSpeed(10000); 
             StepperB->setSpeed(10000); 
             StepperA->setAcceleration(getSpeedA()/ACC);
             StepperA->setDeceleration(getSpeedA()/DEC);
             StepperB->setAcceleration(getSpeedB()/ACC);
             StepperB->setDeceleration(getSpeedB()/DEC); 
             StepperA->move(20000);
             StepperB->move(-20000);
            _AckStpA = true;

        }else if (_Cmd == "GOTO" and _AckStpA == false){  
            // StepperA->setSpeed((-RADIUS*_SpeedAlpha) - (0.5*(_SpeedX)) - (sin(PI/3.0)*(_SpeedY))); 
            // StepperA->setAcceleration(getSpeedA()/ACC);
            // StepperA->setDeceleration(getSpeedA()/DEC); 
            // StepperA->goesTo(int((-RADIUS*float(_MoveAlpha)*((PI/180.0)/KSTP)) - (0.5*(float(_MovepositionX)/KSTP)) - (sin(PI/3.0)*(float(_MovepositionY)/KSTP)))); 

            // StepperC->setSpeed((-RADIUS*_SpeedAlpha) - (0.5*(_SpeedX)) + (sin(PI/3.0)*(_SpeedY))); 
            // StepperC->setAcceleration(getSpeedC()/ACC);
            // StepperC->setDeceleration(getSpeedC()/DEC); 
            // StepperC->goesTo(int((-RADIUS*float(_MoveAlpha)*((PI/180.0)/KSTP)) - (0.5*(float(_MovepositionX)/KSTP)) + (sin(PI/3.0)*(float(_MovepositionY)/KSTP)))); 

            // StepperB->setSpeed((-RADIUS*_SpeedAlpha) + (_SpeedX));
            // StepperB->setAcceleration(getSpeedB()/ACC);
            // StepperB->setDeceleration(getSpeedB()/DEC); 
            // StepperB->goesTo(int(-RADIUS*float(_MoveAlpha)*((PI/180.0)/KSTP) + (float(_MovepositionX)/KSTP)));
            // _AckStpA = true;

        }else if (_Cmd == "STOP"){
            StepperA->stop();
           
        }else if (_Cmd == "ACK"){
            _AckStpA = false;
        }
    }
}

// void Holonome::routine_stepperB(void)
// {
//    while (1)
//    {
//         if(_Cmd == "MOVE" and _AckStpB ==false){

//             // StepperB->setSpeed((-RADIUS*_SpeedAlpha) + (_SpeedX));
//             // StepperB->setAcceleration(getSpeedB()/ACC);
//             // StepperB->setDeceleration(getSpeedB()/DEC); 
//             // StepperB->move(int(-RADIUS*float(_MoveAlpha)*((PI/180.0)/KSTP) + (float(_MovepositionX)/KSTP)));
//             _AckStpB = true;

//         }else if (_Cmd == "GOTO" and _AckStpB ==false){    
//             // StepperB->setSpeed((-RADIUS*_SpeedAlpha) + (_SpeedX));
//             // StepperB->setAcceleration(getSpeedB()/ACC);
//             // StepperB->setDeceleration(getSpeedB()/DEC); 
//             // StepperB->goesTo(int(-RADIUS*float(_MoveAlpha)*((PI/180.0)/KSTP) + (float(_MovepositionX)/KSTP)));
//             _AckStpB = true;

//         }else if (_Cmd == "STOP"){
//             //StepperB->stop();
          
//         }else if (_Cmd == "ACK"){
//             _AckStpB = false;
//         }
//     }
// }

// void Holonome::routine_stepperC(void)
// {
//     while (1) 
//     {
//         if(_Cmd == "MOVE" and _AckStpC ==false){
//             // StepperC->setSpeed((-RADIUS*_SpeedAlpha) - (0.5*(_SpeedX)) + (sin(PI/3.0)*(_SpeedY))); 
//             // StepperC->setAcceleration(getSpeedC()/ACC);
//             // StepperC->setDeceleration(getSpeedC()/DEC); 
//             // StepperC->move(int((-RADIUS*float(_MoveAlpha)*((PI/180.0)/KSTP)) - (0.5*(float(_MovepositionX)/KSTP)) + (sin(PI/3.0)*(float(_MovepositionY)/KSTP)))); 
//             _AckStpC = true;

//         }else if (_Cmd == "GOTO" and _AckStpC ==false){   
//             // StepperC->setSpeed((-RADIUS*_SpeedAlpha) - (0.5*(_SpeedX)) + (sin(PI/3.0)*(_SpeedY))); 
//             // StepperC->setAcceleration(getSpeedC()/ACC);
//             // StepperC->setDeceleration(getSpeedC()/DEC); 
//             // StepperC->goesTo(int((-RADIUS*float(_MoveAlpha)*((PI/180.0)/KSTP)) - (0.5*(float(_MovepositionX)/KSTP)) + (sin(PI/3.0)*(float(_MovepositionY)/KSTP)))); 
//             _AckStpC = true;

//         }else if (_Cmd == "STOP"){
//             //StepperC->stop();
            
//         }else if (_Cmd == "ACK"){
//             _AckStpC = false;
//         } 
//     }    
// }

void Holonome::routine_holonome(void)
{
    while (1)
    {
        _positionX  =   (((2.0/3.0)*StepperB->getPosition()) - ((1.0/3.0)*StepperA->getPosition()) - ((1.0/3.0)*StepperC->getPosition())) * KSTP;     
        _positionY  =   ((-tan(PI/6.0)*StepperA->getPosition()) + (tan(PI/6.0)*StepperC->getPosition())) * KSTP;
        _Alpha      =   (((-1.0/(3.0*RADIUS))*StepperB->getPosition()) + ((-1.0/(3.0*RADIUS))*StepperA->getPosition()) + ((-1.0/(3.0*RADIUS))*StepperC->getPosition()))*KSTP/(PI/180.0);
    }
}